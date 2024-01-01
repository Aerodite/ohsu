#ifndef OHSU_DIRLOOKUP_H
#define OHSU_DIRLOOKUP_H

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <windows.h>
#include <Lmcons.h>
#include <shobjidl.h>
#include <locale>
#include <codecvt>

using namespace std;

// Function to convert wstring to string
inline string wstring_to_string(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

inline std::string openFolderDialog(const std::string& initialDir)
{
    IFileDialog *pfd;
    std::wstring folderPath;

    // Initialize COM library
    CoInitialize(NULL);

    // Create the FileOpenDialog object.
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

    if (SUCCEEDED(hr))
    {
        DWORD dwOptions;
        hr = pfd->GetOptions(&dwOptions);
        if (SUCCEEDED(hr))
        {
            // Set the options to select folders and force filesystem.
            hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
            if (SUCCEEDED(hr))
            {
                // Set the initial directory.
                IShellItem *psi;
                hr = SHCreateItemFromParsingName(std::wstring(initialDir.begin(), initialDir.end()).c_str(), NULL, IID_PPV_ARGS(&psi));
                if (SUCCEEDED(hr))
                {
                    pfd->SetFolder(psi);
                    psi->Release();
                }

                // Show the Open dialog box.
                hr = pfd->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    hr = pfd->GetResult(&psi);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR path;
                        hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &path);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            folderPath = path;
                            CoTaskMemFree(path);
                        }
                        psi->Release();
                    }
                }
            }
        }
        pfd->Release();
    }

    // Uninitialize COM library
    CoUninitialize();

    return wstring_to_string(folderPath);
}

class DirLookup {
public:
    static int DrLookup(){
        ifstream infile;

        //pointers to point to directories.
        struct dirent *d;
        struct stat dst;

        //Startup Prompt
        cout << "o!hsu (Terminal Version) \n";
        cout << "Please pick the skins directory. \n";
        const string initialDir = "C:\\";
        osuSkinFolder = openFolderDialog(initialDir);

        //Adds additional backslash to the path for future use (finding the files themselves).
        const string path = osuSkinFolder + "\\";

        //Opens the directory read into cin.
        DIR *dir = opendir(path.c_str());

        //Verifies directory is not NULL, and assigns a type to each type of file.
        //e.g. if it's a folder, it'll say it's a folder (mainly for debugging purposes).
        if (dir != NULL) {
            for(d = readdir(dir); d != NULL; d = readdir(dir))
            {
                string type = d->d_name;
                type = path + type;
                if (stat(type.c_str(), &dst) == 0)
                {/*
                    if (dst.st_mode & S_IFDIR)
                    {
                        // for verification if necessary
                        type = "is a FOLDER.";
                    }
                    else if (dst.st_mode & S_IFREG)
                    {
                        // for verification if necessary
                        type = "is a FILE.";
                    }*/
                }

            }
            closedir(dir);
        }

        //turn into pointer so can be addressed in SkinLookup.h to search
        extern string osuSkinFolder;

        return 0;
    }
    static string osuSkinFolder;
    string *osuSkinFolderPtr = &osuSkinFolder;

};
string DirLookup::osuSkinFolder;

#endif