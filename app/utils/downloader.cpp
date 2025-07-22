#include <QElapsedTimer>
#include <QFileInfo>
#include <QThread>
#include <atomic>
#include <fstream>
#include <memory>
#include <utility>

#include "downloader.h"

#include <cpr/cpr.h>
#include <cpr/response.h>

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

  cpr::Session session;
  session.SetUrl(cpr::Url {_url.toStdString()});

  const auto contentLength = session.GetDownloadFileLength();
  if (contentLength <= 0) {
    m_progress.error = true;
    m_progress.errorMessage =
        "Invalid content length or cannot determine file size.";
    emit downloadProgress(m_progress);
    return;
  }

  m_progress.total = static_cast<unsigned long long>(contentLength);
  m_progress.downloaded = 0;

  QElapsedTimer timer;
  timer.start();
  unsigned long long lastUpdateTime = 0;

  cpr::WriteCallback callback = {
      [this, &ofs, &timer, &lastUpdateTime](const std::string_view& data,
                                            intptr_t /* userdata */) -> bool
      {
        if (m_cancelled->load()) {
          m_progress.error = true;
          m_progress.errorMessage = "Download cancelled.";
          emit downloadProgress(m_progress);
          return false;
        }

        ofs.write(data.data(), data.size());
        m_progress.downloaded += data.size();

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
      }};

  session.SetCancellationParam(m_cancelled);
  cpr::Response result = session.Download(callback);
  if (result.error) {
    m_progress.error = true;
    m_progress.errorMessage = QString::fromStdString(result.error.message);
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
