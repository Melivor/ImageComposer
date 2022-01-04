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
#include <QImageWriter>
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
    return true;
}

void ImageComposer::paintAll(const QImage &image, const QString &path)
{


    QString subPath=path+"/element";
    if(m_elements.empty()){
        return;
    }
    m_minX=m_elements[0].boundingRect().left();
    m_maxX=m_elements[0].boundingRect().right();
    m_minY=m_elements[0].boundingRect().top();
    m_maxY=m_elements[0].boundingRect().bottom();
    for(int i=1; i<m_elements.size(); ++i){
        m_minX=std::min(m_elements[i].boundingRect().left(), m_minX);
        m_maxX=std::max(m_elements[i].boundingRect().right(), m_maxX);
        m_minY=std::min(m_elements[i].boundingRect().top(), m_minY);
        m_maxY=std::max(m_elements[i].boundingRect().bottom(), m_maxY);
    }
    qDebug()<<__PRETTY_FUNCTION__<<" minX: "<<m_minX;
    qDebug()<<__PRETTY_FUNCTION__<<" maxX: "<<m_maxX;
    qreal scaleX=image.width()/totalWidth();
    qreal scaleY=image.height()/totalHeight();
    for(int i=0; i<m_elements.size(); ++i){
        paintElement(image, subPath+QString::number(i)+".png", i,scaleX, scaleY);
    }

    paintComposition(image,path+"/composition.png",scaleX, scaleY);
}

void ImageComposer::paintElement(const QImage &image, const QString &path, int elementIndice, qreal scaleX, qreal scaleY)
{
    qDebug()<<__PRETTY_FUNCTION__<<": "<<path;
    const auto& painterPath=m_elements.at(elementIndice);
    const qreal width=painterPath.boundingRect().width()*scaleX;
    const qreal height=painterPath.boundingRect().height()*scaleY;
    QImage imageOutput(width, height, QImage::Format_ARGB32);
    imageOutput.fill("white");
    QPainter painter;
    painter.begin(&imageOutput);
    QTransform transform;
    transform.translate(-painterPath.boundingRect().x(),-painterPath.boundingRect().y());
    auto transformedPath=transform.map(painterPath);
    //qDebug()<<__PRETTY_FUNCTION__<<" t-1 path: "<<transformedPath;
    transform.reset();
    transform.scale(scaleX, scaleY);
    transformedPath=transform.map(transformedPath);
    painter.setPen("blue");
    painter.setBrush(QBrush("green"));

    painter.drawPath(transformedPath);

    painter.setClipPath(transformedPath);
    painter.setClipping(true);
    painter.setBrush(QBrush("red"));
    qDebug()<<__PRETTY_FUNCTION__<<" pic size: "<<image.width()<<image.height();
    qDebug()<<__PRETTY_FUNCTION__<<"y offset: "<<-(painterPath.boundingRect().y()-y())*scaleY;
    QRectF target(0, 0,  (painterPath.boundingRect().width())*scaleX , (painterPath.boundingRect().height())*scaleY);
    QRectF source((painterPath.boundingRect().x()-x())*scaleX, (painterPath.boundingRect().y()-y())*scaleY, (painterPath.boundingRect().width())*scaleX, (painterPath.boundingRect().height())*scaleY);
    painter.drawImage(target,image, source);
    painter.end();
    QImageWriter writer(path);
    if(!writer.write(imageOutput)){
        qWarning()<<writer.errorString();
    }
}

void ImageComposer::paintComposition(const QImage& imageSource, const QString &path, qreal scaleX, qreal scaleY)
{


    qreal horizontalMargin=imageSource.width()*0.05;
    qreal verticalMargin=imageSource.height()*0.05;
    QImage imageOutput(imageSource.width()+horizontalMargin*2, imageSource.height()+verticalMargin*2, QImage::Format_ARGB32);
    imageOutput.fill("white");
    QPainter painter;
    painter.begin(&imageOutput);
    QPen pen(QColor(50,50,50));
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i=0; i<m_elements.size(); ++i){
        const auto& painterPath=m_elements.at(i);
        QTransform transform;
        transform.translate(-painterPath.boundingRect().x(),-painterPath.boundingRect().y());
        auto transformedPath=transform.map(painterPath);
        transform.reset();
        transform.scale(scaleX, scaleY);
        transformedPath=transform.map(transformedPath);
        transform.reset();
        transform.translate((painterPath.boundingRect().x()-x())*scaleX,(painterPath.boundingRect().y()-y())*scaleY);
        transformedPath=transform.map(transformedPath);
        QRectF source=transformedPath.boundingRect();

        transform.reset();
        transform.translate(horizontalMargin,verticalMargin);
        transformedPath=transform.map(transformedPath);
        painter.drawPath(transformedPath);
        QRectF target=transformedPath.boundingRect();
        painter.setClipPath(transformedPath);
        painter.setClipping(true);
        painter.drawImage(target,imageSource, source);
        painter.setClipping(false);
        //QRectF source((painterPath.boundingRect().x()-x())*scaleX, (painterPath.boundingRect().y()-y())*scaleY, (painterPath.boundingRect().width())*scaleX, (painterPath.boundingRect().height())*scaleY);


        /*
        QRectF target(0, 0,  (painterPath.boundingRect().width())*scaleX , (painterPath.boundingRect().height())*scaleY);
        QRectF source((painterPath.boundingRect().x()-x())*scaleX, (painterPath.boundingRect().y()-y())*scaleY, (painterPath.boundingRect().width())*scaleX, (painterPath.boundingRect().height())*scaleY);

        painter.end();
        */

    }

    QImageWriter writer(path);
    if(!writer.write(imageOutput)){
        qWarning()<<writer.errorString();
    }
}
