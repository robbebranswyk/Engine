//
// Created by Robbe Branswyk on 15/03/2023.

#include "3DFigures.h"

//Functies en code om de Platonische Lichamen te maken zijn gemaakt en gedeeld door/met Joren Van der Sande en Riwaaz Ranabhat
//Omdat het alleen maar overtypen is

Figure createCube() {
    Figure cube = Figure();

    cube.points.push_back(Vector3D::point(1, -1, -1));
    cube.points.push_back(Vector3D::point(-1, 1, -1));
    cube.points.push_back(Vector3D::point(1,1,1));
    cube.points.push_back(Vector3D::point(-1, -1, 1));
    cube.points.push_back(Vector3D::point(1,1,-1));
    cube.points.push_back(Vector3D::point(-1, -1, -1));
    cube.points.push_back(Vector3D::point(1, -1, 1));
    cube.points.push_back(Vector3D::point(-1, 1, 1));

    cube.faces.push_back(Face({0, 4, 2, 6}));
    cube.faces.push_back(Face({4, 1, 7, 2}));
    cube.faces.push_back(Face({1, 5, 3, 7}));
    cube.faces.push_back(Face({5, 0, 6, 3}));
    cube.faces.push_back(Face({6, 2, 7, 3}));
    cube.faces.push_back(Face({0, 5, 1, 4}));

    return cube;
}

Figure createOctahedron() {
    Figure octaH;
    octaH.points.push_back(Vector3D::point(1,0,0));
    octaH.points.push_back(Vector3D::point(0,1,0));
    octaH.points.push_back(Vector3D::point(-1,0,0));
    octaH.points.push_back(Vector3D::point(0,-1,0));
    octaH.points.push_back(Vector3D::point(0,0,-1));
    octaH.points.push_back(Vector3D::point(0,0,1));

    octaH.faces.push_back(Face({0,1,5}));
    octaH.faces.push_back(Face({1,2,5}));
    octaH.faces.push_back(Face({2,3,5}));
    octaH.faces.push_back(Face({3,0,5}));
    octaH.faces.push_back(Face({1,0,4}));
    octaH.faces.push_back(Face({2,1,4}));
    octaH.faces.push_back(Face({3,2,4}));
    octaH.faces.push_back(Face({0,3,4}));

    return octaH;
}


Figure createTetrahedron() {
    Figure tetrahedron = Figure();

    tetrahedron.points.push_back(Vector3D::point(1, -1, -1));
    tetrahedron.points.push_back(Vector3D::point(-1, 1, -1));
    tetrahedron.points.push_back(Vector3D::point(1, 1, 1));
    tetrahedron.points.push_back(Vector3D::point(-1, -1, 1));

    tetrahedron.faces.push_back(Face({0, 1, 2}));
    tetrahedron.faces.push_back(Face({1, 3, 2}));
    tetrahedron.faces.push_back(Face({0, 3, 1}));
    tetrahedron.faces.push_back(Face({0, 2, 3}));

    return tetrahedron;
}

Figure createIcosahedron() {
    Figure icosahedron;
    for (int i = 1; i < 13; i++) {
        if (i == 1){
            icosahedron.points.push_back(Vector3D::point(0, 0, sqrt(5)/2));
        }
        else if (i == 2 or i == 3 or i == 4 or i == 5 or i == 6){
            double x = cos((i-2)*2*M_PI/5);
            double y = sin((i-2)*2*M_PI/5);
            double z = 0.5;
            icosahedron.points.push_back(Vector3D::point(x, y, z));
        }
        else if (i == 7 or i == 8 or i == 9 or i == 10 or i == 11){
            double x = cos(M_PI/5 + (i-7)*2*M_PI/5);
            double y = sin(M_PI/5 + (i-7)*2*M_PI/5);
            double z = -0.5;
            icosahedron.points.push_back(Vector3D::point(x, y, z));
        }
        else if (i == 12){
            icosahedron.points.push_back(Vector3D::point(0, 0, -sqrt(5)/2));
        }
    }

    icosahedron.faces.push_back(Face({0,1,2}));
    icosahedron.faces.push_back(Face({0,2,3}));
    icosahedron.faces.push_back(Face({0,3,4}));
    icosahedron.faces.push_back(Face({0,4,5}));
    icosahedron.faces.push_back(Face({0,5,1}));
    icosahedron.faces.push_back(Face({1,6,2}));
    icosahedron.faces.push_back(Face({2,6,7}));
    icosahedron.faces.push_back(Face({2,7,3}));
    icosahedron.faces.push_back(Face({3,7,8}));
    icosahedron.faces.push_back(Face({3,8,4}));
    icosahedron.faces.push_back(Face({4,8,9}));
    icosahedron.faces.push_back(Face({4,9,5}));
    icosahedron.faces.push_back(Face({5,9,10}));
    icosahedron.faces.push_back(Face({5,10,1}));
    icosahedron.faces.push_back(Face({1,10,6}));
    icosahedron.faces.push_back(Face({11,7,6}));
    icosahedron.faces.push_back(Face({11,8,7}));
    icosahedron.faces.push_back(Face({11,9,8}));
    icosahedron.faces.push_back(Face({11,10,9}));
    icosahedron.faces.push_back(Face({11,6,10}));

    return icosahedron;
}


