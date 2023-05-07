//
// Created by Robbe Branswyk on 03/05/2023.
//

#include "Light.h"

void calculateAmbient(Figure &fig, Lights3D &lightSources) {
    Color ambientFactor = fig.ambientReflection;
    for(Light &light : lightSources){
        Color tempAmbient = Color(ambientFactor.red * light.ambientLight.red, ambientFactor.green * light.ambientLight.green, ambientFactor.blue * light.ambientLight.blue);
        fig.ambientReflection = Color(ambientFactor.red + tempAmbient.red, ambientFactor.green + tempAmbient.green, ambientFactor.blue + tempAmbient.blue);
    }
}

void calculateAmbient(Color& ambient, Lights3D &lightSources) {
    Color resultingAmbientFactor = Color(0, 0, 0);
    for(Light &light : lightSources){
        Color tempAmbient = Color(ambient.red * light.ambientLight.red, ambient.green * light.ambientLight.green, ambient.blue * light.ambientLight.blue);
        resultingAmbientFactor = Color(resultingAmbientFactor.red + tempAmbient.red, resultingAmbientFactor.green + tempAmbient.green, resultingAmbientFactor.blue + tempAmbient.blue);
    }
    ambient = resultingAmbientFactor;
}

Color resultingColor(const Color& ambient, const Color& diffuse, const Color& specular) {
    double red = ambient.red + diffuse.red + specular.red;
    double green = ambient.green+ diffuse.green+ specular.green;
    double blue = ambient.blue + diffuse.blue + specular.blue;

    Color result = Color(red, green, blue);

    return result;
}



