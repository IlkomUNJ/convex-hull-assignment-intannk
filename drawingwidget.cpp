#include "drawingwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFont>
#include <algorithm>
#include <set>

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(350);
    setAutoFillBackground(true);
    setPalette(Qt::white);
}

void DrawingWidget::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        points.append(event->pos());
        hull.clear();
        hullType = None;
        iterationCount = 0;
        update();
    }
}

void DrawingWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Draw points
    p.setPen(Qt::black);
    p.setBrush(Qt::red);
    for(const auto& pt : points)
        p.drawEllipse(pt, 4, 4);

    // Draw hull
    if(hull.size() > 1) {
        p.setPen(QPen(hullType==Slow ? Qt::darkGreen : Qt::blue, 2));
        for(int i=0; i<hull.size(); ++i)
            p.drawLine(hull[i], hull[(i+1)%hull.size()]);
    }

    // Draw iteration count
    if(hullType != None) {
        p.setPen(Qt::darkMagenta);
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.drawText(10, height()-10, QString("Iterations: %1").arg(iterationCount));
    }
}

void DrawingWidget::clear() {
    points.clear();
    hull.clear();
    hullType = None;
    iterationCount = 0;
    update();
}

void DrawingWidget::runFastConvexHull() {
    if(points.size() < 3) return;
    hull = convexHull(points, iterationCount);
    hullType = Fast;
    update();
}

void DrawingWidget::runSlowConvexHull() {
    if(points.size() < 3) return;
    hull = slowConvexHull(points, iterationCount);
    hullType = Slow;
    update();
}

// Helper: cross product for orientation
double DrawingWidget::cross(const QPointF& O, const QPointF& A, const QPointF& B) {
    return (A.x()-O.x())*(B.y()-O.y()) - (A.y()-O.y())*(B.x()-O.x());
}

// Fast: Monotone chain
QVector<QPointF> DrawingWidget::convexHull(QVector<QPointF> pts, int& iter) {
    iter = 0;
    std::sort(pts.begin(), pts.end(), [](const QPointF& a, const QPointF& b) {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    });
    QVector<QPointF> Lupper, Llower;
    int n = pts.size();
    if(n < 3) return pts;

    // Upper
    Lupper.append(pts[0]);
    Lupper.append(pts[1]);
    for(int i=2; i<n; ++i) {
        Lupper.append(pts[i]);
        iter++;
        while(Lupper.size() > 2 &&
               cross(Lupper[Lupper.size()-3], Lupper[Lupper.size()-2], Lupper[Lupper.size()-1]) <= 0) {
            Lupper.remove(Lupper.size()-2);
            iter++;
        }
    }
    // Lower
    Llower.append(pts[n-1]);
    Llower.append(pts[n-2]);
    for(int i=n-3; i>=0; --i) {
        Llower.append(pts[i]);
        iter++;
        while(Llower.size() > 2 &&
               cross(Llower[Llower.size()-3], Llower[Llower.size()-2], Llower[Llower.size()-1]) <= 0) {
            Llower.remove(Llower.size()-2);
            iter++;
        }
    }
    Llower.removeFirst();
    Llower.removeLast();
    QVector<QPointF> hull = Lupper;
    hull += Llower;
    return hull;
}

// Slow: Brute force, count iterations
QVector<QPointF> DrawingWidget::slowConvexHull(QVector<QPointF> pts, int& iter) {
    iter = 0;
    int n = pts.size();
    std::set<std::pair<int,int>> E;
    for(int i=0; i<n; ++i) {
        for(int j=0; j<n; ++j) {
            if(i==j) continue;
            bool valid = true;
            for(int k=0; k<n; ++k) {
                if(k==i || k==j) continue;
                iter++;
                double cp = cross(pts[i], pts[j], pts[k]);
                if(cp < 0) { // k is left of directed line i->j
                    valid = false;
                    break;
                }
            }
            if(valid) E.insert({i,j});
        }
    }
    // Reconstruct hull in order (clockwise)
    QVector<QPointF> hull;
    // Find starting point: rightmost lowest
    int start = 0;
    for(int i=1; i<n; ++i) {
        if(pts[i].x() > pts[start].x() || (pts[i].x() == pts[start].x() && pts[i].y() < pts[start].y()))
            start = i;
    }
    int curr = start;
    hull.append(pts[curr]);
    while(true) {
        int next = -1;
        for(int i=0; i<n; ++i) {
            if(curr!=i && E.count({curr,i})) {
                next = i;
                break;
            }
        }
        if(next==-1 || (next==start)) break;
        hull.append(pts[next]);
        curr = next;
    }
    return hull;
}
