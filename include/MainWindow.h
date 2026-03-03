#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onNavigationChanged(int index);
    void onAccelerateButtonClicked();
    void updateAccelerationProgress();

private:
    void setupUi();
    void setupStyle();

    QWidget *centralWidget;
    QListWidget *navigationList;
    QStackedWidget *stackedWidget;

    // Home View (Acceleration Dashboard)
    QWidget *homeView;
    QLabel *gameLabel;
    QPushButton *accelerateButton;
    QLabel *statusLabel;

    // Other Views
    QWidget *libraryView;
    QWidget *settingsView;

    // State
    bool isAccelerating;
    int progress;
    QTimer *accelTimer;
};

#endif // MAINWINDOW_H
