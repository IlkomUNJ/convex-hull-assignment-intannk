#include "mainwindow.h"
#include "drawingwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    drawingWidget = new DrawingWidget(this);
    mainLayout->addWidget(drawingWidget, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *runSlowBtn = new QPushButton("Run Slow Convex Hull");
    QPushButton *runFastBtn = new QPushButton("Run Fast Convex Hull");
    QPushButton *clearBtn = new QPushButton("Clear");

    buttonLayout->addWidget(runSlowBtn);
    buttonLayout->addWidget(runFastBtn);
    buttonLayout->addWidget(clearBtn);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(central);

    connect(runSlowBtn, &QPushButton::clicked, this, &MainWindow::runSlowConvexHull);
    connect(runFastBtn, &QPushButton::clicked, this, &MainWindow::runFastConvexHull);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearCanvas);

    setWindowTitle("Convex Hull Algorithms Demo");
    resize(700,500);
}

MainWindow::~MainWindow() {}

void MainWindow::runSlowConvexHull() {
    drawingWidget->runSlowConvexHull();
}

void MainWindow::runFastConvexHull() {
    drawingWidget->runFastConvexHull();
}

void MainWindow::clearCanvas() {
    drawingWidget->clear();
}
