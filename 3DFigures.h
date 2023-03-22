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

//Figure createCone(const int n, )




#endif //ENGINE_3DFIGURES_H
