//
// Created by Robbe Branswyk on 03/05/2023.
//

#include "Light.h"


void calculateAmbient(Color& ambient, Lights3D &lightSources) {
    Color resultingAmbientFactor = Color(0, 0, 0);
    for(Light* light : lightSources){
        Color tempAmbient = Color(ambient.red * light->ambientLight.red, ambient.green * light->ambientLight.green, ambient.blue * light->ambientLight.blue);
        resultingAmbientFactor = Color(resultingAmbientFactor.red + tempAmbient.red, resultingAmbientFactor.green + tempAmbient.green, resultingAmbientFactor.blue + tempAmbient.blue);
    }
    ambient = resultingAmbientFactor;
}

void calculateDiffuse(Color &diffuse, Lights3D &lightSources, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                      Vector3D &pointInEye) {
    Color resultTempDiffuse = Color(0,0,0);
    for(Light* light : lightSources){
        Vector3D l;
        Vector3D n;
        double cosfactor = light->diffuseExtraFactor(A, B, C, pointInEye, n, l);

        //cout << to_string(cosfactor) << endl;

        Color tempDiffuse = Color(diffuse.red * light->diffuseLight.red, diffuse.green * light->diffuseLight.green, diffuse.blue * light->diffuseLight.blue);
        tempDiffuse.red *= cosfactor;
        tempDiffuse.green *= cosfactor;
        tempDiffuse.blue *= cosfactor;

        //toevoegen aan resultaat
        resultTempDiffuse = Color(resultTempDiffuse.red + tempDiffuse.red, resultTempDiffuse.green + tempDiffuse.green, resultTempDiffuse.blue + tempDiffuse.blue);
    }
    diffuse = resultTempDiffuse;
}

void calculateSpecular(Color &specular, double reflectionFactor, Lights3D &lightSources, const Vector3D &A,
                       const Vector3D &B, const Vector3D &C, Vector3D &pointInEye) {

    Color resultTempSpecular = Color(0,0,0);
    for(Light* light : lightSources){
        Vector3D l;
        Vector3D n;
        double cosFactor = light->diffuseExtraFactor(A, B, C, pointInEye, n, l);

        Vector3D r = 2*n*cosFactor + l;
        r = Vector3D::normalise(r);

        Vector3D pointToEye =  - pointInEye;
        pointToEye = Vector3D::normalise(pointToEye);

        double cosBeta = Vector3D::dot(r, pointToEye);
        cosBeta = pow(cosBeta, reflectionFactor);

        Color tempSpecular = Color(specular.red * light->specularLight.red, specular.green * light->specularLight.green, specular.blue * light->specularLight.blue);
        tempSpecular.red *= cosBeta;
        tempSpecular.green *= cosBeta;
        tempSpecular.blue *= cosBeta;

        resultTempSpecular = Color(resultTempSpecular.red + tempSpecular.red, resultTempSpecular.green + tempSpecular.green, resultTempSpecular.blue + tempSpecular.blue);
    }
    specular = resultTempSpecular;
}

Color resultingColor(const Color &ambient, const Color &diffuse, const Color &specular, double reflectionFactor,
                     Lights3D &lightSources, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                     Vector3D projPunt) {

    Color tempAmbient = ambient;
    Color tempDiffuse = diffuse;
    Color tempSpecular = specular;

    //Licht berekenen
    calculateAmbient(tempAmbient, lightSources);
    calculateDiffuse(tempDiffuse, lightSources, A, B, C, projPunt);
    calculateSpecular(tempSpecular, reflectionFactor, lightSources, A, B, C, projPunt);

    double red = tempAmbient.red + tempDiffuse.red + tempSpecular.red;
    double green = tempAmbient.green+ tempDiffuse.green+ tempSpecular.green;
    double blue = tempAmbient.blue + tempDiffuse.blue + tempSpecular.blue;

    vector<double> threeColors = {red, green, blue};
    for(double &color : threeColors){
        if(color > 1){
            color = 1;
        }
    }
    Color result = Color(threeColors[0], threeColors[1], threeColors[2]);

    //cout << to_string(result.red) << ", " << to_string(result.green) << ", " << to_string(result.blue) << endl;

    return result;
}

void transformLights(Lights3D &lightSources, const Matrix &m) {
    for(Light* lightSource : lightSources){
        lightSource->applyTransformation(m);
    }
}

Vector3D calculateEyePoint(double zValue, double d, double xProj, double yProj, double dx, double dy) {
    xProj -= dx;
    yProj -= dy;
    double x = (xProj/(-zValue))/d;
    double y = (yProj/(-zValue))/d;

    return Vector3D::point(x, y, 1/zValue);
}




double
Light::diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C, Vector3D &pointInEye, Vector3D &n,
                          Vector3D &l) {
    return 0;
}

void Light::applyTransformation(const Matrix &m) {
    return;
}

void InfLight::applyTransformation(const Matrix &m) {
    ldVector *= m;
}

void PointLight::applyTransformation(const Matrix &m) {
    location *= m;
}



double
InfLight::diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C, Vector3D &pointInEye, Vector3D &n,
                             Vector3D &l) {
    Vector3D u = B - A;
    Vector3D v = C - A;

    n = Vector3D::cross(u, v);
    n = Vector3D::normalise(n);

    l = ldVector * (-1);
    l = Vector3D::normalise(l);

    double cos = Vector3D::dot(n, l); //n.x * l.x + n.y * l.y + n.z * l.z;
    if (cos < 0){
        cos = 0;
    }
    return cos;
}

double PointLight::diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C, Vector3D &pointInEye,
                                      Vector3D &n,
                                      Vector3D &l) {
    Vector3D u = B - A;
    Vector3D v = C - A;

    n = Vector3D::cross(u, v);
    n = Vector3D::normalise(n);

    l = pointInEye - location;
    l = Vector3D::normalise(l);

    double cosValue = Vector3D::dot(n, -l); //n.x * l.x + n.y * l.y + n.z * l.z;
    double cosSpot = cos(spotAngle);

    //cout << to_string(cosValue) << endl;

    if(cosValue > cosSpot){
        cosValue = 1 - (1-cosValue)/(1-cosSpot);
    } else {
        cosValue = 0;
    }

    return cosValue;
}
