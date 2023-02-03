#include "splinehelper.h"

SplineHelper::SplineHelper(std::vector<QVector2D> pointList, SplineType type, QObject *parent) : QObject(parent)
{
    controlPoints = pointList;
    rebuildSpline(pointList, type);
}


void SplineHelper::rebuildSpline(std::vector<QVector2D> pointList, SplineType type)
{
    if(pointList.size()==0) return;

    int mainSplineType = type;
    bool mainIsLooping = false;
    float mainAlpha = 0;

    bool includeEndpoints = false;
    int genericBsplineDegree = 3;

    mainSpline = createSpline(pointList, mainSplineType, mainIsLooping, mainAlpha, includeEndpoints,genericBsplineDegree);
    //draw the spline itself
    drawSpline(*mainSpline.get());
}



std::shared_ptr<Spline<QVector2D>> SplineHelper::createSpline(
        const std::vector<QVector2D> &pointList,
        const int splineType,
        bool isLooping,
        float alpha,
        bool includeEndpoints,
        int genericDegree
        )
{
    if(pointList.size()<4) return nullptr;
    if(splineType == TypeCubicCatmullRomSpline)
    {
        if(isLooping)
        {
            return std::make_shared<LoopingCubicHermiteSpline<QVector2D>>(pointList, alpha);
        }
        else
        {
            return std::make_shared<CubicHermiteSpline<QVector2D>>(pointList, alpha);
        }
    }
    else if(splineType == TypeCubicBSpline)
    {
        if(isLooping)
        {
            return std::make_shared<LoopingUniformCubicBSpline<QVector2D>>(pointList);
        }
        else
        {
            return std::make_shared<UniformCubicBSpline<QVector2D>>(pointList);
        }
    }
    else if(splineType == TypeGenericBSpline)
    {
        if(isLooping)
        {
            return std::make_shared<LoopingGenericBSpline<QVector2D>>(pointList, genericDegree);
        }
        else
        {
            return std::make_shared<GenericBSpline<QVector2D>>(pointList, genericDegree);
        }
    }
    else if(splineType == TypeCubicNaturalSpline)
    {
        if(isLooping)
        {
            return std::make_shared<LoopingNaturalSpline<QVector2D>>(pointList, alpha);
        }
        else
        {
            return std::make_shared<NaturalSpline<QVector2D>>(pointList, includeEndpoints, alpha, NaturalSpline<QVector2D>::Natural);
        }
    }
    else if(splineType == TypeCubicNaturalSplineNotAKnot)
    {
        if(isLooping)
        {
            return std::make_shared<LoopingNaturalSpline<QVector2D>>(pointList, alpha);
        }
        else
        {
            return std::make_shared<NaturalSpline<QVector2D>>(pointList, includeEndpoints, alpha, NaturalSpline<QVector2D>::NotAKnot);
        }
    }
    else
    {
        if(isLooping)
        {
            return std::make_shared<LoopingQuinticHermiteSpline<QVector2D>>(pointList, alpha);
        }
        else
        {
            return std::make_shared<QuinticHermiteSpline<QVector2D>>(pointList, alpha);
        }
    }
}

void SplineHelper::drawSpline(const Spline<QVector2D> &s)
{
    if(mainSpline==nullptr) return;
    splinePath.append(QPointF(controlPoints.front().x(), controlPoints.front().y()));

    //draw the spline
    float stepSize = 0.25;
    float currentStep = stepSize;
    float limit = s.getMaxT() + 0.01;


    while(currentStep <= limit)
    {
        drawSplineSegment(s, currentStep - stepSize, currentStep);
        currentStep += stepSize;
    }

    splinePath.append(QPointF(controlPoints.back().x(), controlPoints.back().y()));
}


void SplineHelper::drawSplineSegment(const Spline<QVector2D> &s, float beginT, float endT)
{
    auto beginData = s.getPosition(beginT);
    auto endData = s.getPosition(endT);

    float middleT = (beginT + endT) * .5f;

    QVector2D midExpected = (beginData + endData) * .5f;
    auto midActual = s.getPosition((beginT + endT) * .5f);



    //if the difference in T is too small, we're almost certainly at a break point in the spline
    //this obviously isn't supposed to happen, but if there's a bug in the spline code that creates a break, we want to show it
    float minDelta = .001f;
    float maxDistance = .1f;
    if((endT - beginT) > minDelta)
    {
        //if the actual midpoint is too far away from the expected midpoint, subdivide and try again
        if((midExpected - midActual).lengthSquared() > maxDistance)
        {
            drawSplineSegment(s, beginT, middleT);
            drawSplineSegment(s, middleT, endT);
        }
        else
        {
            splinePath.append(QPointF(beginData.x(),beginData.y()));

//            painter.drawLine(
//                QPointF(beginData.x(),beginData.y()),
//                QPointF(endData.x(),endData.y())
//                );
        }
    }
}

QVector<QPointF> SplineHelper::getSplinePath() const
{
    return splinePath;
}
