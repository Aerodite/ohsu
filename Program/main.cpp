/*
osu! hitsound updater (o!hsu) - by Anthony Garcia Roman
Last edited 21/11/2023.
*/

#include <dirent.h>
#include <conio.h>
#include <sys\stat.h>
#include <fstream>
#include "SkinLookup.h"
#include "DirLookup.h"
#include <iostream>
#include <ios>
#include <stdio.h>

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
    cout
    <<
    "----------------\n"
    <<
    "From this list, which skin do you want to change the hitsounds of? (case & white space sensitive)\n";
    cin >> clientSkin;
    cout << "Which skin do you want to take the hitsounds from? (case & white space sensitive)\n";
    cin >> hostSkin;

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


    ifstream infile;
    struct dirent *d;
    struct stat dst{};



    DIR *dir;

    string path = string(clientSkinPtr) + "\\";

    dir = opendir(path.c_str());

    if (dir != nullptr) {
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

            if (dir != NULL) {
                for (d = readdir(dir); d != NULL; d = readdir(dir)) {
                    string type = d->d_name;
                    type += path;
                    if (stat(type.c_str(), &dst) == 0) {
                    }

                        infile.open("hsbanks.txt");
                        if (infile.is_open()) {
                            while (!infile.eof()) {
                                string line;
                                getline(infile, line);
                                if (remove(oldOskHs) == 0)
                                    printf("Deleted Successfully \n");
                                else
                                    printf("Unable to delete the file \n");
                                if (line == d->d_name) {
                                    //Deletes the file at location if exists
                                    if (rename(oldOskHs, newOskHs) != 0)
                                        perror("Error moving file");
                                    else(cout << "Moving file(s)... \n");


                                    //Creates a new file at location
                                    if (infile.eof()) {
                                        break;
                                    }
                                    if (infile.fail()) {
                                        infile.clear();
                                        infile.ignore('\n');
                                        continue;
                                    }
                                }
                        }
                        infile.close();
                    }
                }
            }
            closedir(dir);
        }
    }
    return 0;
}