#include <algorithm>
#include <codecvt>
#include <dirent.h>
#include <fstream>
#include <fstream>
#include <iostream>
#include <iostream>
#include <locale>
#include <QApplication>
#include <QBoxLayout>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMetaObject>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QToolBar>
#include <shlobj.h>
#include <string>
#include <vector>
#include <windows.h>
#include <sys/stat.h>
#include <QComboBox>
#include <fcntl.h>
#include <io.h>
#include <set>


#include "DirLookup.h"
#include "SkinLookup.h"
#include "MainWindow.h"



const QMetaObject *MainWindow::metaObject() const {
    return QMainWindow::metaObject();
}

void *MainWindow::qt_metacast(const char *string) {
    return QMainWindow::qt_metacast(string);
}

int MainWindow::qt_metacall(const QMetaObject::Call call, const int i, void **p) {
    return QMainWindow::qt_metacall(call, i, p);
}

void MainWindow::qt_static_metacall(QObject *, QMetaObject::Call, int, void **) {
}

inline std::wstring string_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}
//Copies file from source to destination
class FileProcessor {
public:
    static void copyFile(const std::wstring& sourcePath, const std::wstring& destinationPath) {
        if (!CopyFileW(sourcePath.c_str(), destinationPath.c_str(), FALSE)) {
            std::wcerr << L"Failed to copy file: " << sourcePath << L" to " << destinationPath << std::endl;
        }
    }
    //Deletes original skin hitsounds.
    static void deleteFile(const string& filePath){
        if (remove(filePath.c_str()) == 0) {
            cout << "Original file deleted successfully\n";
        } else {
            perror("Error deleting original file");
        }
    }
     // Returns a vector of the paths of the hitsound files in a given directory
    static std::vector<std::wstring> getHitsoundPaths(const std::wstring& directoryPath) {
        std::vector<std::wstring> hitsoundPaths;

        // Open the directory
        _WDIR* dir = _wopendir(directoryPath.c_str());
        if (dir == nullptr) {
            std::wcerr << L"Failed to open directory: " << directoryPath << std::endl;
            return hitsoundPaths;
        }

        // Read the directory
        struct _wdirent* entry;
        while ((entry = _wreaddir(dir)) != nullptr) {
            // Check if the file is a hitsound file
            if (isHitsoundFile(entry->d_name)) {
                // Construct the path of the hitsound file
                std::wstring hitsoundPath = directoryPath + L"\\" + entry->d_name;

                // Add the hitsound file path to the hitsoundPaths vector
                hitsoundPaths.push_back(hitsoundPath);
            }
        }

        // Close the directory
        _wclosedir(dir);

        return hitsoundPaths;
    }

    // Reads the hsbanks.txt file and stores the hitsound names in a set
    static std::set<std::wstring> readHsbanks() {
        std::set<std::wstring> hsbanks;
        // Open the hsbanks.txt file
        std::wifstream file(L"hsbanks.txt");
        std::wstring line;
        // Read the file line by line
        while (std::getline(file, line)) {
            // Add the line to the set
            hsbanks.insert(line);
        }
        return hsbanks;
    }

    // Checks if a file is a hitsound file
    static bool isHitsoundFile(const wchar_t* fileName) {
        // Define the hitsound file extensions
        std::vector<std::wstring> hitsoundExtensions = {L".wav", L".mp3"};

        // Get the extension of the file
        std::wstring fileExtension = std::filesystem::path(fileName).extension().wstring();

        // Check if the file has a hitsound file extension
        return std::find(hitsoundExtensions.begin(), hitsoundExtensions.end(), fileExtension) != hitsoundExtensions.end();
    }

    // Checks if a file exists
    static bool fileExists(const std::wstring& filePath) {
        return std::filesystem::exists(filePath);
    }

    // Returns the extension of a file
    static std::wstring getFileExtension(const std::wstring& filePath) {
        return std::filesystem::path(filePath).extension().wstring();
    }

    // Converts a file to .wav format
    static void convertToWav(const std::wstring& sourcePath, const std::wstring& destinationPath) {
        // You need to implement this function
    }

