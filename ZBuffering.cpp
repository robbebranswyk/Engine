//
// Created by Robbe Branswyk on 24/03/2023.
//

#include "ZBuffering.h"

ZBuffer::ZBuffer(const int width, const int height) {
    vector<double> heightValues(height, numeric_limits<double>::infinity());
    for (int i = 0; i < width; i++) {
        this->push_back(heightValues);
    }
}

void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, unsigned int x0, unsigned int y0, const double z0,unsigned int x1, unsigned int y1, const double z1, const img::Color &color) {

    //1/Z bepalen
    //Lengte vector p1p2 bepalen == aantal pixels van lijn
    double lengthP1P2 = sqrt( pow((x1-x0),2) + pow((y1-y0), 2));

    //Loopen over al de pixels en waarde p bepalen en ook de 1/z
    double a = lengthP1P2 -1;

    //Code van draw_line
    if (x0 >= image.get_width() || y0 >= image.get_height() || x1 >= image.get_width() || y1 > image.get_height()) {
        std::stringstream ss;
        ss << "Drawing line from (" << x0 << "," << y0 << ") to (" << x1 << "," << y1 << ") in image of width "
           << image.get_width() << " and height " << image.get_height();
        throw std::runtime_error(ss.str());
    }
    if (x0 == x1)
    {
        //special case for x0 == x1
        for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            double valueP = (a-i)/a;
            double zValue = valueP/z0 + (1-valueP)/z1;
            if (zValue < zbuffer[x0][i]){
                zbuffer[x0][i] = zValue;
                (image)(x0, i) = color;
            }

        }
    }
    else if (y0 == y1)
    {
        //special case for y0 == y1
        for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
        {
            double valueP = (a-i)/a;
            double zValue = valueP/z0 + (1-valueP)/z1;
            if (zValue < zbuffer[i][y0]){
                zbuffer[i][y0] = zValue;
                (image)(i, y0) = color;
            }

        }
    }
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            for (unsigned int i = 0; i <= (x1 - x0); i++)
            {
                double valueP = (a-i)/a;
                double zValue = valueP/z0 + (1-valueP)/z1;
                if (zValue < zbuffer[x0+i][(unsigned int) round(y0 + m * i)]){
                    zbuffer[x0+i][(unsigned int) round(y0 + m * i)] = zValue;
                    (image)(x0 + i, (unsigned int) round(y0 + m * i)) = color;
                }

            }
        }
        else if (m > 1.0)
        {
            for (unsigned int i = 0; i <= (y1 - y0); i++)
            {
                double valueP = (a-i)/a;
                double zValue = valueP/z0 + (1-valueP)/z1;
                if (zValue < zbuffer[(unsigned int) round(x0 + (i / m))][y0 + i]){
                    zbuffer[(unsigned int) round(x0 + (i / m))][y0 + i] = zValue;
                    (image)((unsigned int) round(x0 + (i / m)), y0 + i) = color;
                }
            }
        }
        else if (m < -1.0)
        {
            for (unsigned int i = 0; i <= (y0 - y1); i++)
            {
                double valueP = (a-i)/a;
                double zValue = valueP/z0 + (1-valueP)/z1;
                if (zValue < zbuffer[(unsigned int) round(x0 - (i / m))][y0 - i]){
                    zbuffer[(unsigned int) round(x0 - (i / m))][y0 - i] = zValue;
                    (image)((unsigned int) round(x0 - (i / m)), y0 - i) = color;
                }

            }
        }
    }
}
