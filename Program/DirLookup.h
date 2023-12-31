/*
osu! hitsound updater (o!hsu) - Directory Lookup | by Anthony Garcia Roman
Last edited 21/11/2023.
*/

#ifndef OHSU_DIRLOOKUP_H
#define OHSU_DIRLOOKUP_H

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <conio.h>
#include <windows.h>
#include <Lmcons.h>

using namespace std;
//Global Variable to find UserName


//Global Variable to find Skin Folder
class DirLookup {
public:
    static int DrLookup(){
        ifstream infile;

        //pointers to point to directories.
        struct dirent *d;
        struct stat dst;

        //Startup Prompt
        cout << "o!hsu (Terminal Version) \n";
        while (true) {
            cout << "Enter your skins directory (if it's set to default just type d) \n"
            "(Put in the format [DriveLetter]:\\.........\\osu!\\Skins) \n";
            cin >> DirLookup::osuSkinFolder;
            if (DirLookup::osuSkinFolder.empty()) {
                cout << "Error: Skin folder cannot be empty. Please try again.\n";
                continue;
            }
            struct stat buffer{};
            if (stat(DirLookup::osuSkinFolder.c_str(), &buffer) != 0) {
                cout << "Error: Skins directory does not exist. Please try again.\n";
                continue;
            }
            break;
        }





        //Default case, goes to default osu! skin folder in AppData.
        if (osuSkinFolder == "d") {
            TCHAR windowsUserName [UNLEN + 1];
            osuSkinFolder = "C:\\Users\\" + string(windowsUserName) + R"(\AppData\Local\osu!\Skins)";
        }
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
                cout<<d->d_name<<endl;
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