    // Converts a file to .mp3 format
    static void convertToMp3(const std::wstring& sourcePath, const std::wstring& destinationPath) {
        // You need to implement this function
    }
    static std::wstring getFileNameWithoutExtension(const std::wstring& filePath) {
        return std::filesystem::path(filePath).stem().wstring();
    }

static void changeHitsounds(QComboBox* targetSkinComboBox, QComboBox* sourceSkinComboBox) {
    // Get the selected skins
    QString selectedSkin = targetSkinComboBox->currentText();
    QString selectedHitsounds = sourceSkinComboBox->currentText();

    // Convert the selected skins to std::wstring
    std::wstring selectedSkinWstr = selectedSkin.toStdWString();
    std::wstring selectedHitsoundsWstr = selectedHitsounds.toStdWString();

    // Get the paths of the hitsound files in the source skin directory
    std::vector<std::wstring> sourceHitsoundPaths = getHitsoundPaths(selectedHitsoundsWstr);

    // Read the hsbanks.txt file and store the hitsound names in a set
    std::set<std::wstring> hsbanks = readHsbanks();

    // For each hitsound file in the source skin directory
    for (const std::wstring& sourceHitsoundPath : sourceHitsoundPaths) {
        // Get the name of the hitsound file without the extension
        std::wstring hitsoundName = getFileNameWithoutExtension(sourceHitsoundPath);

        // Check if the hitsound name is listed in hsbanks.txt
        if (hsbanks.find(hitsoundName) != hsbanks.end()) {
            // Construct the path of the hitsound file in the target skin directory
            std::wstring targetHitsoundPath = selectedSkinWstr + L"\\" + hitsoundName;

            // Check if the hitsound file in the target skin directory exists
            if (fileExists(targetHitsoundPath + L".wav")) {
                // If the hitsound file in the source skin directory is a .wav file, replace the hitsound file in the target skin directory
                if (getFileExtension(sourceHitsoundPath) == L".wav") {
                    copyFile(sourceHitsoundPath, targetHitsoundPath + L".wav");
                }
                // If the hitsound file in the source skin directory is not a .wav file, convert it to .wav and replace the hitsound file in the target skin directory
                else {
                    convertToWav(sourceHitsoundPath, targetHitsoundPath + L".wav");
                }
            }
            else if (fileExists(targetHitsoundPath + L".mp3")) {
                // If the hitsound file in the source skin directory is a .mp3 file, replace the hitsound file in the target skin directory
                if (getFileExtension(sourceHitsoundPath) == L".mp3") {
                    copyFile(sourceHitsoundPath, targetHitsoundPath + L".mp3");
                }
                // If the hitsound file in the source skin directory is not a .mp3 file, convert it to .mp3 and replace the hitsound file in the target skin directory
                else {
                    convertToMp3(sourceHitsoundPath, targetHitsoundPath + L".mp3");
                }
            }
        }
    }
}


 //Processes the file, and moves it to the backup directory.
    void processFile(const std::wstring& oldFilePath, const std::wstring& newFilePath, const std::wstring& backupFilePath) {
        struct _stat buffer{};
        // Check if the old file exists
        if (_wstat(oldFilePath.c_str(), &buffer) == 0) {
            std::wcout << L"Old file exists: " << oldFilePath << L"\n";
            // Move the old file to the backup directory
            if (_wrename(oldFilePath.c_str(), backupFilePath.c_str()) != 0) {
                perror("Error moving old file to backup directory");
            } else {
                std::wcout << L"Old file moved to backup directory successfully\n";
                // Check if the new file exists
                if (_wstat(newFilePath.c_str(), &buffer) == 0) {
                    std::wcout << L"New file exists: " << newFilePath << L"\n";
                    // Copy the new file to the old file's location
                    FILE* srcFile = _wfopen(newFilePath.c_str(), L"rb");
                    FILE* dstFile = _wfopen(oldFilePath.c_str(), L"wb");
                    char buffer[BUFSIZ];
                    size_t size;

                    while (size = fread(buffer, 1, BUFSIZ, srcFile)) {
                        fwrite(buffer, 1, size, dstFile);
                    }

                    fclose(srcFile);
                    fclose(dstFile);

                    if (!srcFile || !dstFile) {
                        perror("Error copying new file to old file's location");
                    } else {
                        std::wcout << L"New file copied to old file's location successfully\n";
                    }
                } else {
                    std::wcout << L"New file does not exist: " << newFilePath << L"\n";
                }
            }
        } else {
            std::wcout << L"Old file does not exist: " << oldFilePath << L"\n";
        }
    }

static void convertExtension(const std::wstring& oldFilePath, const std::wstring& newFileExtension) {
    size_t pos = oldFilePath.find_last_of(L'.');
    if (pos != std::wstring::npos) {
        std::wstring newFilePath = oldFilePath.substr(0, pos) + newFileExtension;
        if (_wrename(oldFilePath.c_str(), newFilePath.c_str()) != 0) {
            perror("Error converting file extension");
        } else {
            std::wcout << L"File extension converted successfully\n";
        }
    } else {
        std::wcout << L"Old file does not have an extension: " << oldFilePath << L"\n";
    }
}

