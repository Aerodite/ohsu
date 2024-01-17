#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLabel *osuDirectoryLabel{};
    QComboBox *selectSkinComboBox{};  // New member variable
    QComboBox *selectHitsoundsComboBox{};  // New member variable

public:
    explicit MainWindow(QWidget *parent = nullptr);

    public slots:
        void updateLabel() const;
    void onSelectSkinButtonClicked();
    void onSelectHitsoundsButtonClicked();
    void onSkinComboBoxIndexChanged(int index);
};

#endif // MAINWINDOW_H