Vector3D getMidden(const vector<Vector3D>& punten) {
    double x = (punten[0].x + punten[1].x +punten[2].x) / 3;
    double y = (punten[0].y + punten[1].y +punten[2].y) / 3;
    double z = (punten[0].z + punten[1].z +punten[2].z) / 3;
    return Vector3D::point(x,y,z);
}

Figure createDodecadron() {
    Figure dodecahedron;
    Figure ico = createIcosahedron();

    for(auto i : ico.faces){
        vector<Vector3D> points3;
        for (int j : i.point_indexes) {
             points3.push_back(ico.points[j]);
        }
        dodecahedron.points.push_back(getMidden(points3));
    }

    dodecahedron.faces.push_back(Face({0,1,2,3,4}));
    dodecahedron.faces.push_back(Face({0,5,6,7,1}));
    dodecahedron.faces.push_back(Face({1,7,8,9,2}));
    dodecahedron.faces.push_back(Face({2,9,10,11,3}));
    dodecahedron.faces.push_back(Face({3,11,12,13,4}));
    dodecahedron.faces.push_back(Face({4,13,14,5,0}));
    dodecahedron.faces.push_back(Face({19,18,17,16,15}));
    dodecahedron.faces.push_back(Face({19,14,13,12,18}));
    dodecahedron.faces.push_back(Face({18,12,11,10,17}));
    dodecahedron.faces.push_back(Face({17,10,9,8,16}));
    dodecahedron.faces.push_back(Face({16,8,7,6,15}));
    dodecahedron.faces.push_back(Face({15,6,5,14,19}));

    return dodecahedron;
}

Figure roundingIco(const Figure& Ico) {
    Figure sphere;
    for (auto i : Ico.faces){
        Vector3D A = Ico.points[i.point_indexes[0]];
        Vector3D B = Ico.points[i.point_indexes[1]];
        Vector3D C = Ico.points[i.point_indexes[2]];

        //De 3 extra punten aanmaken
        Vector3D D = (A+B)/2;
        Vector3D E = (A+C)/2;
        Vector3D F = (B+C)/2;

        //Alle punten toevoegen aan de Bol
        sphere.points.push_back(A);
        sphere.points.push_back(B);
        sphere.points.push_back(C);
        sphere.points.push_back(D);
        sphere.points.push_back(E);
        sphere.points.push_back(F);

        //4 nieuwe faces aanmaken
        int indexLastPnt = sphere.points.size()-1;
        Face ADE = Face({indexLastPnt-5, indexLastPnt-2, indexLastPnt-1});
        Face DBF = Face({indexLastPnt-2, indexLastPnt-4, indexLastPnt});
        Face DFE = Face({indexLastPnt-2, indexLastPnt, indexLastPnt-1});
        Face FCE = Face({indexLastPnt, indexLastPnt-3, indexLastPnt-1});

        //4 nieuwe faces toevoegen
        sphere.faces.push_back(ADE);
        sphere.faces.push_back(DBF);
        sphere.faces.push_back(DFE);
        sphere.faces.push_back(FCE);
    }
    return sphere;
}

Figure createSphere(const int n) {
    Figure sphere = createIcosahedron();
    for (int i = 0; i < n; ++i) {
        sphere = roundingIco(sphere);
    }
    for (auto &i : sphere.points){
        double r = sqrt(pow(i.x,2) + pow(i.y,2) + pow(i.z, 2));
        i = i/r;
    }
    return sphere;
}

