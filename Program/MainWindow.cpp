#include "MainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTimer>
#include <QFileDialog>
#include <iostream>
#include <string>
#include "DirLookup.h"
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    selectSkinComboBox = new QComboBox(this);
    selectSkinComboBox = new QComboBox(this);
    selectHitsoundsComboBox = new QComboBox(this);
}

void MainWindow::updateLabel() const {
}

void MainWindow::onSelectSkinButtonClicked() {
    QString selectedSkin = selectSkinComboBox->currentText();
    std::wstring osuDirectory = getOsuDirectory();
    std::filesystem::path selectedSkinPath = std::filesystem::path(osuDirectory) / "Skins" / selectedSkin.toStdWString();
    std::wcout << "Selected skin path: " << selectedSkinPath.wstring() << std::endl;
}

void MainWindow::onSelectHitsoundsButtonClicked() {
    QString selectedHitsounds = selectHitsoundsComboBox->currentText();
    std::wstring osuDirectory = getOsuDirectory();
    std::filesystem::path selectedHitsoundsPath = std::filesystem::path(osuDirectory) / "Skins" / selectedHitsounds.toStdWString();
    std::wcout << "Selected hitsounds path: " << selectedHitsoundsPath.wstring() << std::endl;
}
void MainWindow::onSkinComboBoxIndexChanged(int index)
{
}