//
// Created by Robbe Branswyk on 14/03/2023.
//

#ifndef ENGINE_3DLINEDRAWING_H
#define ENGINE_3DLINEDRAWING_H

#include "L2DSystemen.h"

//////3D Lijntekeningen////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Face{
public:
    vector<int> point_indexes;

    Face(const vector<int> &pointIndexes);

    Face();
};

class Figure{
public:
    Figure(const Color &color) : color(color) {}

    Figure() {}

    vector<Vector3D> points;
    vector<Face> faces;
    Color color;
};

typedef list<Figure> Figures3D;

//Transformaties
Matrix scaleFigure(const double scale);

Matrix rotateX(double angle);

Matrix rotateY(double angle);

Matrix rotateZ(double angle);

Matrix translate(const Vector3D& vector);

void applyTransformation(Figure &fig, const Matrix &m);

void applyTransformation(Figures3D &figs, const Matrix &m);

//EyePoint
void toPolar(const Vector3D &point, double &theta, double &phi, double &r);

Matrix eyePointTrans(Vector3D &eyepoint);

//Projectie
Point2D doProjection(Vector3D &point, const double d);

Lines2D doProjection(Figures3D &figure);


#endif //ENGINE_3DLINEDRAWING_H
