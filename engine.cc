#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "vector3d.h"
#include "intro.h"
#include "L2DSystemen.h"
#include "3DLineDrawing.h"
#include "3DFigures.h"
#include "ZBuffering.h"
#include "Light.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <list>
#include <cmath>
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
img::EasyImage generate_image(const ini::Configuration &configuration)
{
    //ZBuffer test = ZBuffer(10,20);
    string typePicture = configuration["General"]["type"].as_string_or_die();

    if (typePicture == "IntroColorRectangle"){
        //Informatie INI-file
        int width = configuration["ImageProperties"]["width"].as_int_or_die();
        int height = configuration["ImageProperties"]["height"].as_int_or_die();

        return colorRectangle(width, height);
    }

    else if (typePicture == "IntroBlocks"){
        //Informatie INI-file
        int width = configuration["ImageProperties"]["width"].as_int_or_die();
        int height = configuration["ImageProperties"]["height"].as_int_or_die();
        int squareWidth = width/configuration["BlockProperties"]["nrXBlocks"].as_int_or_die();
        int squareHeight = height/configuration["BlockProperties"]["nrYBlocks"].as_int_or_die();
        vector<double> colorWhite = configuration["BlockProperties"]["colorWhite"].as_double_tuple_or_die();
        vector<double> colorBlack = configuration["BlockProperties"]["colorBlack"].as_double_tuple_or_die();
        bool invert = configuration["BlockProperties"]["invertColors"].as_bool_or_die();

        return colorBlocks(width, height, squareWidth, squareHeight, colorWhite, colorBlack, invert);
    }

    else if (typePicture == "IntroLines"){
        //Informatie INI-file
        int width = configuration["ImageProperties"]["width"].as_int_or_die();
        int height = configuration["ImageProperties"]["height"].as_int_or_die();
        int numbLines = configuration["LineProperties"]["nrLines"].as_int_or_die();
        vector<double> line = configuration["LineProperties"]["lineColor"].as_double_tuple_or_die();

        return introLines(width, height, numbLines, line);
    }

    else if (typePicture == "2DLSystem"){
        //Informatie INI-file
        string inputFile = configuration["2DLSystem"]["inputfile"].as_string_or_die();
        int size = configuration["General"]["size"].as_int_or_die();

        vector<double> lineColorValues = configuration["2DLSystem"]["color"].as_double_tuple_or_die();
        Color lineColor = Color(lineColorValues[0], lineColorValues[1], lineColorValues[2]);

        vector<double> backColorValues = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
        Color backColor = Color(backColorValues[0], backColorValues[1], backColorValues[2]);

        LParser::LSystem2D l_system;
        ifstream input_stream(inputFile);
        input_stream >> l_system;
        input_stream.close();

        Lines2D LSystemLines = drawLSystem(l_system, lineColor);
        return draw2DLines(LSystemLines, size, backColor, typePicture);
    }

    else if (typePicture == "Wireframe" or typePicture == "ZBufferedWireframe" or typePicture == "ZBuffering" or typePicture == "LightedZBuffering"){
        //Eyepoint uitlezen en Matrix aanmaken
        vector<double> eye = configuration["General"]["eye"].as_double_tuple_or_die();
        Vector3D eyePoint = Vector3D::point(eye[0], eye[1], eye[2]);
        Matrix eyeMatrix = eyePointTrans(eyePoint);

        //Size en backgroundcolor
        int size = configuration["General"]["size"].as_int_or_die();
        vector<double> backColorDoubles = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
        Color backColor = Color(backColorDoubles[0], backColorDoubles[1], backColorDoubles[2]);

        //Lichtbronnen maken
        Lights3D lightSources;
        if(typePicture == "LightedZBuffering"){
            int nrOfLightSources = configuration["General"]["nrLights"].as_int_or_die();
            for (int i = 0; i < nrOfLightSources; i++) {
                string lighti = "Light" + to_string(i);

                //Licht aanmaken en ambient factor toevoegen
                Light light;
                vector<double> ambientLight = configuration[lighti]["ambientLight"].as_double_tuple_or_die();
                light.ambientLight = Color(ambientLight[0], ambientLight[1], ambientLight[2]);

                //Diffuus factor
                light.diffuseLight = Color(0,0,0);

                //Specular factor
                light.specularLight = Color(0,0,0);

                //Toevoegen aan de vector
                lightSources.push_back(light);
            }
        } else {
            Light lightSrc;
            lightSrc.ambientLight = Color(1.0, 1.0, 1.0);
            lightSrc.diffuseLight = Color(0.0, 0.0, 0.0);
            lightSrc.specularLight = Color(0.0, 0.0, 0.0);

            lightSources.push_back(lightSrc);
        }


        //De figuur uitlezen
        list<Figure> theFigure;
        //Elke onderdeelFiguur uitlezen en aanmaken
        for (int i = 0; i < configuration["General"]["nrFigures"].as_int_or_die(); i++) {
            string figi = "Figure" + to_string(i);

            //Kleur lezen en Figuur aanmaken
            vector<double> colorDoubles;
            Color figureColor;
            if(typePicture != "LightedZBuffering") {
                 colorDoubles = configuration[figi]["color"].as_double_tuple_or_die();
                 figureColor = Color(colorDoubles[0], colorDoubles[1], colorDoubles[2]);
            }

            string typeFigure = configuration[figi]["type"].as_string_or_die();
            Figure figure;

            if (typeFigure == "LineDrawing") {
                figure = Figure();

                //De punten van de figuur uitlezen en aanmaken
                for (int j = 0; j < configuration[figi]["nrPoints"].as_int_or_die(); j++) {
                    string pointi = "point" + to_string(j);
                    vector<double> pointiDoubles = configuration[figi][pointi].as_double_tuple_or_die();
                    Vector3D pi = Vector3D::point(pointiDoubles[0], pointiDoubles[1], pointiDoubles[2]);
                    figure.points.push_back(pi);
                }

                //De Faces van de figuur uitlezen en aanmaken
                for (int j = 0; j < configuration[figi]["nrLines"].as_int_or_die(); ++j) {
                    string linei = "line" + to_string(j);
                    vector<double> faceDoubles = configuration[figi][linei].as_double_tuple_or_die();
                    Face facei;
                    for (int k = 0; k < faceDoubles.size(); k++) {
                        facei.point_indexes.push_back(faceDoubles[k]);
                    }
                    figure.faces.push_back(facei);
                }
            }

            else if (typeFigure == "Cube" or typeFigure == "FractalCube"){
                figure = createCube();
            }

            else if (typeFigure == "Tetrahedron" or typeFigure == "FractalTetrahedron"){
                figure = createTetrahedron();
            }

            else if (typeFigure == "Octahedron" or typeFigure == "FractalOctahedron"){
                figure = createOctahedron();
            }

            else if (typeFigure == "Icosahedron" or typeFigure == "FractalIcosahedron"){
                figure = createIcosahedron();
            }

            else if (typeFigure == "Dodecahedron" or typeFigure == "FractalDodecahedron"){
                figure = createDodecadron();
            }

            else if (typeFigure == "Sphere"){
                int n = configuration[figi]["n"].as_int_or_die();
                figure = createSphere(n);
            }

            else if (typeFigure == "Cone"){
                int n = configuration[figi]["n"].as_int_or_die();
                double h = configuration[figi]["height"].as_double_or_die();
                figure = createCone(n, h);
            }

            else if (typeFigure == "Cylinder"){
                int n = configuration[figi]["n"].as_int_or_die();
                double h = configuration[figi]["height"].as_double_or_die();
                figure = createCylinder(n, h);
            }

            else if (typeFigure == "Torus"){
                int n = configuration[figi]["n"].as_int_or_die();
                int m = configuration[figi]["m"].as_int_or_die();
                double r = configuration[figi]["r"].as_double_or_die();
                double R = configuration[figi]["R"].as_double_or_die();
                figure = createTorus(r, R, n, m);
            }

            else if (typeFigure == "3DLSystem"){
                string inputfile = configuration[figi]["inputfile"].as_string_or_die();

                LParser::LSystem3D l_system;
                ifstream input_stream(inputfile);
                input_stream >> l_system;
                input_stream.close();

                figure = draw3DLsystem(l_system);

            }

            //EXTRA
            else if (typeFigure == "Mobius"){
                int n = configuration[figi]["n"].as_int_or_die();
                int m = configuration[figi]["m"].as_int_or_die();
                figure = createMobius(n, m);
            }

            else if (typeFigure == "nvTorus"){
                int n = configuration[figi]["n"].as_int_or_die();
                int m = configuration[figi]["m"].as_int_or_die();
                figure = createNavelvormigeTorus(n, m);
            }

            //De verschillende transformaties uitlezen en toepassen op de figuur
            //Scaling
            double scaleFactor = configuration[figi]["scale"].as_double_or_die();
            Matrix scaleM = scaleFigure(scaleFactor);

            //RotateX
            double xAngle = configuration[figi]["rotateX"].as_double_or_die();
            Matrix xRotateM = rotateX(xAngle);

            //RotateY
            double yAngle = configuration[figi]["rotateY"].as_double_or_die();
            Matrix yRotateM = rotateY(yAngle);

            //RotateZ
            double zAngle = configuration[figi]["rotateZ"].as_double_or_die();
            Matrix zRotateM = rotateZ(zAngle);

            //Translatie
            vector<double> centerDoubles = configuration[figi]["center"].as_double_tuple_or_die();
            Vector3D center = Vector3D::vector(centerDoubles[0], centerDoubles[1], centerDoubles[2]);
            Matrix translateM = translate(center);

            //De grote TransformatieMatrix
            Matrix transformaties = scaleM * xRotateM * yRotateM * zRotateM * translateM;

            //Transformatiematrix toepassen op figuur
            applyTransformation(figure, transformaties);

            //LICHT en KLEUR
            if(typePicture == "LightedZBuffering"){
                //Belichtingswaarde inlezen
                vector<double> ambientLight = configuration[figi]["ambientReflection"].as_double_tuple_or_die();
                figure.ambientReflection = Color(ambientLight[0], ambientLight[1], ambientLight[2]);

                //Diffuus factor
                figure.diffuseReflection = Color(0,0,0);

                //Specular factor
                figure.specularReflection = Color(0,0,0);

                figure.reflectionCoefficient = 0;

            } else {
                figure.ambientReflection = figureColor;

                //Diffuus factor
                figure.diffuseReflection = Color(0,0,0);

                //Specular factor
                figure.specularReflection = Color(0,0,0);

                figure.reflectionCoefficient = 0;
            }

            if (typeFigure == "FractalCube" or typeFigure == "FractalTetrahedron" or typeFigure == "FractalOctahedron"
                or typeFigure == "FractalIcosahedron" or typeFigure == "FractalDodecahedron"){
                int nr_iterations = configuration[figi]["nrIterations"].as_int_or_die();
                double scaleFractal = configuration[figi]["fractalScale"].as_double_or_die();

                Figures3D fractals;
                generateFractal(figure, fractals, nr_iterations, scaleFractal);

                for(Figure &fractal : fractals){
                    theFigure.push_back(fractal);
                }
            }
            else {
                //Figuur toevoegen aan 3DFigure
                theFigure.push_back(figure);
            }
        }

        //Figuur aanpassen naar het Eye-coördinaat systeem
        applyTransformation(theFigure, eyeMatrix);

        //Omzetten naar vector van 2D lijnen
        Lines2D linesDrawing = doProjection(theFigure);

        if (typePicture == "ZBuffering" or typePicture == "LightedZBuffering"){
            return drawZBuffFigure(theFigure, linesDrawing, size, backColor, lightSources);
        }

        return draw2DLines(linesDrawing, size, backColor, typePicture);
    }

    else {
        img::EasyImage image(100, 100);
        return image;
    }
}

int main(int argc, char const* argv[])
{
        int retVal = 0;
        try
        {
                std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
                if (args.empty()) {
                        std::ifstream fileIn("filelist");
                        std::string filelistName;
                        while (std::getline(fileIn, filelistName)) {
                                args.push_back(filelistName);
                        }
                }
                for(std::string fileName : args)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(fileName);
                                if (fin.peek() == std::istream::traits_type::eof()) {
                                    std::cout << "Ini file appears empty. Does '" <<
                                    fileName << "' exist?" << std::endl;
                                    continue;
                                }
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cout << "Could not generate image for " << fileName << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }
        return retVal;
}
