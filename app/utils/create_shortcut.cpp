#include <filesystem>
#include <string>

#include "create_shortcut.h"

#include <comdef.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <windows.h>

bool create_shortcut_on_desktop(std::string const& exe_path)
{
  // Convert std::string to wide string
  std::wstring w_exe_path(exe_path.begin(), exe_path.end());

  // Initialize COM
  HRESULT hr = CoInitialize(nullptr);
  if (FAILED(hr)) {
    return false;
  }

  // Get desktop path
  wchar_t desktop_path[MAX_PATH];
  if (FAILED(
          SHGetFolderPathW(nullptr, CSIDL_DESKTOP, nullptr, 0, desktop_path)))
  {
    CoUninitialize();
    return false;
  }

  // Extract filename from exe_path
  std::filesystem::path p(exe_path);
  std::wstring filename = p.stem().wstring();

  // Build full path for the shortcut
  std::wstring shortcut_path =
      std::wstring(desktop_path) + L"\\" + filename + L".lnk";

  // Create IShellLink object
  IShellLinkW* pShellLink = nullptr;
  hr = CoCreateInstance(CLSID_ShellLink,
                        nullptr,
                        CLSCTX_INPROC_SERVER,
                        IID_IShellLinkW,
                        reinterpret_cast<void**>(&pShellLink));
  if (FAILED(hr)) {
    CoUninitialize();
    return false;
  }

  // Set the path to the .exe file
  pShellLink->SetPath(w_exe_path.c_str());
  pShellLink->SetDescription(L"Shortcut to application");

  // Query for the IPersistFile interface for saving the shortcut
  IPersistFile* pPersistFile = nullptr;
  hr = pShellLink->QueryInterface(IID_IPersistFile,
                                  reinterpret_cast<void**>(&pPersistFile));
  if (SUCCEEDED(hr)) {
    hr = pPersistFile->Save(shortcut_path.c_str(), TRUE);
    pPersistFile->Release();
  }

  pShellLink->Release();
  CoUninitialize();

  return SUCCEEDED(hr);
}
