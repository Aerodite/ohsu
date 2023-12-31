#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "SkinLookup.h"
#include "DirLookup.h"

using namespace std;

string getUserInput(const string& prompt, const string& skinFolder) {
    string userInput;
    while (true) {
        cout << prompt;
        cin >> userInput;
        if (userInput.empty()) {
            cout << "Error: Skin name cannot be empty. Please try again.\n";
            continue;
        }
        string skinPath = skinFolder + "\\" += userInput;
        struct stat buffer{};
        if (stat(skinPath.c_str(), &buffer) != 0) {
            cout << "Error: Skin directory does not exist. Please try again.\n";
            continue;
        }
        break;
    }
    return userInput;
}

class FileProcessor {
public:
    static void copyFile(const string& sourcePath, const string& destinationPath) {
        ifstream src(sourcePath, ios::binary);
        ofstream dst(destinationPath, ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
        if (src.good() || dst.good()) {{
            cout << "File copied successfully \n";
        }
        }}

    static void deleteFile(const string& filePath) {
        if (remove(filePath.c_str()) == 0) {
            cout << "Original file deleted successfully \n";
        } else {
            perror("Error deleting original file");
        }
    }


    static void processFile(const string& oldFilePath, const string& newFilePath, const string& backupFilePath) {
        struct stat buffer{};
        // Check if the old file exists
        if (stat(oldFilePath.c_str(), &buffer) == 0) {
            // Move the old file to the backup directory
            if (rename(oldFilePath.c_str(), backupFilePath.c_str()) != 0) {
            }
            // Check if the new file exists
            if (stat(newFilePath.c_str(), &buffer) == 0) {
                // Copy the new file to the old file's location
                copyFile(newFilePath, oldFilePath);
            }
        }
    }

    static void processDirectory(const string& oldOskHs, const string& newOskHs, const string& backupDirPath, DIR* dir) {
        ifstream infile;
        for(const dirent *d = readdir(dir); d != nullptr; d = readdir(dir)) {
            infile.open("hsbanks.txt");
            if (infile.is_open()) {
                while (!infile.eof()) {
                    string line;
                    getline(infile, line);
                    string oldFilePath = string(oldOskHs) + "\\" + line;
                    string newFilePath = string(newOskHs) + "\\" + line;
                    string backupFilePath = backupDirPath + "\\" += line;
                    processFile(oldFilePath, newFilePath, backupFilePath);
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
    }
};

int main() {
    DirLookup::DrLookup();
    SkinLookup::oskLookup();

    const string clientSkin = getUserInput("From this list, which skin do you want to change the hitsounds of? (case & white space sensitive)\n", DirLookup::osuSkinFolder);
    const string hostSkin = getUserInput("Which skin do you want to take the hitsounds from? (case & white space sensitive)\n", DirLookup::osuSkinFolder);

    const string clientSkinPtr = DirLookup::osuSkinFolder + "\\" + clientSkin;
    const string hostSkinPtr = DirLookup::osuSkinFolder + "\\" + hostSkin;

    const string backupDirPath = clientSkinPtr + "\\oldhs";
    mkdir(backupDirPath.c_str());

    const string path = clientSkinPtr +  "\\";

    DIR *dir = opendir(path.c_str());

    if (dir != nullptr) {
        FileProcessor::processDirectory(clientSkinPtr, hostSkinPtr, backupDirPath, dir);
        closedir(dir);
    }

    return 0;
}