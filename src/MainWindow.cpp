#include "../include/MainWindow.h"
#include <QPainter>
#include <QTimer>
#include <QStyle>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isAccelerating(false), progress(0)
{
    setupUi();
    setupStyle();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    resize(900, 600);
    setWindowTitle(tr("Game Accelerator"));

    // Enable frameless window (if desired)
    // setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // Transparent background for custom styling (if needed)
    // setAttribute(Qt::WA_TranslucentBackground);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar
    QWidget *sidebar = new QWidget(centralWidget);
    sidebar->setFixedWidth(200);
    sidebar->setObjectName("Sidebar");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(10, 20, 10, 20);

    navigationList = new QListWidget(sidebar);
    navigationList->setObjectName("NavigationList");
    navigationList->addItem(tr("Home"));
    navigationList->addItem(tr("Game Library"));
    navigationList->addItem(tr("Settings"));
    navigationList->setCurrentRow(0);

    sidebarLayout->addWidget(navigationList);
    sidebarLayout->addStretch();

    // Main Content Area
    stackedWidget = new QStackedWidget(centralWidget);
    stackedWidget->setObjectName("MainContentArea");

    // Home View (Acceleration Dashboard)
    homeView = new QWidget();
    QVBoxLayout *homeLayout = new QVBoxLayout(homeView);
    homeLayout->setAlignment(Qt::AlignCenter);

    gameLabel = new QLabel(tr("Selected Game: Valorant"), homeView);
    gameLabel->setObjectName("GameLabel");
    gameLabel->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel(tr("Status: Disconnected"), homeView);
    statusLabel->setObjectName("StatusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);

    accelerateButton = new QPushButton(tr("Accelerate (一键加速)"), homeView);
    accelerateButton->setObjectName("AccelerateButton");
    accelerateButton->setFixedSize(200, 60);

    homeLayout->addStretch();
    homeLayout->addWidget(gameLabel);
    homeLayout->addSpacing(20);
    homeLayout->addWidget(statusLabel);
    homeLayout->addSpacing(40);
    homeLayout->addWidget(accelerateButton, 0, Qt::AlignHCenter);
    homeLayout->addStretch();

    // Library View (Mock)
    libraryView = new QWidget();
    QVBoxLayout *libraryLayout = new QVBoxLayout(libraryView);

    QLabel *libraryTitle = new QLabel(tr("My Games"), libraryView);
    libraryTitle->setObjectName("ViewTitle");

    QListWidget *gameList = new QListWidget(libraryView);
    gameList->setObjectName("GameList");
    gameList->addItem(tr("Valorant"));
    gameList->addItem(tr("League of Legends"));
    gameList->addItem(tr("Apex Legends"));
    gameList->addItem(tr("CS:GO"));
    gameList->addItem(tr("PUBG"));

    libraryLayout->addWidget(libraryTitle);
    libraryLayout->addWidget(gameList);

    // Settings View (Mock)
    settingsView = new QWidget();
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsView);

    QLabel *settingsTitle = new QLabel(tr("Settings"), settingsView);
    settingsTitle->setObjectName("ViewTitle");

    QLabel *setting1 = new QLabel(tr("Network Mode: Auto"), settingsView);
    QLabel *setting2 = new QLabel(tr("Start on Boot: Enabled"), settingsView);
    QLabel *setting3 = new QLabel(tr("Theme: Dark"), settingsView);

    setting1->setObjectName("SettingItem");
    setting2->setObjectName("SettingItem");
    setting3->setObjectName("SettingItem");

    settingsLayout->addWidget(settingsTitle);
    settingsLayout->addSpacing(20);
    settingsLayout->addWidget(setting1);
    settingsLayout->addWidget(setting2);
    settingsLayout->addWidget(setting3);
    settingsLayout->addStretch();

    stackedWidget->addWidget(homeView);
    stackedWidget->addWidget(libraryView);
    stackedWidget->addWidget(settingsView);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);

    // Connections
    connect(navigationList, &QListWidget::currentRowChanged, this, &MainWindow::onNavigationChanged);
    connect(accelerateButton, &QPushButton::clicked, this, &MainWindow::onAccelerateButtonClicked);

    // Timer setup for mock acceleration
    accelTimer = new QTimer(this);
    connect(accelTimer, &QTimer::timeout, this, &MainWindow::updateAccelerationProgress);
}

