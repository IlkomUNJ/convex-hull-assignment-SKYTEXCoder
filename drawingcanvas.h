#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QPaintEvent>
#include <QMouseEvent>
#include <string>

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

    void clearAll();
    void setConvexHull(const QVector<QPointF> &hull);
    void setIterationsCount(int iterationsCount);
    void setTheMostRecentlyUsedConvexHullFindingAlgorithmName(std::string usedAlgorithm);
    QVector<QPointF> getPoints() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawGrid(QPainter &painter);
    void drawPoints(QPainter &painter);
    void drawConvexHull(QPainter &painter);
    void drawIterationsCount(QPainter &painter);
    bool isPointPositionTooClose(const QPointF &newPoint) const;
    void addPointIfPointPositionIsValid(const QPointF &point);

    int iterationsCount;
    std::string theMostRecentlyUsedConvexHullFindingAlgorithmName;
    bool showConvexHull;
    bool isMouseCurrentlyDrawing;
    QVector<QPointF> points;
    QVector<QPointF> convexHullPoints;

    static const int MINIMUM_DISTANCE_BETWEEN_POINTS_IN_PIXELS = 8;
};

#endif // DRAWINGCANVAS_H
