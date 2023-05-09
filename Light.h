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

    virtual double diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C);
};

class InfLight : public Light{
public:
    //De richting waarin de vector schijnt
    Vector3D ldVector;

    double diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C);
};

class PointLight : public Light{
public:
    //Locatie puntbron
    Vector3D location;
    //Hoek van de spotlight
    double spotAngle;

    double diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C);
};

typedef list<Light*>Lights3D;

void calculateAmbient(Color& ambient, Lights3D& lightSources);
void calculateDiffuse(Color &diffuse, Lights3D &lightSources, const Vector3D &A, const Vector3D &B, const Vector3D &C);

Color resultingColor(const Color& ambient, const Color& diffuse, const Color& specular);


#endif //ENGINE_LIGHT_H
