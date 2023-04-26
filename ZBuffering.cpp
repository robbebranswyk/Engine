//
// Created by Robbe Branswyk on 24/03/2023.
//

#include "ZBuffering.h"
#include <limits>
#include <algorithm>

ZBuffer::ZBuffer(const int width, const int height) {
    vector<double> heightValues(height, numeric_limits<double>::infinity());
    for (int i = 0; i < width; i++) {
        this->push_back(heightValues);
    }
}

void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, int x0, int y0, double z0, int x1, int y1, double z1, const img::Color &color) {

    //1/Z bepalen
    //aantal pixels van lijn = langste lijn breedte OF lengte ≠ lengte lijn
    double lengthP1P2 = max(abs(x1-x0), abs(y1-y0));
    //sqrt( pow((x1-x0),2) + pow((y1-y0), 2));

    //Loopen over al de pixels en waarde p bepalen en ook de 1/z
    double a = lengthP1P2;

    //Code van draw_line
    if (x0 >= image.get_width() || y0 >= image.get_height() || x1 >= image.get_width() || y1 > image.get_height()) {
        std::stringstream ss;
        ss << "Drawing line from (" << x0 << "," << y0 << ") to (" << x1 << "," << y1 << ") in image of width "
           << image.get_width() << " and height " << image.get_height();
        throw std::runtime_error(ss.str());
    }
    //Verticale lijn
    if (x0 == x1)
    {
        int i = 0;

        //In de for-loop zelf geen probleem, maar daarbinne gaan we ervan uit dat z0 de kleinste is van de twee
        if(y1 < y0){
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }

        //special case for x0 == x1
        for (unsigned int j = std::min(y0, y1); j <= std::max(y0, y1); j++)
        {
            double valueP = (a - i) / a;
            double zValue = valueP/z0 + (1-valueP)/z1;
            if (zValue < zbuffer[x0][j]){
                zbuffer[x0][j] = zValue;
                (image)(x0, j) = color;
            }

            i += 1;
        }
    }

    //Horizontale lijn
    else if (y0 == y1)
    {
        int i = 0;

        //In de for-loop zelf geen probleem, maar daarbinne gaan we ervan uit dat z0 de kleinste is van de twee
        if(x1 < x0){
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }

        //special case for y0 == y1
        for (unsigned int j = std::min(x0, x1); j <= std::max(x0, x1); j++)
        {
            double valueP = (a - i) / a;
            double zValue = valueP/z0 + (1-valueP)/z1;
            if (zValue < zbuffer[j][y0]){
                zbuffer[j][y0] = zValue;
                (image)(j, y0) = color;
            }
            i += 1;

        }
    }
    //Schuine lijn
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            int i = 0;
            for (unsigned int j = 0; j <= (x1 - x0); j++)
            {
                double valueP = (a - i) / a;
                double zValue = valueP/z0 + (1-valueP)/z1;
                if (zValue < zbuffer[x0 + j][(unsigned int) round(y0 + m * j)]){
                    zbuffer[x0 + j][(unsigned int) round(y0 + m * j)] = zValue;
                    (image)(x0 + j, (unsigned int) round(y0 + m * j)) = color;
                }
                i += 1;

            }
        }
        else if (m > 1.0)
        {
            int i = 0;
            for (unsigned int j = 0; j <= (y1 - y0); j++)
            {
                double valueP = (a-i)/a;
                double zValue = valueP/z0 + (1-valueP)/z1;
                if (zValue < zbuffer[(unsigned int) round(x0 + (j / m))][y0 + j]){
                    zbuffer[(unsigned int) round(x0 + (j / m))][y0 + j] = zValue;
                    (image)((unsigned int) round(x0 + (j / m)), y0 + j) = color;
                }
                i += 1;
            }
        }
        else if (m < -1.0)
        {
            int i = 0;
            for (unsigned int j = 0; j <= (y0 - y1); j++)
            {
                double valueP = (a - i) / a;
                double zValue = valueP/z0 + (1-valueP)/z1;
                if (zValue < zbuffer[(unsigned int) round(x0 - (j / m))][y0 - j]){
                    zbuffer[(unsigned int) round(x0 - (j / m))][y0 - j] = zValue;
                    (image)((unsigned int) round(x0 - (j / m)), y0 - j) = color;
                }
                i += 1;

            }
        }
    }
}

vector<Face> triangulate(const Face &face) {
    int n = face.point_indexes.size(); //Aantal hoeken figuur (bv n = 6: Zeshoek)
    vector<Face> triangFaces;
    for (int i = 1; i <= n-2; i++) {
        Face fi = Face({face.point_indexes[0], face.point_indexes[i], face.point_indexes[i+1]});
        triangFaces.push_back(fi);
    }

    return triangFaces;
}

Figures3D triangulateFigure(const Figures3D &theFigure) {
    Figures3D triangFigure;
    for (const Figure &fig : theFigure){
        Figure triangFig;
        triangFig.points = fig.points;
        triangFig.color = fig.color;
        for (const Face &i : fig.faces){
            vector<Face> triangFaces = triangulate(i);
            for (Face &j : triangFaces){
                triangFig.faces.push_back(j);
            }
        }
        triangFigure.push_back(triangFig);
    }
    return triangFigure;
}

void draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &image, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                     double d, double dx, double dy, Color color) {

    //Driehoek projecteren
    Point2D projA = Point2D(0,0);
    Point2D projB = Point2D(0,0);
    Point2D projC = Point2D(0,0);

    projA.x = (d*A.x)/(-A.z) + dx;
    projA.y = (d*A.y)/(-A.z) + dy;

    projB.x = (d*B.x)/(-B.z) + dx;
    projB.y = (d*B.y)/(-B.z) + dy;

    projC.x = (d*C.x)/(-C.z) + dx;
    projC.y = (d*C.y)/(-C.z) + dy;

    //Pixels bepalen die tot driehoek behoren
    int yMin = round(min(min(projA.y, projB.y), projC.y) + 0.5);
    int yMax = round(max(max(projA.y, projB.y), projC.y) - 0.5);

    //Bepalen 1/Z-waarde van driehoek (adhv zwaartepunt)
    double xG = (projA.x + projB.x + projC.x)/3.0;
    double yG = (projA.y + projB.y + projC.y)/3;
    double zGValue = 1.0/(3.0*A.z) + 1.0/(3.0*B.z) + 1.0/(3.0*C.z);

    for (int y = yMin; y <= yMax; y++) {
        int xL;
        int xR;
        xLenXR(projA, projB, projC, xL, xR, y);
        for (int x = xL; x <= xR; x++) {
            double dzdx;
            double dzdy;
            incrementZValue(d, A, B, C, dzdx, dzdy);
            double zValue = 1.0001 * zGValue + (x - xG) * dzdx + (y - yG) * dzdy;
            //cout << to_string(zValue)<< endl;
            if (zValue < zbuffer[x][y]){
                zbuffer[x][y] = zValue;
                (image)(x, y) = color.toColor();
            }
            //cout << to_string(x) << ", " << to_string(y) << ": dzdx = " << to_string(dzdx) << " en dzdy = " << to_strin§g(dzdy) << endl;
        }
    }




}

img::EasyImage drawZBuffFigure(Figures3D &theFigure, Lines2D &linesDrawing, const int size, Color backColor) {
    //Triangulatie
    Figures3D theFigureTrian = triangulateFigure(theFigure);

    //De 5 waarden ophalen die in Projection worden berekend
    double imageX;
    double imageY;
    double factor; //schaalfactor d
    double dx;
    double dy;
    draw2DLinesValues(linesDrawing, size, imageX, imageY, factor, dx, dy);

    img::EasyImage image(lround(imageX), lround(imageY), backColor.toColor());
    ZBuffer zbuffer = ZBuffer(lround(imageX), lround(imageY));

    for (Figure &fig : theFigureTrian){
        for (Face &face : fig.faces){
            Vector3D A = fig.points[face.point_indexes[0]];
            Vector3D B = fig.points[face.point_indexes[1]];
            Vector3D C = fig.points[face.point_indexes[2]];
            draw_zbuf_triag(zbuffer, image, A, B, C, factor, dx, dy, fig.color);
        }
    }

    return image;
}

void onLine(const Point2D& p1, const Point2D& p2, double &xI, int yI) {
    if ((yI - p1.y) * (yI - p2.y) <= 0 && p1.y != p2.y) {
        xI = p1.x + (p2.x - p1.x) * ((yI - p1.y) / (p2.y - p1.y));
    }
}

void xLenXR(const Point2D& A, const Point2D& B, const Point2D& C, int &xL, int &xR, int i) {
    double xL_AB = numeric_limits<double>::infinity();
    double xR_AB = -numeric_limits<double>::infinity();

    double xL_AC = numeric_limits<double>::infinity();
    double xR_AC = -numeric_limits<double>::infinity();

    double xL_BC = numeric_limits<double>::infinity();
    double xR_BC = -numeric_limits<double>::infinity();

    vector<double*> xLenR = {&xL_AB, &xL_AC, &xL_BC, &xR_AB, &xR_AC, &xR_BC};
    vector<vector<Point2D>> driehoekzijden = {{A,B}, {A,C}, {B,C}, {A,B}, {A,C}, {B,C}};

    for (int j = 0; j < 6; j++) {
        onLine(driehoekzijden[j][0], driehoekzijden[j][1], *xLenR[j], i);
    }


    xL = round(min(min(xL_AB, xL_AC), xL_BC) + 0.5);
    xR = round(max(max(xR_AB, xR_AC), xR_BC) - 0.5);

    //cout << to_string(xL) << " " << to_string(xR) << endl;
}

void incrementZValue(double factor, const Vector3D& A, const Vector3D& B, const Vector3D& C, double &dzdx, double &dzdy) {
    Vector3D u = B - A;
    Vector3D v = C - A;

    Vector3D w = Vector3D::cross(u, v);
    double k = w.x*A.x + w.y*A.y + w.z*A.z;

    dzdx = w.x/(-factor*k);
    dzdy = w.y/(-factor*k);
}

