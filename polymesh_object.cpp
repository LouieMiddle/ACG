/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "polymesh_object.h"
#include "hit.h"
#include "transform.h"
#include "ray.h"

using namespace std;

template<typename Out>
void split(const string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

PolyMesh::PolyMesh(char *file, bool smooth) {
    ifstream infile;
    infile.open(file);

    if (!infile.is_open()) {
        perror("Error opening polymesh file");
        exit(EXIT_FAILURE);
    }

    string first_line;
    getline(infile, first_line);
    if (first_line == "kcply") {
        cout << "ken cameron ply reading..." << endl;
    } else {
        cout << "not ken cameron ply, ignoring file" << endl;
        exit(EXIT_FAILURE);
    }

    string second_line;
    getline(infile, second_line);
    if ((second_line.rfind("element", 0) == 0) && (second_line.rfind("vertex", 8) == 8)) {
        vertex_count = stoi(second_line.substr(15));
    } else {
        cout << "can't find vertexes, ignoring file" << endl;
        exit(EXIT_FAILURE);
    }

    string third_line;
    getline(infile, third_line);
    if ((second_line.rfind("element", 0) == 0) && (third_line.rfind("face", 8) == 8)) {
        triangle_count = stoi(third_line.substr(13));
    } else {
        cout << "can't find triangles, ignoring file" << endl;
        exit(EXIT_FAILURE);
    }

    vertex = new Vertex[vertex_count];
    triangle = new TriangleIndex[triangle_count];

    string line;

    for (int v = 0; v < vertex_count; v++) {
        getline(infile, line);
        vector<string> words = split(line, ' ');

        vertex[v] = *new Vertex(stof(words[0]), stof(words[1]), stof(words[2]));
    }

    for (int t = 0; t < triangle_count; t++) {
        getline(infile, line);
        vector<string> words = split(line, ' ');

        triangle[t][0] = stoi(words[1]);
        triangle[t][1] = stoi(words[2]);
        triangle[t][2] = stoi(words[3]);
    }
}


Hit *PolyMesh::intersection(Ray ray) {
    Hit *hits = 0;
//BEGIN_STAGE_ONE
//END_STAGE_ONE
    return hits;
}

void PolyMesh::apply_transform(Transform &trans) {
    for (int v = 0; v < vertex_count; v++) {
        trans.apply(vertex[v]);
    }
}