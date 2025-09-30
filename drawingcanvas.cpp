#include "drawingcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <iostream>
#include <cmath>

DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget(parent), iterationsCount(0), showConvexHull(false), isMouseCurrentlyDrawing(false)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(false);
}

void DrawingCanvas::clearAll()
{
    points.clear();
    convexHullPoints.clear();
    iterationsCount = 0;
    showConvexHull = false;
    isMouseCurrentlyDrawing = false;
    setMouseTracking(false);
    update();
}

void DrawingCanvas::setConvexHull(const QVector<QPointF> &hull)
{
    convexHullPoints = hull;
    showConvexHull = true;
    update();
}

void DrawingCanvas::setIterationsCount(int iterationsCount)
{
    DrawingCanvas::iterationsCount = iterationsCount;
    update();
}

void DrawingCanvas::setTheMostRecentlyUsedConvexHullFindingAlgorithmName(std::string usedAlgorithm) {
    DrawingCanvas::theMostRecentlyUsedConvexHullFindingAlgorithmName = usedAlgorithm;
    update();
}

QVector<QPointF> DrawingCanvas::getPoints() const
{
    return points;
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // if you want the main drawing canvas window to draw grids in the form of horizontal lines and vertical lines, you can uncomment
    // the line of C++ code below:
    // drawGrid(painter);

    drawPoints(painter);

    if (showConvexHull) {
        drawConvexHull(painter);
    }

    if (iterationsCount > 0) {
        drawIterationsCount(painter);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isMouseCurrentlyDrawing = true;
        setMouseTracking(true);
        addPointIfPointPositionIsValid(event->position());
    }
}

void DrawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (isMouseCurrentlyDrawing && (event->buttons() & Qt::LeftButton)) {
        addPointIfPointPositionIsValid(event->position());
    }
}

void DrawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMouseCurrentlyDrawing = false;
        setMouseTracking(false);
    }
}

bool DrawingCanvas::isPointPositionTooClose(const QPointF &newPoint) const {
    for (const QPointF &existingPoint : points) {
        qreal dx = newPoint.x() - existingPoint.x();
        qreal dy = newPoint.y() - existingPoint.y();
        qreal distance = qSqrt(pow(dx, 2) + pow(dy, 2));

        if (distance < MINIMUM_DISTANCE_BETWEEN_POINTS_IN_PIXELS) {
            return true;
        }
    }
    return false;
}

void DrawingCanvas::addPointIfPointPositionIsValid(const QPointF &point) {
    if (point.x() < 0 || point.x() >= width() || point.y() < 0 || point.y() >= height()) {
        return;
    }

    if (!isPointPositionTooClose(point)) {
        points.append(point);
        update();
    }
}


void DrawingCanvas::drawGrid(QPainter &painter)
{
    QPen gridPen(QColor(230, 230, 230), 1);
    painter.setPen(gridPen);

    int gridSize = 20;

    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }

    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}

void DrawingCanvas::drawPoints(QPainter &painter)
{
    QPen pointPen(Qt::red, 2);
    QBrush pointBrush(Qt::red);
    painter.setPen(pointPen);
    painter.setBrush(pointBrush);

    for (const QPointF &point : points) {
        painter.drawEllipse(point, 4, 4);
    }
}

void DrawingCanvas::drawConvexHull(QPainter &painter)
{
    if (convexHullPoints.size() < 3) return;

    QPen hullPen(Qt::blue, 3);
    painter.setPen(hullPen);
    painter.setBrush(Qt::NoBrush);

    QPolygonF hullPolygon(convexHullPoints);
    painter.drawPolygon(hullPolygon);

    QPen vertexPen(Qt::darkBlue, 2);
    QBrush vertexBrush(Qt::blue);
    painter.setPen(vertexPen);
    painter.setBrush(vertexBrush);

    for (const QPointF &point : convexHullPoints) {
        painter.drawEllipse(point, 6, 6);
    }
}

void DrawingCanvas::drawIterationsCount(QPainter &painter)
{
    QPen textPen(Qt::white, 1);
    painter.setPen(textPen);

    QFont font("Arial", 12, QFont::Bold);
    painter.setFont(font);

    QString text = QString("Count Of Iterations: %1 Iterations (%2 Algorithm)").arg(iterationsCount).arg(theMostRecentlyUsedConvexHullFindingAlgorithmName);
    painter.drawText(10, 25, text);
}
