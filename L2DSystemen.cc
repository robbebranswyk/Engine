//
// Created by Robbe Branswyk on 13/03/2023.
//

#include "L2DSystemen.h"
#include "ZBuffering.h"

img::EasyImage draw2DLines(Lines2D &lines, const int size, Color backColor, bool zbuf) {
    //1. Bepaal de minima en maxima
    Line2D firstLine = lines.front();
    double xmin = firstLine.p1.x; double xmax = firstLine.p1.x;
    double ymin = firstLine.p1.y; double ymax = firstLine.p1.y;

    for (auto i : lines){
        if (i.p1.x < xmin){xmin = i.p1.x;}
        if (i.p2.x < xmin){xmin = i.p2.x;}

        if (i.p1.x > xmax){xmax = i.p1.x;}
        if (i.p2.x > xmax){xmax = i.p2.x;}

        if (i.p1.y < ymin){ymin = i.p1.y;}
        if (i.p2.y < ymin){ymin = i.p2.y;}

        if (i.p1.y > ymax){ymax = i.p1.y;}
        if (i.p2.y > ymax){ymax = i.p2.y;}
    }

    //2. Bepaal de grootte van de image
    double rangeX = xmax - xmin;
    double rangeY = ymax - ymin;

    double imageX = size * (rangeX / max(rangeX, rangeY));
    double imageY = size * (rangeY / max(rangeX, rangeY));
    //cout << imageX << " and " << imageY << endl;

    //3. Schaal de lijntekening
    double factor = 0.95 * (imageX / rangeX);

    for (auto &i : lines){
        i.p1.x *= factor; i.p1.y *= factor;
        i.p2.x *= factor; i.p2.y *= factor;
    }

    //4. Verschuif de tekening
    pair<double, double> imageM = {factor * (xmin + xmax) / 2.0, factor * (ymin + ymax) / 2.0};
    double dx = imageX/2.0 - imageM.first;
    double dy = imageY/2.0 - imageM.second;

    for (auto &i : lines){
        i.p1.x += dx; i.p1.y += dy;
        i.p2.x += dx; i.p2.y += dy;
    }

    //5. Coördinaten afronden en Lijnen tekenen
    img::EasyImage image(lround(imageX), lround(imageY), backColor.toColor());
    ZBuffer zbuffer = ZBuffer(lround(imageX), lround(imageY));

    for (auto line : lines){
        if(zbuf){
            draw_zbuf_line(zbuffer, image, lround(line.p1.x), lround(line.p1.y), line.z1, lround(line.p2.x), lround(line.p2.y), line.z2, line.color.toColor());
        }
        else {
            image.draw_line(lround(line.p1.x), lround(line.p1.y), lround(line.p2.x), lround(line.p2.y), line.color.toColor());
        }
    }

    return image;

}

Lines2D drawLSystem(LParser::LSystem2D &l_system, Color lineColor){
    Lines2D LSystemLines = {};

    //1. Al de Informatie ophalen

    set<char> Alphabet = l_system.get_alphabet();
    //verzamelingen met A0 de symbolen zonder tekenen en A1 met tekenen
    set<char> A0 = {};
    set<char> A1 = {};
    for (char i : Alphabet){
        if(l_system.draw(i)){A1.insert(i);}
        else {A0.insert(i);}
    }

    //Beginhoek en hoek omzettenn naar radialen
    double Alpha0 = l_system.get_starting_angle() * (M_PI/180.0);
    double Angle = l_system.get_angle() * (M_PI/180.0);

    //Iteration ophalen
    string initiator = l_system.get_initiator();

    //2. Beginnen Loopen en juiste string vormen
    string bigString = initiator;

    for (int i = 0; i < l_system.get_nr_iterations(); i++) {
        string tempString = "";
        for (char c : bigString){
            if (Alphabet.find(c) != Alphabet.end()){
                tempString += l_system.get_replacement(c);
            } else {
                tempString += c;
            }
        }
        bigString = tempString;
    }


    //3. Over de String lopen
    Point2D currentPoint = Point2D(0,0);
    double currentAngle = Alpha0;
    vector<pair<Point2D, double>> saveAngles = {};

    for(char c : bigString){
        if (c == '+'){
            currentAngle += Angle;
        } else if (c == '-'){
            currentAngle -= Angle;
        } else if (c == '('){
            saveAngles.push_back({currentPoint, currentAngle});
        } else if (c == ')'){
            pair<Point2D, double> recover = saveAngles[saveAngles.size() - 1];
            saveAngles.pop_back();

            currentPoint = recover.first;
            currentAngle = recover.second;
        } else {
            //Bereken eind punt
            Point2D p1 = currentPoint;
            currentPoint.x += cos(currentAngle);
            currentPoint.y += sin(currentAngle);

            if (l_system.draw(c)){
                //Voeg Lijn toe aan Set
                Line2D drawLine = Line2D(p1, currentPoint, lineColor);
                LSystemLines.insert(LSystemLines.end(), drawLine);
            }
        }
    }


    return LSystemLines;
}