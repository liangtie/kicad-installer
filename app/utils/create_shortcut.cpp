#include <filesystem>
#include <string>

#include "create_shortcut.h"

#include <windows.h>
#include <shlobj.h>
#include <winnls.h> 


HRESULT CreateShellLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc)
{
    HRESULT hres;
    IShellLink* psl;

    // Initialize COM
    hres = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hres))
    {
        // Create an IShellLink object
        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
        if (SUCCEEDED(hres))
        {
            IPersistFile* ppf;

            // Set the path to the shortcut target
            psl->SetPath(lpszPathObj);
            // Set the description for the shortcut
            psl->SetDescription(lpszDesc);

            // Query for the IPersistFile interface
            hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
            if (SUCCEEDED(hres))
            {
                // Save the shortcut to the specified file
                hres = ppf->Save(lpszPathLink, TRUE);
                ppf->Release();
            }
            psl->Release();
        }
        CoUninitialize();
    }
    return hres;
}


bool create_shortcut_on_desktop(const std::string& exe_path)
{
    // Get desktop folder path
    wchar_t desktopPath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOP, NULL, 0, desktopPath)))
        return false;

    // Convert input path to wide string
    std::filesystem::path exePathUtf8(exe_path);
    std::wstring wExePath = exePathUtf8.wstring();

    // Construct shortcut path: Desktop\YourApp.lnk
    std::wstring shortcutName = exePathUtf8.stem().wstring() + L".lnk";
    std::wstring shortcutFullPath = std::filesystem::path(desktopPath) / shortcutName;

    // Description for shortcut
    std::wstring description = L"Shortcut to " + exePathUtf8.stem().wstring();

    // Call helper
    HRESULT hr = CreateShellLink(
        wExePath.c_str(),
        shortcutFullPath.c_str(),
        description.c_str()
    );

    return SUCCEEDED(hr);
}