Figure createCone(const int n, const double h) {
    Figure cone;

    //Top aanmaken
    Vector3D top = Vector3D::point(0,0,h);

    //Alle punten aanmaken en toevoegen
    for (int i = 0; i < n; i++) {
        Vector3D pointi = Vector3D::point(cos(2 * i * M_PI / n), sin(2 * i * M_PI / n), 0);
        cone.points.push_back(pointi);
    }

    //Top toevoegen
    cone.points.push_back(top);

    //Alle faces aanmaken
    for (int i = 0; i < n; i++) {
        Face facei = Face({i, (i+1)%n, n});
        cone.faces.push_back(facei);
    }

    //Ondervlak
    Face underFace = Face();
    for (int i = 0; i < n; i++) {
        underFace.point_indexes.push_back(i);
    }

    return cone;
}

Figure createCylinder(const int n, const double h) {
    Figure cylinder;
    
    //Punten aanmaken ondervlak
    for (int i = 0; i < n; i++) {
        Vector3D pointi = Vector3D::point(cos(2 * i * M_PI / n), sin(2 * i * M_PI / n), 0);
        cylinder.points.push_back(pointi);
    }

    //Punten aanmaken bovenvlak
    for (int i = n; i < 2*n; i++) {
        Vector3D pointi = Vector3D::point(cos(2 * (i%n) * M_PI / n), sin(2 * (i%n) * M_PI / n), h);
        cylinder.points.push_back(pointi);
    }

    //Ondervlak
    Face bottomFace = Face();
    for (int i = 0; i < n; i++) {
        bottomFace.point_indexes.push_back(i);
    }
    cylinder.faces.push_back(bottomFace);

    //Bovenvlak
    Face topFace = Face();
    for (int i = n; i < 2*n; i++) {
        topFace.point_indexes.push_back(i);
    }
    cylinder.faces.push_back(topFace);

    //Zijvlakken
    for (int i = 0; i < n; i++) {
        Face facei = Face({i, (i+1)%n, ((i+1)%n)+n, i+n});
        cylinder.faces.push_back(facei);
    }

    return cylinder;

}

Figure createTorus(const double r, const double R, const int n, const int m) {
    Figure torus;

    //Alle punten aanmaken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double u = 2*i*M_PI/n;
            double v = 2*j*M_PI/m;
            Vector3D point = parameterVgl(u, v, r, R);
            torus.points.push_back(point);
        }
    }

    //Alle faces aanmaken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Face face = Face({(j+(i*m)), (j+((i+1)%n)*m), (((j+1)%m) + ((i+1)%n)*m), (((j+1)%m) + i*m)});
            torus.faces.push_back(face);
        }
    }

    return torus;

}

Vector3D parameterVgl(double u, double v, const double r, const double R) {
    double x = (R + r* cos(v))* cos(u);
    double y = (R + r* cos(v))* sin(u);
    double z = r* sin(v);

    return Vector3D::point(x, y, z);
}

Figure draw3DLsystem(LParser::LSystem3D &l_system) {
    Figure System3D;
    Vector3D currPos = Vector3D::point(0,0,0);

    Vector3D H = Vector3D::vector(1,0,0);
    Vector3D L = Vector3D::vector(0,1,0);
    Vector3D U = Vector3D::vector(0,0,1);

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
    vector<vector<Vector3D>> save = {};

    for (char c: bigString){
        Vector3D oldH = H;
        Vector3D oldL = L;
        Vector3D oldU = U;
        if (c == '+'){
            H = oldH* cos(Angle) + oldL * sin(Angle);
            L = oldH * (-sin(Angle)) + oldL * cos(Angle);
        } else if (c == '-'){
            H = oldH* cos(-Angle) + oldL * sin(-Angle);
            L = oldH * (-sin(-Angle)) + oldL * cos(-Angle);
        } else if (c == '^'){
            H = oldH* cos(Angle) + oldU * sin(Angle);
            U = oldH * (-sin(Angle)) + oldU * cos(Angle);
        } else if (c == '&'){
            H = oldH* cos(-Angle) + oldU * sin(-Angle);
            U = oldH * (-sin(-Angle)) + oldU * cos(-Angle);
        } else if (c == '\\'){
            L = oldL*cos(Angle) - oldU*sin(Angle);
            U = oldL*sin(Angle) + oldU*cos(Angle);
        } else if (c == '/'){
            L = oldL*cos(-Angle) - oldU*sin(-Angle);
            U = oldL*sin(-Angle) + oldU*cos(-Angle);
        } else if (c == '|'){
            H = -H;
            L = -L;
        } else if (c == '('){
            save.push_back({currPos, H, L, U});
        } else if (c == ')'){
            vector<Vector3D> recover = save[save.size()-1];
            save.pop_back();
            currPos = recover[0];
            H = recover[1];
            L = recover[2];
            U = recover[3];
        } else {
            if(l_system.draw(c)){
                System3D.points.push_back(currPos);
                System3D.points.push_back(currPos+H);

                int pointSize = System3D.points.size();
                Face newFace = Face({pointSize-2, pointSize-1});
                System3D.faces.push_back(newFace);
            }
            currPos += H;
        }
    }

    return System3D;
}

