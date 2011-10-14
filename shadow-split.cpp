/*
 * Simple program to split a show PNG file into the 8 outer parts required for QtC.
 *
 * Compile with: g++ shadow-split.cpp -o shadow-split -I/usr/include/ -lQtGui
 */

#include <QtGui/QImage>
#include <QtGui/QPainter>

static void fill(QImage &dest, const QImage &src)
{
    dest.fill(Qt::transparent);
    QPainter p(&dest);
    
    if(dest.width()==src.width()) {
        for(int i=0; i<dest.height(); ++i) {
            p.drawImage(QPoint(0, i), src);
        }
    } else {
        for(int i=0; i<dest.width(); ++i) {
            p.drawImage(QPoint(i, 0), src);
        }
    }
    p.end();
}

int main(int argc, char **argv)
{
    QImage shadowImageSrc(argv[1]);
    
    QImage   shadowImage( shadowImageSrc.width(), shadowImageSrc.height(), QImage::Format_ARGB32 );
    QPainter painter( &shadowImage );
    //painter.setCompositionMode( QPainter::CompositionMode_Multiply );
    painter.setPen( Qt::NoPen );
    painter.setBrush( QColor(57, 56, 53) );
    painter.drawRect( 0, 0, shadowImage.width(), shadowImage.height() );
    painter.end();
    shadowImage.setAlphaChannel( shadowImageSrc.alphaChannel() ); // Cheat, just use the alpha mask

    {
    QPainter painter( &shadowImage );

    // add transparency
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect( shadowImage.rect(), QColor( 0, 0, 0, 150 ) );
    painter.end();
    }

    int _size=shadowImage.width()/2;

    QImage tmp(_size, _size, QImage::Format_ARGB32);
    fill(tmp, shadowImage.copy(_size-1, 0, 1, _size));
    tmp.save("shadow0.png");
    shadowImage.copy(_size, 0, _size, _size).save("shadow1.png");
    tmp=QImage(_size, _size, QImage::Format_ARGB32);
    fill(tmp, shadowImage.copy(_size-1, _size, _size, 1));
    tmp.save("shadow2.png");
    shadowImage.copy(_size-1, _size, _size, _size).save("shadow3.png");
    tmp=QImage(_size, _size, QImage::Format_ARGB32);
    fill(tmp, shadowImage.copy(_size-1, _size, 1, _size));
    tmp.save("shadow4.png");
    shadowImage.copy(0, _size, _size, _size).save("shadow5.png");
    tmp=QImage(_size, _size, QImage::Format_ARGB32);
    fill(tmp, shadowImage.copy(0, _size, _size, 1));
    tmp.save("shadow6.png");
    shadowImage.copy(0, 0, _size, _size).save("shadow7.png");
    return 0;
}
