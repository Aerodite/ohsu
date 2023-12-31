/*
osu! hitsound updater (o!hsu) - by Anthony Garcia Roman
Last edited 21/11/2023.
*/

#include <dirent.h>
#include <conio.h>
#include <sys/stat.h>
#include <fstream>
#include "SkinLookup.h"
#include "DirLookup.h"
#include <iostream>
#include <ios>
#include <cstdio>

using namespace std;

//Skin to be edited
string clientSkin;
//Skin to take hitsounds from
string hostSkin;

int main() {
    //import directory lookup function
    DirLookup::DrLookup();
    SkinLookup::oskLookup();
    //request user input to search for the skins
    while (true) {
        cout << "From this list, which skin do you want to change the hitsounds of? (case & white space sensitive)\n";
        cin >> clientSkin;
        if (clientSkin.empty()) {
            cout << "Error: Skin name cannot be empty. Please try again.\n";
            continue;
        }
        string clientSkinPath = DirLookup::osuSkinFolder + "\\" + clientSkin;
        struct stat buffer{};
        if (stat(clientSkinPath.c_str(), &buffer) != 0) {
            cout << "Error: Skin directory does not exist. Please try again.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Which skin do you want to take the hitsounds from? (case & white space sensitive)\n";
        cin >> hostSkin;
        if (hostSkin.empty()) {
            cout << "Error: Skin name cannot be empty. Please try again.\n";
            continue;
        }
        string hostSkinPath = DirLookup::osuSkinFolder + "\\" + hostSkin;
        struct stat buffer{};
        if (stat(hostSkinPath.c_str(), &buffer) != 0) {
            cout << "Error: Skin directory does not exist. Please try again.\n";
            continue;
        }
        break;
    }

    //make able to search inside the skin files for the hitsounds.
    //DirLookup is a class, osuSkinFolder is a statics string variable inside the class.
    //ClientSkin is a string variable that is assigned by the user.
    //ClientSkinPtr is a pointer to the string variable.
    char *clientSkinPtr = const_cast<char *>((DirLookup::osuSkinFolder + "\\" + clientSkin).c_str());
    char *hostSkinPtr = const_cast<char *>((DirLookup::osuSkinFolder + "\\" + hostSkin).c_str());

    char *oldOskHs = {clientSkinPtr};
    char *newOskHs = {hostSkinPtr};




    //This won't work unless it's reinterpreted (?).
    char *oldHs[] = {(oldOskHs)};
    char *newHs[] = {(newOskHs)};


    struct stat dst{};


    DIR *dir;

    string path = string(clientSkinPtr) + "\\";

    dir = opendir(path.c_str());

    if (dir != nullptr) {
        dirent *d;
        ifstream infile;
        for(d = readdir(dir); d != nullptr; d = readdir(dir))
        {
            string type;
            type = d->d_name;
            type += path;
            if (stat(type.c_str(), &dst) == 0)
            {
                //Unnecessary for the function of the code.
                //Very helpful for debugging though.
                /*
                if (dst.st_mode & S_IFDIR)
                {
                    type = "is a folder.";
                }
                else if (dst.st_mode & S_IFREG)
                {
                    type = "is a file.";
                }
                 */
            }



            string path = string(clientSkinPtr) +  "\\";

            dir = opendir(path.c_str());

            string backupDirPath = string(clientSkinPtr) + "\\oldhs";
            mkdir(backupDirPath.c_str()); // Create new directory

            if (dir != nullptr) {
                for (d = readdir(dir); d != nullptr; d = readdir(dir)) {
                    string type = d->d_name;
                    type += path;
                    if (stat(type.c_str(), &dst) == 0) {
                    }

                    infile.open("hsbanks.txt");
                    if (infile.is_open()) {
                        while (!infile.eof()) {
                            string line;
                            getline(infile, line);
                            string oldFilePath = string(oldOskHs) + "\\" + line;
                            string newFilePath = string(newOskHs) + "\\" + line;
                            string backupFilePath = backupDirPath + "\\" + line;
                            struct stat buffer{};
                            if (stat(oldFilePath.c_str(), &buffer) == 0) { // Check if the file exists
                                int renameResult = rename(oldFilePath.c_str(), backupFilePath.c_str()); // Move file to new directory
                                if (renameResult != 0)
                                    perror("Error moving file to backup");
                                else
                                    cout << "File moved to backup successfully \n";
                                ifstream src(newFilePath, ios::binary);
                                ofstream dst(oldFilePath, ios::binary);
                                dst << src.rdbuf(); // Copy file
                                src.close();
                                dst.close();
                                if (!src.good() || !dst.good()) {
                                    cout << "Error copying file \n";
                                } else {
                                    cout << "File copied successfully \n";
                                }
                            }
                            if (infile.eof()) {
                                break;
                            }
                            if (infile.fail()) {
                                infile.clear();
                                infile.ignore('\n');
                                continue;
                            }
                        }
                        infile.close();
                    }
                }
                closedir(dir);
            }
            return 0;
        }
    }
}