void MainWindow::setupStyle()
{
    // High-performance styling: use QSS efficiently
    QString qss = R"(
        QMainWindow {
            background-color: #1E1E2E; /* Dark theme background */
            color: #FFFFFF;
        }

        /* Sidebar Styling */
        #Sidebar {
            background-color: #252538;
            border-right: 1px solid #3A3A52;
        }

        /* Navigation List Styling */
        #NavigationList {
            background-color: transparent;
            border: none;
            outline: none;
            color: #A0A0B5;
            font-size: 16px;
        }

        #NavigationList::item {
            padding: 10px 15px;
            margin: 5px 0px;
            border-radius: 8px;
        }

        #NavigationList::item:selected {
            background-color: #3A3A52;
            color: #FFFFFF;
        }

        #NavigationList::item:hover {
            background-color: #2E2E42;
        }

        /* Main Content Area */
        #MainContentArea {
            background-color: #1E1E2E;
        }

        /* Labels */
        #GameLabel {
            font-size: 24px;
            font-weight: bold;
            color: #E0E0E0;
        }

        #StatusLabel {
            font-size: 16px;
            color: #A0A0B5;
        }

        /* Accelerate Button Styling (Key element like Xunyou) */
        #AccelerateButton {
            background-color: #4CAF50; /* Green color for acceleration */
            color: white;
            border: none;
            border-radius: 30px; /* Highly rounded corners */
            font-size: 20px;
            font-weight: bold;
            box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.5); /* Fake shadow via QSS might be tricky, use GraphicsDropShadowEffect if needed */
        }

        #AccelerateButton:hover {
            background-color: #45a049;
        }

        #AccelerateButton:pressed {
            background-color: #3e8e41;
        }

        #AccelerateButton[accelerating="true"] {
            background-color: #FFA500; /* Orange for 'In Progress' */
        }

        #AccelerateButton[accelerated="true"] {
            background-color: #2196F3; /* Blue for 'Accelerated' */
        }

        /* Library and Settings Views Styling */
        #ViewTitle {
            font-size: 28px;
            font-weight: bold;
            color: #FFFFFF;
            padding: 10px;
        }

        #GameList {
            background-color: #252538;
            border: 1px solid #3A3A52;
            border-radius: 8px;
            outline: none;
            color: #E0E0E0;
            font-size: 18px;
            padding: 10px;
        }

        #GameList::item {
            padding: 15px;
            border-bottom: 1px solid #3A3A52;
        }

        #GameList::item:hover {
            background-color: #2E2E42;
        }

        #GameList::item:selected {
            background-color: #3A3A52;
            color: #FFFFFF;
        }

        #SettingItem {
            font-size: 18px;
            color: #A0A0B5;
            padding: 10px;
            background-color: #252538;
            border-radius: 5px;
            margin-bottom: 5px;
        }
    )";

    setStyleSheet(qss);

    // Add shadow to the accelerate button for better appearance
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    shadowEffect->setOffset(0, 4);
    accelerateButton->setGraphicsEffect(shadowEffect);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    // Custom painting if needed for high-performance visual effects (e.g., dynamic background)
    // Avoid overdrawing here to maintain high performance.
}

void MainWindow::onNavigationChanged(int index)
{
    stackedWidget->setCurrentIndex(index);
}

void MainWindow::onAccelerateButtonClicked()
{
    if (!isAccelerating && progress == 0) {
        // Start acceleration
        isAccelerating = true;
        progress = 0;

        // Update styling
        accelerateButton->setProperty("accelerating", true);
        accelerateButton->setProperty("accelerated", false);
        accelerateButton->style()->unpolish(accelerateButton);
        accelerateButton->style()->polish(accelerateButton);

        accelerateButton->setText(tr("Accelerating... 0%"));
        statusLabel->setText(tr("Status: Connecting to optimal node..."));

        // Start high-performance non-blocking mock update
        accelTimer->start(50); // Fast updates
    } else if (!isAccelerating && progress >= 100) {
        // Stop acceleration
        progress = 0;

        // Update styling
        accelerateButton->setProperty("accelerating", false);
        accelerateButton->setProperty("accelerated", false);
        accelerateButton->style()->unpolish(accelerateButton);
        accelerateButton->style()->polish(accelerateButton);

        accelerateButton->setText(tr("Accelerate (一键加速)"));
        statusLabel->setText(tr("Status: Disconnected"));
    }
}

void MainWindow::updateAccelerationProgress()
{
    if (isAccelerating) {
        progress += 2;

        if (progress <= 100) {
            accelerateButton->setText(QString(tr("Accelerating... %1%")).arg(progress));
        }

        if (progress >= 100) {
            isAccelerating = false;
            accelTimer->stop();

            // Finished acceleration
            accelerateButton->setProperty("accelerating", false);
            accelerateButton->setProperty("accelerated", true);
            accelerateButton->style()->unpolish(accelerateButton);
            accelerateButton->style()->polish(accelerateButton);

            accelerateButton->setText(tr("Stop Acceleration (停止加速)"));

            // Mock connection stats
            statusLabel->setText(tr("Status: Connected | Ping: 12ms | Loss: 0%"));
        }
    }
}
