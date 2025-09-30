#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DrawingWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runSlowConvexHull();
    void runFastConvexHull();
    void clearCanvas();

private:
    DrawingWidget* drawingWidget;
};

#endif // MAINWINDOW_H
