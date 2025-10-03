#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

enum HullType {
    None,
    Slow,
    Fast
};

class DrawingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingWidget(QWidget *parent = nullptr);

    void runSlowConvexHull();
    void runFastConvexHull();
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVector<QPointF> points;
    QVector<QPointF> hull;
    HullType hullType = None;
    int iterationCount = 0;

    static double cross(const QPointF& O, const QPointF& A, const QPointF& B);
    QVector<QPointF> convexHull(QVector<QPointF> pts, int& iter);
    QVector<QPointF> slowConvexHull(QVector<QPointF> pts, int& iter);

    friend class MainWindow;
};

#endif // DRAWINGWIDGET_H
