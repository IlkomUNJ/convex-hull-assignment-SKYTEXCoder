#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include "drawingcanvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runSlowConvexHullAlgorithm();
    void runStandardConvexHullAlgorithm();
    void clearDrawingCanvas();

private:
    void setupUI();
    QVector<QPointF> computeSlowConvexHullAlgorithmPoints(QVector<QPointF> passedSetPoints, int &iterationsCount);
    QVector<QPointF> computeStandardConvexHullAlgorithmPoints(QVector<QPointF> passedSetPoints, int &iterationsCount);
    bool isPointToLeftOfLine(const QPointF &p_point, const QPointF &q_point, const QPointF &r_point);
    QVector<QPointF> constructConvexHullPointsVectorFromEdges(const QVector<QPair<QPointF, QPointF>> &edges);
    bool arePointsEqual(const QPointF &p1, const QPointF &p2);
    bool doesThisSetOfPointsMakeARightTurn(const QPointF &point_1, const QPointF &point_2, const QPointF &point_3);

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    DrawingCanvas *drawingCanvas;
    QPushButton *slowConvexHullAlgorithmButton;
    QPushButton *standardConvexHullAlgorithmButton;
    QPushButton *clearDrawingCanvasButton;
};
#endif // MAINWINDOW_H
