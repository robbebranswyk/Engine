//
// Created by Robbe Branswyk on 14/03/2023.
//

#include <cmath>
#include <list>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "L2DSystemen.h"
#include "intro.h"
#include "vector3d.h"
#include "l_parser.h"
#include "easy_image.h"
#include "3DLineDrawing.h"


//Transformaties
//Schalen
Matrix scaleFigure(const double scale){
    Matrix scaling;
    scaling(1,1) = scale;
    scaling(2,2) = scale;
    scaling(3,3) = scale;
    return scaling;
}

//Rotatie rond X-as
Matrix rotateX(double angle){
    angle *= (M_PI/180);
    Matrix rotate;
    rotate(2,2) = cos(angle);
    rotate(2,3) = sin(angle);
    rotate(3,2) = -sin(angle);
    rotate(3,3) = cos(angle);
    return rotate;
}

//Rotatie rond Y-as
Matrix rotateY(double angle){
    angle *= (M_PI/180);
    Matrix rotate;
    rotate(1,1) = cos(angle);
    rotate(3,1) = sin(angle);
    rotate(1,3) = -sin(angle);
    rotate(3,3) = cos(angle);
    return rotate;
}

//Rotatie rond Z-as
Matrix rotateZ(double angle){
    angle *= (M_PI/180);
    Matrix rotate;
    rotate(1,1) = cos(angle);
    rotate(1,2) = sin(angle);
    rotate(2,1) = -sin(angle);
    rotate(2,2) = cos(angle);
    return rotate;
}

//Translatie
Matrix translate(const Vector3D vector){
    Matrix translate;
    translate(4,1) = vector.x;
    translate(4,2) = vector.y;
    translate(4,3) = vector.z;
    return translate;
}

//Transformatie toepassen op 3D figuur
void applyTransformation(Figure &fig, const Matrix &m){
    for(Vector3D &i : fig.points){
        i = i*m;
    }
}

//Transformatie toepassen op grote 3D Figuur (bestaande uit meerdere figuren)
void applyTransformation(Figures3D &figs, const Matrix &m){
    for (Figure &i : figs){
        applyTransformation(i, m);
    }
}

//Eye-Point transformatie
//Omzetten van carthesische naar poolcoördinaten
void toPolar(const Vector3D &point, double &theta, double &phi, double &r){
    r = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
    theta = atan2(point.y,point.x);
    phi = acos(point.z/r);

    while (theta <= -(M_PI)/2){
        theta += 2*M_PI;
    }
    while (theta >= (M_PI)/2){
        theta -= 2*M_PI;
    }
}

Matrix eyePointTrans(Vector3D &eyepoint){
    //r, phi en theta bepalen
    double r;
    double phi;
    double theta;
    toPolar(eyepoint, theta, phi, r);

    //Transformatiematrix opstellen
    Matrix V;
    V(1,1) = -sin(theta);
    V(1,2) = -(cos(theta) * cos(phi));
    V(1,3) = cos(theta) * sin(phi);
    V(2,1) = cos(theta);
    V(2,2) = -(sin(theta) * cos(phi));
    V(2,3) = sin(theta)* sin(phi);
    V(3,2)  = sin(phi);
    V(3,3) = cos(phi);
    V(4,3) = -r;

    return V;

}

//Perspectiefprojectie
//Van Vector3D Punt naar Point2D
Point2D doProjection(Vector3D &point, const double d){
    double x = -(d*point.x)/point.z;
    double y = -(d*point.y)/point.z;
    Point2D p = Point2D(x,y);
    return p;
}

//Van Figure3D figuur naar Lines2D lijn
Lines2D doProjection(Figures3D &figure){
    Lines2D lineDrawing;
    for (Figure i : figure){
        for (Face j : i.faces){
            if (j.point_indexes.size() == 2){
                Point2D p1 = doProjection(i.points[j.point_indexes[0]], 1);
                Point2D p2 = doProjection(i.points[j.point_indexes[1]], 1);
                lineDrawing.push_back(Line2D(p1, p2, i.color));
            } else {
                for (int k = 0; k < j.point_indexes.size(); k++) {
                    int p1Index = k;
                    int p2Index = k+1;
                    if (k == j.point_indexes.size()-1 ){
                        p2Index = 0;
                    }

                    Point2D p1 = doProjection(i.points[j.point_indexes[p1Index]], 1);
                    Point2D p2 = doProjection(i.points[j.point_indexes[p2Index]], 1);
                    lineDrawing.push_back(Line2D(p1, p2, i.color));
                }
            }

        }
    }
    return lineDrawing;
}

Face::Face(const vector<int> &pointIndexes) : point_indexes(pointIndexes) {}

Face::Face() {}
