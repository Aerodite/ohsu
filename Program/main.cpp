#include "DirLookup.h"
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "SkinLookup.h"
#include <windows.h>

using namespace std;


//gets user input for the skin name(s)
string getUserInput(const string& prompt, const string& skinFolder){
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


//Copies file from source to destination
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
    //Deletes original skin hitsounds.
    static void deleteFile(const string& filePath){
        if (remove(filePath.c_str()) == 0) {
            cout << "Original file deleted successfully \n";
        } else {
            perror("Error deleting original file");
        }
    }

    //Processes the file, and moves it to the backup directory.
    static void processFile(const string& oldFilePath, const string& newFilePath, const string& backupFilePath) {
        struct stat buffer{};
        // Check if the old file exists
        if (stat(oldFilePath.c_str(), &buffer) == 0) {
            // Move the old file to the backup directory
            if (rename(oldFilePath.c_str(), backupFilePath.c_str()) != 0) {
                perror("Error moving old file to backup directory");
            }
            // Check if the new file exists
            if (stat(newFilePath.c_str(), &buffer) == 0) {
                // Copy the new file to the old file's location
                copyFile(newFilePath, oldFilePath);
            }
        }
    }
    //Processes the directory, and moves it to the backup directory.
    static void processDirectory(const string& oldOskHs, const string& newOskHs, const string& backupDirPath, DIR* dir){
        ifstream infile;
        for(const dirent *d = readdir(dir); d != nullptr; d = readdir(dir)) {
            infile.open("hsbanks.txt");
            if (infile.is_open()) {
                string line;
                while (getline(infile, line)) {
                    // Remove the file extension
                    string lineWithoutExtension = line.substr(0, line.find_last_of('.'));
                    string oldFilePath = oldOskHs + "\\" += line;
                    string newFilePath = newOskHs + "\\" += line;
                    string backupFilePath = backupDirPath + "\\" += line;
                    // Process all files in the client skin directory that match the filename
                    DIR* clientDir = opendir(oldOskHs.c_str());
                    dirent *clientFile;
                    while ((clientFile = readdir(clientDir)) != nullptr) {
                        string clientFileName = clientFile->d_name;
                        string clientFileNameWithoutExtension = clientFileName.substr(0, clientFileName.find_last_of('.'));
                        if (clientFileNameWithoutExtension == lineWithoutExtension) {
                            cout << "Processing file: " << clientFileName << "\n";
                            processFile(oldOskHs + "\\" += clientFileName, newOskHs + "\\" += clientFileName, backupFilePath);
                        }
                    }
                    closedir(clientDir);
                    if (infile.fail()) {
                        infile.clear();
                        infile.ignore('\n');
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

    const string initialDir = DirLookup::osuSkinFolder;
    cout << "\nPlease pick the skin that you want to change the hitsounds of. \n";
    const string clientSkinPtr = openFolderDialog(initialDir);
    cout << "> " << clientSkinPtr << ">\n";
    cout << "\nPlease pick the skin that you want to grab the hitsounds from. \n";
    const string hostSkinPtr = openFolderDialog(initialDir);
    cout << "> " << hostSkinPtr << ">\n";

    const string backupDirPath = clientSkinPtr + "\\oldhs";
    mkdir(backupDirPath.c_str());

    const string path = clientSkinPtr +  "\\";

    DIR *dir = opendir(path.c_str());

    if (dir != nullptr) {
        FileProcessor::processDirectory(clientSkinPtr, hostSkinPtr, backupDirPath, dir);
        closedir(dir);
    }

    cout << endl;
    cout << "Processing Complete!\n";
    cin.ignore();
    cin.get();

    return 0;
}