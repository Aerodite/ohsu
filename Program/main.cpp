#include "DirLookup.h"
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "SkinLookup.h"
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <shlobj.h>
#include <vector>
#include <algorithm>


// Function to convert string to wstring
inline std::wstring string_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

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
            cout << "Old file exists: " << oldFilePath << "\n";
            // Move the old file to the backup directory
            if (rename(oldFilePath.c_str(), backupFilePath.c_str()) != 0) {
                perror("Error moving old file to backup directory");
            } else {
                cout << "Old file moved to backup directory successfully \n";
                // Check if the new file exists
                if (stat(newFilePath.c_str(), &buffer) == 0) {
                    cout << "New file exists: " << newFilePath << "\n";
                    // Copy the new file to the old file's location
                    ifstream src(newFilePath, ios::binary);
                    ofstream dst(oldFilePath, ios::binary);
                    dst << src.rdbuf();
                    src.close();
                    dst.close();
                    if (src.fail() || dst.fail()) {
                        perror("Error copying new file to old file's location");
                    } else {
                        cout << "New file copied to old file's location successfully \n";
                    }
                } else {
                    cout << "New file does not exist: " << newFilePath << "\n";
                }
            }
        } else {
            cout << "Old file does not exist: " << oldFilePath << "\n";
        }
    }

    static void convertExtension(const string& oldFilePath, const string& originalExtension) {
        size_t pos = oldFilePath.find_last_of(".");
        if (pos != string::npos) {
            const string oldFilePathWithoutExtension = oldFilePath.substr(0, pos);
            const string newFilePath = oldFilePathWithoutExtension + originalExtension;
            if (rename(oldFilePath.c_str(), newFilePath.c_str()) != 0) {
                perror("Error converting file extension");
            } else {
                cout << "File extension converted successfully \n";
            }
        } else {
            cout << "Old file does not have an extension: " << oldFilePath << "\n";
        }
    }

    //Processes the directory, and moves it to the backup directory.
static void processDirectory(const string& oldOskHs, const string& newOskHs, const string& backupDirPath, DIR* dir){
    //cout << "Entering processDirectory function\n";
    ifstream infile("hsbanks.txt");
    vector<string> lines;
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            lines.push_back(line.substr(0, line.find_last_of('.'))); // store filename without extension
        }
        infile.close();
    }

    const std::wstring oldPath = string_to_wstring(oldOskHs +  "\\");
    _WDIR *oldDir = _wopendir(oldPath.c_str());

    if (oldDir != nullptr) {
        struct _wdirent *d;
        while ((d = _wreaddir(oldDir)) != nullptr) {
            wstring oldFileName = d->d_name;
            string oldFileNameStr = wstring_to_string(oldFileName);
            size_t pos = oldFileNameStr.find_last_of(".");
            if (pos != string::npos) {
                wstring newFileName = d->d_name;
                string newFileNameStr = wstring_to_string(newFileName);
                size_t pos = newFileNameStr.find_last_of(".");

                string oldFileExtension = oldFileNameStr.substr(pos); // get extension from filename
                string newFileExtension = newFileNameStr.substr(pos); // get extension from filename
                string newFileNameWithoutExtension = newFileNameStr.substr(0, pos); // get filename without extension
                string oldFileNameWithoutExtension = oldFileNameStr.substr(0, pos); // get filename without extension
                if (find(lines.begin(), lines.end(), oldFileNameWithoutExtension) != lines.end()) {
                    cout << "Processing file: " << oldFileNameWithoutExtension << "\n";
                    string oldFilePath = oldOskHs + "\\" + oldFileNameWithoutExtension + newFileExtension; // use old file extension
                    string backupFilePath = backupDirPath + "\\" + oldFileNameWithoutExtension + newFileExtension; // use old file extension

                    const std::wstring newPath = string_to_wstring(newOskHs +  "\\");
                    _WDIR *newDir = _wopendir(newPath.c_str());

                    if (newDir != nullptr) {
                        struct _wdirent *d;
                        while ((d = _wreaddir(newDir)) != nullptr) {
                            wstring newFileName = d->d_name;
                            string newFileNameStr = wstring_to_string(newFileName);
                            size_t pos = newFileNameStr.find_last_of(".");
                            if (pos != string::npos) {
                                string newFileExtension = newFileNameStr.substr(pos); // get extension from filename
                                string newFileNameWithoutExtension = newFileNameStr.substr(0, pos); // get filename without extension
                                if (newFileNameWithoutExtension == oldFileNameWithoutExtension) {
                                    string backupFilePath = backupDirPath + "\\" + oldFileNameWithoutExtension + oldFileExtension; // use new file extension
                                    string newFilePath = newOskHs + "\\" + newFileNameWithoutExtension + newFileExtension; // use new file extension
                                    processFile(oldFilePath, newFilePath, backupFilePath);
                                  //  cout << "Calling processFile function\n";
                                    convertExtension(oldFilePath, newFileExtension);
                                    cout << "Converting file extension\n";
                                }
                            }

                        }
                        _wclosedir(newDir);
                    }
                }
            }
        }
        _wclosedir(oldDir);
    } else {
        wcout << L"Failed to open old skin directory: " << oldPath << L"\n";
    }
}
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    DirLookup::DrLookup();
    SkinLookup::oskLookup();

    const string initialDir = DirLookup::osuSkinFolder;
    cout << "\nPlease pick the skin that you want to change the hitsounds of. \n";
    const string clientSkinPtr = openFolderDialog(initialDir);
    cout << "> " << clientSkinPtr << ">\n";
    cout << "\nPlease pick the skin that you want to grab the hitsounds from. \n";
    const string hostSkinPtr = openFolderDialog(initialDir);
    cout << "> " << hostSkinPtr << ">\n";

    const std::wstring backupDirPath = string_to_wstring(clientSkinPtr + "\\oldhs");
    if (!CreateDirectoryW(backupDirPath.c_str(), NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            wcout << L"Error creating backup directory\n";
            return 1; // Exit the program if the directory could not be created
        }
    }

    std::string backupDirPathStr = wstring_to_string(backupDirPath);

    const string path = clientSkinPtr +  "\\";

    DIR *dir = opendir(path.c_str());

    if (dir != nullptr) {
        cout << "Directory opened successfully: " << path << "\n";
        FileProcessor::processDirectory(clientSkinPtr, hostSkinPtr, backupDirPathStr, dir);
        closedir(dir);
    } else {
        cout << "Failed to open directory: " << path << "\n";
    }

    cout << endl;
    cout << "Processing Complete!\n";
    cin.ignore();
    cin.get();

    return 0;
}