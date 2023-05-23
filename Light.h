//
// Created by Robbe Branswyk on 03/05/2023.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "L2DSystemen.h"
#include "vector3d.h"
#include "3DLineDrawing.h"
#include <cmath>

class Light {
public:
    Color ambientLight;
    Color diffuseLight;
    Color specularLight;

    virtual double
    diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C, Vector3D &pointInEye, Vector3D &n,
                       Vector3D &l);
    virtual void applyTransformation(const Matrix& m);
};

class InfLight : public Light{
public:
    //De richting waarin de vector schijnt
    Vector3D ldVector;

    double
    diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C, Vector3D &pointInEye, Vector3D &n,
                       Vector3D &l);
    void applyTransformation(const Matrix& m);
};

class PointLight : public Light{
public:
    //Locatie puntbron
    Vector3D location;
    //Hoek van de spotlight
    double spotAngle;

    double
    diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C, Vector3D &pointInEye, Vector3D &n,
                       Vector3D &l);
    void applyTransformation(const Matrix& m);
};

typedef list<Light*>Lights3D;

void calculateAmbient(Color& ambient, Lights3D& lightSources);
void calculateDiffuse(Color &diffuse, Lights3D &lightSources, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                      Vector3D &pointInEye);

void calculateSpecular(Color &specular, double reflectionFactor, Lights3D &lightSources, const Vector3D &A,
                       const Vector3D &B, const Vector3D &C, Vector3D &pointInEye);

Color resultingColor(const Color &ambient, const Color &diffuse, const Color &specular, double reflectionFactor,
                     Lights3D &lightSources, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                     Vector3D projPunt);

void transformLights(Lights3D& lightSources, const Matrix& m);

Vector3D calculateEyePoint(double zValue, double d, double xProj, double yProj, double dx, double dy);





#endif //ENGINE_LIGHT_H
