#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Computer Graphics Course Project: Convex-Shaped Hull Finding Algorithms Simulation Running");
    setMinimumSize(1024, 720);
    setupUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    drawingCanvas = new DrawingCanvas(this);
    drawingCanvas->setMinimumSize(800, 600);
    drawingCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(drawingCanvas);

    buttonLayout = new QHBoxLayout();

    slowConvexHullAlgorithmButton = new QPushButton("Run The Slow Convex Hull Finding Algorithm", this);
    standardConvexHullAlgorithmButton = new QPushButton("Run The Standard Convex Hull Finding Algorithm", this);
    clearDrawingCanvasButton = new QPushButton("Clear The Current Drawing Canvas Of Any Set Points", this);

    slowConvexHullAlgorithmButton->setMinimumHeight(40);
    standardConvexHullAlgorithmButton->setMinimumHeight(40);
    clearDrawingCanvasButton->setMinimumHeight(40);

    buttonLayout->addWidget(slowConvexHullAlgorithmButton);
    buttonLayout->addWidget(standardConvexHullAlgorithmButton);
    buttonLayout->addWidget(clearDrawingCanvasButton);

    mainLayout->addLayout(buttonLayout);

    connect(slowConvexHullAlgorithmButton, &QPushButton::clicked, this, &MainWindow::runSlowConvexHullAlgorithm);
    connect(standardConvexHullAlgorithmButton, &QPushButton::clicked, this, &MainWindow::runStandardConvexHullAlgorithm);
    connect(clearDrawingCanvasButton, &QPushButton::clicked, this, &MainWindow::clearDrawingCanvas);
}

void MainWindow::runSlowConvexHullAlgorithm() {
    QVector<QPointF> points = drawingCanvas->getPoints();

    if (points.size() < 3) {
        QMessageBox::information(this, "Information", "I need, at the very least, 3 set points on the current state of the Drawing Canvas in order to be able to compute the convex hull properly.");
        return;
    }

    // QMessageBox::information(this, "Slow Convex Hull", QString("Running the slow convex hull finding algorithm with %1 points.\n" "Please implement the slow convex hull finding algorithm in this function.").arg(points.size()));

    int iterationsCount = 0;

    QVector<QPointF> hullPoints = computeSlowConvexHullAlgorithmPoints(points, iterationsCount);

    drawingCanvas->setConvexHull(hullPoints);

    // int n = points.size();
    // int iterationsCount = n * (n - 1) * (n - 2);
    drawingCanvas->setIterationsCount(iterationsCount);

    drawingCanvas->setTheMostRecentlyUsedConvexHullFindingAlgorithmName("Slow Convex Hull - Brute-Force Method");

    QMessageBox::information(this, "Slow Convex Hull Finding Algorithm (Brute-Force Method) Simulation Running COMPLETED!", QString("The slow convex hull finding algorithm (which in this case is just the brute-force method of finding the convex hull points) has finally finished running on the current set of points that have been set on the most current state of the Drawing Canvas!\n" "Points processed: %1\n" "Hull vertices: %2\n" "Total iterations: %3").arg(points.size()).arg(hullPoints.size()).arg(iterationsCount));

    // QVector<QPointF> hullPoints = yourSlowConvexHullAlgorithm(points);
    // canvas->setConvexHull(hullPoints);
    // canvas->setIterationCount(iterationCount);
}

QVector<QPointF> MainWindow::computeSlowConvexHullAlgorithmPoints(QVector<QPointF> passedSetPoints, int &iterationsCount) {
    iterationsCount = 0;
    int n = passedSetPoints.size();

    if (n < 3) return QVector<QPointF>();

    QVector<QPair<QPointF, QPointF>> validEdges;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;

            QPointF p_point = passedSetPoints[i];
            QPointF q_point = passedSetPoints[j];

            bool valid = true;

            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;

                QPointF r_point = passedSetPoints[k];

                iterationsCount++;

                if (isPointToLeftOfLine(p_point, q_point, r_point)) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                validEdges.append(qMakePair(p_point, q_point));
            }
        }
    }

    return constructConvexHullPointsVectorFromEdges(validEdges);
}

bool MainWindow::isPointToLeftOfLine(const QPointF &p_point, const QPointF &q_point, const QPointF &r_point) {
    double crossProduct = (q_point.x() - p_point.x()) * (r_point.y() - p_point.y()) - (q_point.y() - p_point.y()) * (r_point.x() - p_point.x());
    return crossProduct > 1e-10;
}

QVector<QPointF> MainWindow::constructConvexHullPointsVectorFromEdges(const QVector<QPair<QPointF, QPointF>> &edges) {
    if (edges.isEmpty()) return QVector<QPointF>();

    QVector<QPointF> hull;
    QPointF currentPoint = edges[0].first;
    hull.append(currentPoint);

    while(true) {
        QPointF nextPoint;
        bool foundNext = false;

        for (const auto &edge: edges) {
            if (arePointsEqual(edge.first, currentPoint)) {
                nextPoint = edge.second;
                foundNext = true;
                break;
            }
        }

        if (!foundNext || arePointsEqual(nextPoint, hull[0])) {
            break;
        }

        hull.append(nextPoint);
        currentPoint = nextPoint;

        if (hull.size() > edges.size()) {
            break;
        }
    }
    return hull;
}