    //Processes the directory, and moves it to the backup directory.
static void processDirectory(const std::wstring& oldOskHs, const std::wstring& newOskHs, const std::wstring& backupDirPath, _WDIR* dir){
    std::wifstream infile(L"hsbanks.txt");
    std::vector<std::wstring> lines;
    if (infile.is_open()) {
        std::wstring line;
        while (std::getline(infile, line)) {
            lines.push_back(line.substr(0, line.find_last_of(L'.'))); // store filename without extension
        }
        infile.close();
    }

    const std::wstring oldPath = oldOskHs +  L"/";

    if (_WDIR *oldDir = _wopendir(oldPath.c_str()); oldDir != nullptr) {
        _wdirent *d;
        while ((d = _wreaddir(oldDir)) != nullptr) {
            std::wstring oldFileName = d->d_name;
            std::wstring oldFileNameStr = oldFileName;
            size_t pos = oldFileNameStr.find_last_of(L'.');
            if (pos != std::wstring::npos) {
                std::wstring newFileName = d->d_name;
                std::wstring newFileNameStr = newFileName;
                size_t pos = newFileNameStr.find_last_of(L'.');

                std::wstring oldFileExtension = oldFileNameStr.substr(pos); // get extension from filename
                std::wstring newFileExtension = newFileNameStr.substr(pos); // get extension from filename
                std::wstring newFileNameWithoutExtension = newFileNameStr.substr(0, pos); // get filename without extension
                std::wstring oldFileNameWithoutExtension = oldFileNameStr.substr(0, pos); // get filename without extension
                if (std::find(lines.begin(), lines.end(), oldFileNameWithoutExtension) != lines.end()) {
                    std::wcout << L"Processing file: " << oldFileNameWithoutExtension << L"\n";
                    std::wstring oldFilePath = oldOskHs + L"\\" + oldFileNameWithoutExtension + newFileExtension; // use old file extension
                    std::wstring backupFilePath = backupDirPath + L"\\" + oldFileNameWithoutExtension + newFileExtension; // use old file extension

                    const std::wstring newPath = newOskHs + L"\\";

                    if (_WDIR *newDir = _wopendir(newPath.c_str()); newDir != nullptr) {
                        _wdirent *d;
                        while ((d = _wreaddir(newDir)) != nullptr) {
                            std::wstring newFileName = d->d_name;
                            std::wstring newFileNameStr = newFileName;
                            size_t pos = newFileNameStr.find_last_of(L'.');
                            if (pos != std::wstring::npos) {
                                std::wstring newFileExtension = newFileNameStr.substr(pos); // get extension from filename
                                std::wstring newFileNameWithoutExtension = newFileNameStr.substr(0, pos); // get filename without extension
                                if (newFileNameWithoutExtension == oldFileNameWithoutExtension) {
                                    std::wstring backupFilePath = backupDirPath + L"\\" + oldFileNameWithoutExtension + oldFileExtension; // use new file extension
                                    std::wstring newFilePath = newOskHs + L"\\" + newFileNameWithoutExtension + newFileExtension; // use new file extension
                                    FileProcessor fileProcessor;
                                    fileProcessor.processFile(oldFilePath, newFilePath, backupFilePath);
                                    convertExtension(oldFilePath, newFileExtension);
                                    std::wcout << L"Converting file extension\n";
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
        std::wcout << L"Failed to open old skin directory: " << oldPath << L"\n";
    }
}
};
int main(int argc, char *argv[]) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    SetConsoleOutputCP(CP_UTF8);
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("o!hsu 0.3.0");
    window.resize(727, 427);
    window.setWindowIcon(QIcon(":/IDI_ICON1"));

    window.setStyleSheet("background-color:grey;");

    // Create a QWidget for the central widget of the main window
    auto *centralWidget = new QWidget(&window);

    // Get the osuDirectory
    const std::wstring osuDirectory = getOsuDirectory();

    QLabel *osuDirectoryLabel = new QLabel(QString::fromStdWString(osuDirectory), centralWidget);

    auto *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(osuDirectoryLabel);

    auto *groupBox = new QGroupBox("osu! directory:", centralWidget);
    groupBox->setLayout(vBoxLayout);

    // Allow the QBox to resize if the window is resized
    groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *layout = new QVBoxLayout;
    layout->addWidget(groupBox);

    // Set the stretch factor of the QGroupBox to make it take up a certain percentage of the height
    // which is intentionally funny :)
    layout->setStretchFactor(groupBox, 0.15727);
    layout->addStretch(1);

    auto *selectSkinComboBox = new QComboBox;
    auto *selectHitsoundsComboBox = new QComboBox;

    // placeholder text because I think they describe it well enough.
    auto *selectSkinLabel = new QLabel("Change hitsounds of this skin");
    auto *selectHitsoundsLabel = new QLabel("Grab hitsounds from this skin");

    // Get a list of all the skins in the skin folder
    const QDir skinDir(QString::fromStdWString(getOsuDirectory()));
    QStringList skins = skinDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Add each skin to the QComboBoxes
    for (const QString &skin : skins) {
        selectSkinComboBox->addItem(skin);
        selectHitsoundsComboBox->addItem(skin);
    }


    layout->addWidget(selectSkinLabel);
    layout->addWidget(selectSkinComboBox);
    layout->addWidget(selectHitsoundsLabel);
    layout->addWidget(selectHitsoundsComboBox);

    // Set the layout to the center
    centralWidget->setLayout(layout);
    window.setCentralWidget(centralWidget);

    osuDirectoryLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    //lots of osu skins have weird unicode characters, support is critical.
    SetConsoleOutputCP(CP_UTF8);

    for (const QString &skin : skins) {
        selectSkinComboBox->addItem(skin);
        selectHitsoundsComboBox->addItem(skin);
    }
    //console logging because this is prone to failure on some utf skins (for now)
    QObject::connect(selectSkinComboBox, QOverload<int>::of(&QComboBox::activated), [=](int){
        const QString selectedSkin = selectSkinComboBox->currentText();
        std::wstring osuDirectory = getOsuDirectory();
        std::wstring selectedSkinPath = osuDirectory + L"\\" + selectedSkin.toStdWString();
        std::wcout << "Selected skin path: " << selectedSkinPath << std::endl;
    });
    QObject::connect(selectHitsoundsComboBox, QOverload<int>::of(&QComboBox::activated), [=](int){
        const QString selectedHitsounds = selectHitsoundsComboBox->currentText();
        std::wstring osuDirectory = getOsuDirectory();
        std::wstring selectedHitsoundsPath = osuDirectory + L"\\" + selectedHitsounds.toStdWString();
        std::wcout << "Selected hitsounds path: " << selectedHitsoundsPath << std::endl;
    });

    auto *confirmButton = new QPushButton("Confirm");

    // Connect the clicked signal of the "Confirm" button to a slot that changes the hitsounds
    // Connect the clicked signal of the "Confirm" button to a slot that changes the hitsounds
    QObject::connect(confirmButton, &QPushButton::clicked, [&](){
        // Change the hitsounds of the selected skin using the hitsounds of the other selected skin
        FileProcessor::changeHitsounds(selectSkinComboBox, selectHitsoundsComboBox);
    });

    // Add the "Confirm" button to the layout
    layout->addWidget(confirmButton);


    window.show();
    std::wcout << "osu! directory: " << osuDirectory << L"\n";

    return QApplication::exec();
}