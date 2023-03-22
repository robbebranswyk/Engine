//
// Created by Robbe Branswyk on 10/03/2023.
//


#include "intro.h"

using namespace std;

//////INTRO////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
img::EasyImage colorRectangle(int width, int height){
    img::EasyImage image(width,height);
    for (unsigned int i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            image(i, j).red = i;
            image(i, j).green = j;
            image(i, j).blue = (i + j) % height;
        }
    }
    return image;
}

img::EasyImage colorBlocks(int width, int height, int squareWidth, int squareHeight, vector<double> colorWhite, vector<double> colorBlack, bool invert ){
    img::EasyImage image(width, height);

    if (invert){
        vector<double> temp = colorWhite;
        colorWhite = colorBlack;
        colorBlack = temp;
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int Bx = i/squareWidth;
            int By = j/squareHeight;

            if ((Bx+By)%2 == 0){
                image(i, j).red = colorWhite[0]*255;
                image(i,j).green = colorWhite[1]*255;
                image(i, j).blue = colorWhite[2]*255;
            } else {
                image(i, j).red = colorBlack[0]*255;
                image(i,j).green = colorBlack[1]*255;
                image(i, j).blue = colorBlack[2]*255;
            }
        }
    }
    return image;
}


img::EasyImage introLines(int width, int height, int numbLines, vector<double> line){
    img::EasyImage image(width, height);

    img::Color lineColor = img::Color(line[0]*255, line[1]*255, line[2]*255);
    for (int i = 0; i < numbLines; i++) {
        double Hs = (height/numbLines-1)*i;
        double Ws = (width/numbLines-1)*i;
        image.draw_line(0, Ws, Hs, 0, lineColor);

    }
    return image;
}
