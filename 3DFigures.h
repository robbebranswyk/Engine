//
// Created by Robbe Branswyk on 15/03/2023.
//

#ifndef ENGINE_3DFIGURES_H
#define ENGINE_3DFIGURES_H

#include "3DLineDrawing.h"

Figure createCube();
Figure createTetrahedron();
Figure createOctahedron();
Figure createIcosahedron();
Figure createDodecadron();

Vector3D getMidden(const vector<Vector3D>& punten);

Figure roundingIco(const Figure& Ico);
Figure createSphere(const int n);

Figure createCone(const int n, const double h);

Figure createCylinder(const int n, const double h);

Figure createTorus(const double r, const double R, const int n, const int m);
Vector3D parameterVgl(double u, double v, const double r, const double R);

//3D L-systemen
Figure draw3DLsystem(LParser::LSystem3D &l_system);

//EXTRA
Figure createMobius(const int n, const int m);
Vector3D mobiusParameterVgl(double u, double v);

Figure createNavelvormigeTorus(const int n, const int m);
Vector3D nvTorusParameterVgl(double u, double v);

//3D-Fractalen
void generateFractal(Figure& fig, Figures3D& fractal, const int nr_iterations, const double scale);
//void generateFractalsFromList(Figures3D& tempFigs, const double scale)

void copyColors(Figure &fig1, const Figure& fig2);


#endif //ENGINE_3DFIGURES_H
