/*
 * Simple program to convert an RGB image where white=alpha, and black=colour into 
 * an RGBA image suitable for use by QtC.
 *
 * Compile with: g++ pnga4.cpp -o pnga4 -I/usr/include/ -lQtGui
 */
#include <QtGui/QImage>
#include <stdio.h>

static void qtc_adjust_pix(unsigned char *data, int numChannels, int w, int h, int stride)
{
    int width=w*numChannels,
        offset=0,
        row;

    for(row=0; row<h; ++row)
    {
        int column;

        for(column=0; column<width; column+=numChannels)
        {
            int alpha=(int)data[offset+column+2];

            data[offset+column] = data[offset+column+1] = data[offset+column+2] = 0;
            data[offset+column+3] = 255-alpha;
        }
        offset+=stride;
    }
}

int main(int argc, char *argv[])
{
    if(2==argc)
    {
        QString fname(argv[1]);
        QImage  img(fname);

        img=img.convertToFormat(QImage::Format_ARGB32);

        qtc_adjust_pix(img.bits(), 4, img.width(), img.height(), img.bytesPerLine());

        int pos=fname.lastIndexOf('.');

        img.save(fname.left(pos)+"_a.png", "PNG");
    }
    else
        printf("Usage: %s <img file>\n", argv[0]);

    return 0;
}
