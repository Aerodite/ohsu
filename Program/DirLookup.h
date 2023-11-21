/*
osu! hitsound updater (o!hsu) - Directory Lookup | by Anthony Garcia Roman
Last edited 21/11/2023.
*/

#include <sys\stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <conio.h>
#include <windows.h>
#include <Lmcons.h>

using namespace std;
//Global Variable to find UserName
TCHAR UserName [ UNLEN + 1 ];
DWORD UserName_len = UNLEN + 1;

//Global Variable to find Skin Folder
string osuSkinFolder;
int DrLookup(){
    ifstream infile;

    //pointers to point to directories.
    struct dirent *d;
    struct stat dst;
    DIR *dir;

    //Startup Prompt
    cout << "o!hsu (Terminal Version) \n";
    cout << "Enter your skins directory (if it's set to default just type d) \n"
            "(Put in the format [DriveLetter]:\\.........\\osu!\\Skins)";
    cin >> osuSkinFolder;


    //Default case, goes to default osu! skin folder in AppData.
    if (osuSkinFolder == "d") {
        osuSkinFolder = "C:\\Users\\" + string(UserName) + "\\AppData\\Local\\osu!\\Skins";
    }
    //Adds additional backslash to the path for future use (finding the files themselves).
    string path = osuSkinFolder + "\\";

    //Opens the directory read into cin.
    dir = opendir(path.c_str());

    //Verifies directory is not NULL, and assigns a type to each type of file.
    //e.g. if it's a folder, it'll say it's a folder (mainly for debugging purposes).
    if (dir != NULL) {
        for(d = readdir(dir); d != NULL; d = readdir(dir))
        {
            string type = d->d_name;
            type = path + type;
            if (stat(type.c_str(), &dst) == 0)
            {
                if (dst.st_mode & S_IFDIR)
                {
                    // for verification if necessary
                    type = "is a FOLDER.";
                }
                else if (dst.st_mode & S_IFREG)
                {
                    // for verification if necessary
                    type = "is a FILE.";
                }
            }
            cout<<d->d_name<<endl;
        }
        closedir(dir);
    }

    //turn into pointer so can be addressed in SkinLookup.h to search
    string *osuSkinFolderPtr = &osuSkinFolder;

    return 0;
}

//initialise pointer to be able to be used in other file.
int *osuSkinFolderPtr();