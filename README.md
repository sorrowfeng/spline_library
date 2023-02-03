# spline_library

用于解析样条曲线，通过输入控制点，输出样条的QPainterPath

# 示例
```cpp
#include "splinehelper.h"

int main()
{
    std::vector<QVector2D> pointList; // 输入的控制点
    SplineHelper spline(pointList, TypeCubicBSpline);
    QPainterPath path = spline.getSplinePath();

    return 0;
}
```