//
// Created by Robbe Branswyk on 24/03/2023.
//

#ifndef ENGINE_ZBUFFERING_H
#define ENGINE_ZBUFFERING_H
#include <iostream>
using namespace std;
#include <vector>
#include <sstream>

#include "Light.h"
#include "easy_image.h"
#include "L2DSystemen.h"
#include "3DLineDrawing.h"

//using infinity = numeric_limits<double>::infinity();

class ZBuffer : public vector<vector<double>>{
public:
    ZBuffer(const int width, const int height);
};

void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image,
                    int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    const img::Color &color);

vector<Face> triangulate(const Face& face);
Figures3D triangulateFigure(const Figures3D& theFigure);

void draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &image, Vector3D const &A, Vector3D const &B, Vector3D const &C,
                     double d, double dx, double dy, Color ambientReflection, Color diffuseReflection,
                     Color specularReflection, double reflectionCoeff, Lights3D &lightSources);

img::EasyImage
drawZBuffFigure(Figures3D &theFigure, Lines2D &linesDrawing, const int size, Color backColor, Lights3D &lightSources);

void onLine(const Point2D& p1, const Point2D& p2, double &xI, int yI);
void xLenXR(const Point2D& A, const Point2D& B, const Point2D& C, int &xL, int &xR, int i);

void incrementZValue(double factor, const Vector3D& A, const Vector3D& B, const Vector3D& C, double &dzdx, double &dzdy);
Vector3D normaalVector(const Vector3D& A, const Vector3D& B, const Vector3D& C);


#endif //ENGINE_ZBUFFERING_H
