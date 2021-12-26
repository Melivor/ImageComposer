#ifndef SVGTOQPAINTERPATH_H
#define SVGTOQPAINTERPATH_H
#include <QPainterPath>


namespace SvgToQPainterPath
{

    QList<QPainterPath> getElements(const QString filename);
    bool parsePathDataFast(const QString &dataStr, QPainterPath &path);
    void pathArc(QPainterPath &path,
                        qreal               rx,
                        qreal               ry,
                        qreal               x_axis_rotation,
                        int         large_arc_flag,
                        int         sweep_flag,
                        qreal               x,
                        qreal               y,
                        qreal curx, qreal cury);
};

#endif // SVGTOQPAINTERPATH_H
