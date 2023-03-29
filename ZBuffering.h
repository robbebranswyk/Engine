//
// Created by Robbe Branswyk on 24/03/2023.
//

#ifndef ENGINE_ZBUFFERING_H
#define ENGINE_ZBUFFERING_H
#include <iostream>
using namespace std;
#include <vector>
#include <limits>
#include <sstream>


#include "easy_image.h"
#include "L2DSystemen.h"
#include "3DLineDrawing.h"

//using infinity = numeric_limits<double>::infinity();

class ZBuffer : public vector<vector<double>>{
public:
    ZBuffer(const int width, const int height);
};

void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image,
                     unsigned int x0,  unsigned y0, const double z0,
                     unsigned int x1,  unsigned y1, const double z1,
                    const img::Color &color);




#endif //ENGINE_ZBUFFERING_H
