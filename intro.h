//
// Created by Robbe Branswyk on 10/03/2023.
//

#ifndef ENGINE_INTRO_H
#define ENGINE_INTRO_H

#endif //ENGINE_INTRO_H
#include <cmath>
#include <list>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include "vector3d.h"
#include "l_parser.h"
#include "ini_configuration.h"
#include "easy_image.h"
#include <vector>
using namespace std;


img::EasyImage colorRectangle(int width, int height);

img::EasyImage colorBlocks(int width, int height, int squareWidth, int squareHeight, vector<double> colorWhite, vector<double> colorBlack, bool invert );

img::EasyImage introLines(int width, int height, int numbLines, vector<double> line);