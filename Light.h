//
// Created by Robbe Branswyk on 03/05/2023.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "L2DSystemen.h"
#include "vector3d.h"
#include "3DLineDrawing.h"

class Light {
public:
    Color ambientLight;
    Color diffuseLight;
    Color specularLight;
};

class InfLight : public Light{
public:
    //De richting waarin de vector schijnt
    Vector3D ldVector;
};

class PointLight : public Light{
public:
    //Locatie puntbron
    Vector3D location;
    //Hoek van de spotlight
    double spotAngle;
};

typedef list<Light>Lights3D;

void calculateAmbient(Figure& fig, Lights3D& lightSources);
void calculateAmbient(Color& ambient, Lights3D& lightSources);

Color resultingColor(const Color& ambient, const Color& diffuse, const Color& specular);


#endif //ENGINE_LIGHT_H
