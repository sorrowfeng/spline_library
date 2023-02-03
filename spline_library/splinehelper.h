#ifndef SPLINEHELPER_H
#define SPLINEHELPER_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QDebug>
#include <iostream>
#include <bits/shared_ptr.h>
#include "spline.h"
#include "splines/quintic_hermite_spline.h"
#include "splines/cubic_hermite_spline.h"
#include "splines/uniform_cubic_bspline.h"
#include "splines/generic_b_spline.h"
#include "splines/natural_spline.h"

enum SplineType {
    TypeCubicCatmullRomSpline = 0,
    TypeCubicBSpline,
    TypeGenericBSpline,
    TypeCubicNaturalSpline,
    TypeCubicNaturalSplineNotAKnot
};

class SplineHelper : public QObject
{
    Q_OBJECT
public:
    explicit SplineHelper(std::vector<QVector2D> pointList, SplineType type, QObject *parent = nullptr);

    QVector<QPointF> getSplinePath() const;

signals:


private:
    void rebuildSpline(std::vector<QVector2D> pointList, SplineType type);
    std::shared_ptr<Spline<QVector2D>> createSpline(
            const std::vector<QVector2D> &pointList,
            const int splineType,
            bool isLooping,
            float alpha,
            bool includeEndpoints,
            int genericDegree
            );
    void drawSpline(const Spline<QVector2D> &s);
    void drawSplineSegment(const Spline<QVector2D> &s, float beginT, float endT);


    std::shared_ptr<Spline<QVector2D>> mainSpline;
    std::vector<QVector2D> controlPoints;
    QVector<QPointF> splinePath;
};

#endif // SPLINEHELPER_H
