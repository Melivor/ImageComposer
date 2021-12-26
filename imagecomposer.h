#ifndef IMAGECOMPOSER_H
#define IMAGECOMPOSER_H
#include <QPainterPath>
#include <QList>
#include <QPicture>
class ImageComposer
{
public:
    ImageComposer();
    ImageComposer(const QString& path);
    bool loadScheme(const QString& fileName);
    int numberOfElements(){return m_elements.size();}
    qreal totalHeight(){return m_maxY-m_minY;}
    qreal totalWidth(){return m_maxX-m_minX;}
    qreal x(){return m_minX;}
    qreal y(){return m_minY;}
    void paintAll(const QPicture& pic, const QString& path);
    void paintElement(const QPicture& pic, const QString& path, int elementIndice, qreal scaleX=1, qreal scaleY=1);
    void paintComposition();
private:
    QList<QPainterPath> m_elements={};
    qreal m_minX=0;
    qreal m_minY=0;
    qreal m_maxX=0;
    qreal m_maxY=0;

};

#endif // IMAGECOMPOSER_H
