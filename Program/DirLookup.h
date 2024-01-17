#ifndef OHSU_DIRLOOKUP_H
#define OHSU_DIRLOOKUP_H

#include <windows.h>
#include <string>
#include <iostream>

//Get the osu! directory from the registry.
inline std::wstring getOsuDirectory() {
    HKEY hKey;
    wchar_t buf[255] = {0};
    DWORD dwType = 0;
    DWORD dwBufSize = sizeof(buf);
    const wchar_t* subkey = L"osu!\\DefaultIcon";

    if(LONG result = RegOpenKeyEx(HKEY_CLASSES_ROOT, subkey, 0, KEY_QUERY_VALUE, &hKey); result == ERROR_SUCCESS)
    {
        dwType = REG_SZ;
        result = RegQueryValueEx(hKey, L"", 0, &dwType, reinterpret_cast<BYTE *>(buf), &dwBufSize);
        if(result == ERROR_SUCCESS)
        {
            std::wstring osuPath = buf;
            osuPath = osuPath.substr(0, osuPath.find_last_of(L','));
            osuPath = osuPath.substr(1, osuPath.length() - 2);
            osuPath = osuPath.substr(0, osuPath.find_last_of(L'\\'));
            osuPath += L"\\Skins";
            RegCloseKey(hKey);
            return osuPath;
        }
        std::wcout << L"RegQueryValueEx failed with error: " << result << std::endl;
        RegCloseKey(hKey);
    }
    return L"";
}

#endif