#include "imagecomposer.h"
#include <QSvgGenerator>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "svgtoqpainterpath.h"
ImageComposer::ImageComposer()
{
}

ImageComposer::ImageComposer(const QString& file)
{
    loadScheme(file);
}

bool ImageComposer::loadScheme(const QString &fileName)
{
    m_elements=SvgToQPainterPath::getElements(fileName);
    qDebug()<<"Elements size: "<<m_elements.size();
    qDebug()<<"Fisrt element: "<<m_elements.front();
    return true;
}

void ImageComposer::paintAll(const QPicture &pic, const QString &path)
{
    qreal scaleX=totalWidth()/pic.width();
    qreal scaleY=totalHeight()/pic.height();

    QString subPath=path+"/element";
    for(int i=0; i<m_elements.size(); ++i){
        paintElement(pic, subPath+QString::number(i), i,scaleX, scaleY);
    }
    //paintComposition();
}

void ImageComposer::paintElement(const QPicture &pic, const QString &path, int elementIndice, qreal scaleX, qreal scaleY)
{
    QSvgGenerator generator;
    generator.setFileName(path);
    const auto& painterPath=m_elements.at(elementIndice);
    const qreal width=painterPath.boundingRect().width()*scaleX;
    const qreal height=painterPath.boundingRect().height()*scaleY;
    generator.setSize(QSize(width,height));
    generator.setViewBox(QRect(0, 0, width, height));
    generator.setTitle(QObject::tr("SVG Generator Example Drawing"));
    generator.setDescription(QObject::tr("An SVG drawing created by the SVG Generator "
                                    "Example provided with Qt."));

    QTransform transform;
    transform.scale(scaleX, scaleY);
    QImage image(width, height, QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&generator);
    painter.setClipping(true);

    painter.setClipPath(transform.map(painterPath));
    painter.drawPicture(QPointF(0,0),pic);
    painter.end();

}

void ImageComposer::paintComposition()
{

}
