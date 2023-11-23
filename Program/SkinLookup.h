/*
osu! hitsound updater (o!hsu) - Skin Lookup | by Anthony Garcia Roman
Last edited 21/11/2023.
*/

#include <sys\stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <conio.h>
#include <windows.h>
#include <Lmcons.h>
#include "Program/DirLookup.h"

using namespace std;

//Global Variable to find Skin Folder
int DrLookup();
int oskLookup(){
    ifstream infile;

    struct dirent *d;
    struct stat dst;


    DIR *dir;

    string path = osuSkinFolder + "\\";

    dir = opendir(path.c_str());

    if (dir != NULL) {
        for(d = readdir(dir); d != NULL; d = readdir(dir))
        {
            string type = d->d_name;
            type = path + type;
            if (stat(type.c_str(), &dst) == 0)
            {
                /*
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

    return 0;
}