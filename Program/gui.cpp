/*#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

int gui(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("ohsu");

    // Create a central widget to hold the layout and widgets
    auto *centralWidget = new QWidget(&window);

    // Create a vertical layout
    auto *layout = new QVBoxLayout(centralWidget);

    // Create a button and add it to the layout
    auto *button = new QPushButton("Change Hitsound", centralWidget);
    layout->addWidget(button);

    // Create a label and add it to the layout
    auto *label = new QLabel("o!hsu", centralWidget);
    layout->addWidget(label);

    // Set the layout of the central widget
    centralWidget->setLayout(layout);

    // Set the central widget of the window
    window.setCentralWidget(centralWidget);

    window.show();

    return QApplication::exec();
}*/