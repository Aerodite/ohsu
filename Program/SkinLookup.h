/*
osu! hitsound updater (o!hsu) - Skin Lookup | by Anthony Garcia Roman
Last edited 31/12/2023.
*/

#ifndef OHSU_SKINLOOKUP_H
#define OHSU_SKINLOOKUP_H

#include <sys\stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <conio.h>
#include <windows.h>
#include <Lmcons.h>
#include "Program/DirLookup.h"

using namespace std;

class SkinLookup {

public:
//Global Variable to find Skin Folder
    static int oskLookup() {
        ifstream infile;

        struct stat dst{};


        const string path = DirLookup::osuSkinFolder + "\\";

        DIR *dir = opendir(path.c_str());

        if (dir != nullptr) {
            for (const struct dirent *d = readdir(dir); d != nullptr; d = readdir(dir)) {
                string type = d->d_name;
                type = path + type;
                if (stat(type.c_str(), &dst) == 0) {
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
                cout << d->d_name << endl;
            }
            closedir(dir);
        }

        return 0;
    }
};
#endif