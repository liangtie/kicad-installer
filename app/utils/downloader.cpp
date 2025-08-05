#include <QElapsedTimer>
#include <QFileInfo>
#include <QThread>
#include <QUrl>
#include <atomic>
#include <fstream>
#include <memory>
#include <utility>

#include "downloader.h"

#include "httplib_wrapper.h"

DOWNLOADER::DOWNLOADER(QString url, QString save_path)
    : _url(std::move(url))
    , _savePath(std::move(save_path))
{
}

DOWNLOADER::~DOWNLOADER()
{
  cancel();
}

void DOWNLOADER::start()
{
  downloadFile();
}

void DOWNLOADER::cancel()
{
  if (m_cancelled) {
    m_cancelled->store(true);
  }
}

void DOWNLOADER::downloadFile()
{
  m_progress = DOWNLOAD_PROGRESS {};
  m_cancelled = std::make_shared<std::atomic_bool>();

  std::ofstream ofs(_savePath.toStdString(), std::ios::binary);
  if (!ofs.is_open()) {
    m_progress.error = true;
    m_progress.errorMessage = "Failed to open file for writing";
    emit downloadProgress(m_progress);
    return;
  }

  // Parse URL to extract host and path
  QUrl url(_url);
  QString host = url.host();
  QString path = url.path();
  if (!url.query().isEmpty()) {
    path += "?" + url.query();
  }

  httplib::SSLClient client(host.toStdString(), url.port(443));

  // Get content length first
  auto head_res = client.Head(path.toStdString().c_str());
  if (!head_res || head_res->status != 200) {
    m_progress.error = true;
    m_progress.errorMessage = "Failed to get file information";
    emit downloadProgress(m_progress);
    return;
  }

  auto content_length_it = head_res->get_header_value("Content-Length");
  if (content_length_it.empty()) {
    m_progress.error = true;
    m_progress.errorMessage = "Cannot determine file size.";
    emit downloadProgress(m_progress);
    return;
  }

  m_progress.total = std::stoull(content_length_it);
  m_progress.downloaded = 0;

  QElapsedTimer timer;
  timer.start();
  unsigned long long lastUpdateTime = 0;

  // Download with progress tracking
  auto res = client.Get(
      path.toStdString().c_str(),
      [&](const char* data, size_t len) -> bool
      {
        if (m_cancelled->load()) {
          m_progress.error = true;
          m_progress.errorMessage = "Download cancelled.";
          emit downloadProgress(m_progress);
          return false;
        }

        ofs.write(data, len);
        m_progress.downloaded += len;

        auto elapsedMs = timer.elapsed();
        if (elapsedMs - lastUpdateTime >= 500
            || m_progress.downloaded == m_progress.total)
        {
          lastUpdateTime = elapsedMs;

          double seconds = elapsedMs / 1000.0;
          m_progress.speed = static_cast<unsigned long long>(
              m_progress.downloaded / (seconds > 0 ? seconds : 1));
          m_progress.eta = m_progress.speed > 0
              ? (m_progress.total - m_progress.downloaded) / m_progress.speed
              : 0;

          emit downloadProgress(m_progress);
        }

        return true;
      });

  if (!res || res->status != 200) {
    m_progress.error = true;
    m_progress.errorMessage = "Download failed with HTTP status: "
        + QString::number(res ? res->status : 0);
    emit downloadProgress(m_progress);
    return;
  }

  if (!m_cancelled->load()) {
    m_progress.downloaded = m_progress.total;
    m_progress.finished = true;
    m_progress.eta = 0;
    emit downloadProgress(m_progress);
  }
}
