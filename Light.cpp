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

void calculateDiffuse(Color &diffuse, Lights3D &lightSources, const Vector3D &A, const Vector3D &B, const Vector3D &C) {
    Color resultTempDiffuse = Color(0,0,0);
    for(Light* light : lightSources){
        double cosfactor = light->diffuseExtraFactor(A, B, C);
        Color tempDiffuse = Color(diffuse.red * light->diffuseLight.red, diffuse.green * light->diffuseLight.green, diffuse.blue * light->diffuseLight.blue);
        tempDiffuse.red *= cosfactor;
        tempDiffuse.green *= cosfactor;
        tempDiffuse.blue *= cosfactor;

        //toevoegen aan resultaat
        resultTempDiffuse = Color(resultTempDiffuse.red + tempDiffuse.red, resultTempDiffuse.green + tempDiffuse.green, resultTempDiffuse.blue + tempDiffuse.blue);
    }
    diffuse = resultTempDiffuse;
}

Color resultingColor(const Color& ambient, const Color& diffuse, const Color& specular) {
    double red = ambient.red + diffuse.red + specular.red;
    double green = ambient.green+ diffuse.green+ specular.green;
    double blue = ambient.blue + diffuse.blue + specular.blue;

    Color result = Color(red, green, blue);

    return result;
}




double Light::diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C) {
    return 0;
}

double InfLight::diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C) {
    Vector3D u = B - A;
    Vector3D v = C - A;

    Vector3D n = Vector3D::cross(u, v);
    Vector3D::normalise(n);

    Vector3D l = ldVector * (-1);
    Vector3D::normalise(l);

    Vector3D result = Vector3D::cross(n, l);
    return result.x + result.y + result.z;

}

double PointLight::diffuseExtraFactor(const Vector3D &A, const Vector3D &B, const Vector3D &C) {
    return Light::diffuseExtraFactor(A, B, C);
}