Figure createMobius(const int n, const int m) {
    Figure mobius;

    //Alle punten aanmaken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double u = 2*i*M_PI/n;
            double v = 2*j*M_PI/m;
            Vector3D point = mobiusParameterVgl(u, v);
            mobius.points.push_back(point);
        }
    }

    //Alle faces aanmaken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Face face = Face({(j+(i*m)), (j+((i+1)%n)*m), (((j+1)%m) + ((i+1)%n)*m), (((j+1)%m) + i*m)});
            mobius.faces.push_back(face);
        }
    }

    return mobius;
}

Vector3D mobiusParameterVgl(double u, double v) {
    double x = (1+(1/2)*v*cos(1/2)*u);
    double y = (1+(1/2)*v*cos(1/2)*v);
    double z = (1/2)*v*sin(1/2)*u;

    return Vector3D::point(x, y, z);
}

Figure createNavelvormigeTorus(const int n, const int m) {
    Figure nvTorus;

    //Alle punten aanmaken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double u = 2*i*M_PI/n;
            double v = 2*j*M_PI/m;
            Vector3D point = nvTorusParameterVgl(u, v);
            nvTorus.points.push_back(point);
        }
    }

    //Alle faces aanmaken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Face face = Face({(j+(i*m)), (j+((i+1)%n)*m), (((j+1)%m) + ((i+1)%n)*m), (((j+1)%m) + i*m)});
            nvTorus.faces.push_back(face);
        }
    }

    return nvTorus;
}

Vector3D nvTorusParameterVgl(double u, double v) {
    double x = sin(u)*(7+ cos((u/3)-2*u) + 2*cos((u/3)+v));
    double y = cos(u)*(7+ cos((u/3)-2*u) + 2*cos((u/3)+v));
    double z = sin((u/3) - 2*v) + 2*sin((u/3)+v);

    return Vector3D::point(x, y, z);
}

void generateFractal(Figure &fig, Figures3D &fractal, const int nr_iterations, const double scale) {
    Figures3D tempFractals = {fig}; //we beginnen met 1 figuur = 1 fractaal

    //Aantal hoekpunten
    int nr_corners = fig.points.size();

    //Aantal iteraties
    for (int i = 0; i < nr_iterations; i++) {

        Figures3D tempMiniFractals; //Nog een temporary Vector

        //Elke figuur/fractaal overlopen
        for(Figure miniFig : tempFractals){
            for (int j = 0; j < nr_corners; j++) {
                Figure newFig = miniFig;

                //schaalMatrix
                Matrix scaleM = scaleFigure(1/scale);
                //Matrix toepassen/schalen
                applyTransformation(newFig, scaleM);

                //Translatie
                //verschuif over vector
                Vector3D translatieVector = miniFig.points[j] - newFig.points[j];
                //TranslatieMatrix
                Matrix translateM = translate(translatieVector);
                //Toepassen/verplaatsen
                applyTransformation(newFig, translateM);

                //Toevoegen aan de tijdelijke vector tempMiniFractals
                tempMiniFractals.push_back(newFig);
            }
        }
        if(i == nr_iterations-1){
            fractal = tempMiniFractals;
        } else{
            tempFractals = tempMiniFractals;
        }
    }


}
