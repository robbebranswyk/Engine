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

Vector3D getMidden(vector<Vector3D> punten);

Figure roundingIco(Figure Ico);
Figure createSphere(const int n);

Figure createCone(const int n, const double h);

Figure createCylinder(const int n, const double h);

Figure createTorus(const double r, const double R, const int n, const int m);
Vector3D parameterVgl(double u, double v, const double r, const double R);




#endif //ENGINE_3DFIGURES_H