bool MainWindow::arePointsEqual(const QPointF &p1, const QPointF &p2) {
    const double epsilon = 1e-6;
    return qAbs(p1.x() - p2.x()) < epsilon && qAbs(p1.y() - p2.y()) < epsilon;
}

void MainWindow::runStandardConvexHullAlgorithm()
{
    QVector<QPointF> points = drawingCanvas->getPoints();

    if (points.size() < 3) {
        QMessageBox::information(this, "Information", "I need, at the very least, 3 set points on the current state of the Drawing Canvas in order to be able to compute the convex hull properly.");
        return;
    }

    // QMessageBox::information(this, "Convex Hull", QString("Running convex hull algorithm with %1 points.\n" "Please implement the standard convex hull finding algorithm in this function.").arg(points.size()));

    int iterationsCount = 0;

    QVector<QPointF> hullPoints = computeStandardConvexHullAlgorithmPoints(points, iterationsCount);

    // QVector<QPointF> hullPoints = yourConvexHullAlgorithm(points);
    // canvas->setConvexHull(hullPoints);
    // canvas->setIterationCount(iterationCount);

    drawingCanvas->setConvexHull(hullPoints);
    drawingCanvas->setIterationsCount(iterationsCount);
    drawingCanvas->setTheMostRecentlyUsedConvexHullFindingAlgorithmName("Standard Convex Hull - Andrew's Monotone Chain");

    QMessageBox::information(this, "Standard Convex Hull Finding Algorithm (Andrew's Monotone Chain Algorithm) Simulation Running COMPLETED!", QString("The standard convex hull finding algorithm (which in this case just goes by the official name of the Andrew's Monotone Chain Algorithm) has finally finished running on the current set of points that have been set on the most current state of the Drawing Canvas! \n\n\n" "Amount of points processed so far: %1\n\n" "Amount of hull vertices taken so far for the found convex hull: %2\n\n" "Total amount of iterations taken so far: %3").arg(points.size()).arg(hullPoints.size()).arg(iterationsCount));
}

QVector<QPointF> MainWindow::computeStandardConvexHullAlgorithmPoints(QVector<QPointF> passedSetPoints, int &iterationsCount) {
    iterationsCount = 0;
    int n = passedSetPoints.size();

    if (n < 3) return QVector<QPointF>();

    QVector<QPointF> sortedPassedSetPoints = passedSetPoints;

    std::sort(sortedPassedSetPoints.begin(), sortedPassedSetPoints.end(), [](const QPointF &point_a, const QPointF &point_b) {
        if (qAbs(point_a.x() - point_b.x()) < 1e-9) {
            return point_a.y() < point_b.y();
        }
        return point_a.x() < point_b.x();
    });

    QVector<QPointF> upperHullSetPoints;
    upperHullSetPoints.append(sortedPassedSetPoints[0]);
    upperHullSetPoints.append(sortedPassedSetPoints[1]);

    for (int i = 2; i < n; i++) {
        upperHullSetPoints.append(sortedPassedSetPoints[i]);
        while (upperHullSetPoints.size() > 2 && !doesThisSetOfPointsMakeARightTurn(upperHullSetPoints[upperHullSetPoints.size() - 3], upperHullSetPoints[upperHullSetPoints.size() - 2], upperHullSetPoints[upperHullSetPoints.size() - 1])) {
            iterationsCount++;
            upperHullSetPoints.remove(upperHullSetPoints.size() - 2);
        }
        if (upperHullSetPoints.size() > 2) {
            iterationsCount++;
        }
    }

    QVector<QPointF> lowerHullSetPoints;
    lowerHullSetPoints.append(sortedPassedSetPoints[n - 1]);
    lowerHullSetPoints.append(sortedPassedSetPoints[n - 2]);

    for (int i = n - 3; i >= 0; i--) {
        lowerHullSetPoints.append(sortedPassedSetPoints[i]);
        while (lowerHullSetPoints.size() > 2 && !doesThisSetOfPointsMakeARightTurn(lowerHullSetPoints[lowerHullSetPoints.size() - 3], lowerHullSetPoints[lowerHullSetPoints.size() - 2], lowerHullSetPoints[lowerHullSetPoints.size() - 1])) {
            iterationsCount++;
            lowerHullSetPoints.remove(lowerHullSetPoints.size() - 2);
        }

        if (lowerHullSetPoints.size() > 2) {
            iterationsCount++;
        }
    }

    if (lowerHullSetPoints.size() > 0) {
        lowerHullSetPoints.removeFirst();
    }

    if (lowerHullSetPoints.size() > 0) {
        lowerHullSetPoints.removeLast();
    }

    QVector<QPointF> finalHullSetPoints = upperHullSetPoints;
    finalHullSetPoints.append(lowerHullSetPoints);

    return finalHullSetPoints;
}

bool MainWindow::doesThisSetOfPointsMakeARightTurn(const QPointF &point_1, const QPointF &point_2, const QPointF &point_3) {
    double crossProduct = (point_2.x() - point_1.x()) * (point_3.y() - point_1.y()) - (point_2.y() - point_1.y()) * (point_3.x() - point_1.x());
    return crossProduct <= 1e-10;
}

void MainWindow::clearDrawingCanvas()
{
    drawingCanvas->clearAll();
}
