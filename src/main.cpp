#include "../include/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Enable high DPI scaling for better appearance on modern monitors
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);

    // Set application info
    QApplication::setApplicationName("GameAccelerator");
    QApplication::setOrganizationName("MyCompany");

    MainWindow w;
    w.show();

    return app.exec();
}
