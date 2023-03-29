//
// Created by Robbe Branswyk on 13/03/2023.
//

#ifndef ENGINE_L2DSYSTEMEN_H
#define ENGINE_L2DSYSTEMEN_H

#include <cmath>
#include <list>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;

#include "intro.h"
#include "easy_image.h"

//////2D L-Systemen////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Color{
public:
    double red;
    double green;
    double blue;

    Color(double red, double green, double blue) : red(red), green(green), blue(blue) {}

    Color() {}

    img::Color toColor(){
        return img::Color(red*255, green*255, blue*255);
    }

};

class Point2D{
public:
    double x;
    double y;

    Point2D(double x, double y) : x(x), y(y) {}
};

class Line2D{
public:
    Point2D p1;
    Point2D p2;
    Color color;

    //Voor Z-buffering
    double z1;
    double z2;

    Line2D(const Point2D &p1, const Point2D &p2, const Color &color) : p1(p1), p2(p2), color(color) {}
};

using Lines2D = list<Line2D>;

img::EasyImage draw2DLines(Lines2D &lines, const int size, Color backColor, bool zbuf = false);

Lines2D drawLSystem(LParser::LSystem2D &l_system, Color lineColor);
#endif //ENGINE_L2DSYSTEMEN_H
