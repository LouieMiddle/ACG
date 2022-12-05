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
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "polymesh_object.h"

using namespace std;

PolyMesh::PolyMesh(char *file, bool smooth) {
    int count;

    ifstream meshfile(file);

    smoothing = smooth;

    cerr << "Opening meshfile: " << file << endl;

    if (!meshfile.is_open()) {
        cerr << "Problem reading meshfile (not found)." << endl;
        meshfile.close();
        exit(-1);
    }

    string line;

    try {
        getline(meshfile, line);
    }
    catch (ifstream::failure e) {
        cerr << "Problem reading meshfile (getline failed)." << endl;
    }

    if (line.compare("kcply") != 0) {
        cerr << "Problem reading meshfile (not kcply). [" << line << "]" << endl;
        meshfile.close();
        exit(-1);
    }

    try {
        getline(meshfile, line);
    }
    catch (ifstream::failure e) {
        cerr << "Problem reading meshfile (getline failed)." << endl;
        exit(-1);
    }

    istringstream vertex_iss(line);
    string vertex_element;
    string vertex_label;

    vertex_iss >> vertex_element >> vertex_label >> vertex_count;

    if ((vertex_element.compare("element") != 0) || (vertex_label.compare("vertex") != 0)) {
        cerr << "Problem reading meshfile (element vertex)." << endl;
        meshfile.close();
        exit(-1);
    }

    cerr << "Expect " << vertex_count << " vertices." << endl;

    try {
        getline(meshfile, line);
    }
    catch (ifstream::failure e) {
        cerr << "Problem reading meshfile (getline failed)." << endl;
        exit(-1);
    }

    istringstream triangle_iss(line);
    string triangle_element;
    string triangle_label;

    triangle_iss >> triangle_element >> triangle_label >> triangle_count;

    if ((triangle_element.compare("element") != 0) || (triangle_label.compare("face") != 0)) {
        cerr << "Problem reading meshfile (element triangle)." << endl;
        meshfile.close();
        exit(-1);
    }

    cerr << "Expect " << triangle_count << " triangles." << endl;

    vertex = new Vertex[vertex_count];

    triangle = new TriangleIndex[triangle_count];

    face_normal = new Vector[triangle_count];

    for (int i = 0; i < vertex_count; i += 1) {
        try {
            getline(meshfile, line);
        }
        catch (ifstream::failure e) {
            cerr << "Problem reading meshfile (getline failed)." << endl;
            exit(-1);
        }

        vertex_iss.clear();
        vertex_iss.str(line);

        vertex_iss >> vertex[i].x >> vertex[i].y >> vertex[i].z;

        vertex[i].w = 1.0f;
    }

    for (int i = 0; i < triangle_count; i += 1) {
        try {
            getline(meshfile, line);
        }
        catch (ifstream::failure e) {
            cerr << "Problem reading meshfile (getline failed)." << endl;
            exit(-1);
        }

        triangle_iss.clear();
        triangle_iss.str(line);

        triangle_iss >> count >> triangle[i][0] >> triangle[i][1] >> triangle[i][2];

        if (count != 3) {
            cerr << "Problem reading meshfile (non-triangle present)." << endl;
            exit(-1);
        }

        compute_face_normal(i, face_normal[i]);
    }

    if (smooth) {
        vertex_normal = new Vector[vertex_count];
        compute_vertex_normals();
    } else {
        vertex_normal = 0;
    }

    meshfile.close();
    cerr << "Meshfile read." << endl;

    next = 0;
}


// Moller-Trumbore
bool PolyMesh::rayTriangleIntersect(const Ray &ray, Vector v0, Vector v1, Vector v2, float &t, float &u, float &v) {
    Vector p;
    Vector d;
    Vector e1, e2, h, s, q;
    float a, f;

    p.x = ray.position.x;
    p.y = ray.position.y;
    p.z = ray.position.z;
    d = ray.direction;

    e1 = v1 - v0;
    e2 = v2 - v0;

    d.cross(e2, h);
    a = e1.dot(h);

    if (a > -0.00001f && a < 0.00001f) {
        return false;
    }

    f = 1 / a;
    s = p - v0;
    u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    s.cross(e1, q);
    v = f * d.dot(q);

    if ((v < 0.0f) || ((u + v) > 1.0f)) {
        return false;
    }

    // compute t

    t = f * e2.dot(q);

    return true;
}

void PolyMesh::compute_vertex_normals(void) {
    // The vertex_normal array is already zeroed.

    for (int i = 0; i < triangle_count; i += 1) {
        for (int j = 0; j < 3; j += 1) {
            vertex_normal[triangle[i][j]] = vertex_normal[triangle[i][j]] + face_normal[i];
        }
    }

    for (int i = 0; i < vertex_count; i += 1) {
        vertex_normal[i].normalise();
    }
}

void PolyMesh::compute_face_normal(int which_triangle, Vector &normal) {
    Vector v0v1, v0v2;
    v0v1.x = vertex[triangle[which_triangle][1]].x - vertex[triangle[which_triangle][0]].x;
    v0v1.y = vertex[triangle[which_triangle][1]].y - vertex[triangle[which_triangle][0]].y;
    v0v1.z = vertex[triangle[which_triangle][1]].z - vertex[triangle[which_triangle][0]].z;

    v0v1.normalise();

    v0v2.x = vertex[triangle[which_triangle][2]].x - vertex[triangle[which_triangle][0]].x;
    v0v2.y = vertex[triangle[which_triangle][2]].y - vertex[triangle[which_triangle][0]].y;
    v0v2.z = vertex[triangle[which_triangle][2]].z - vertex[triangle[which_triangle][0]].z;

    v0v2.normalise();

    v0v1.cross(v0v2, normal);
    normal.normalise();
}

Hit *PolyMesh::triangle_intersection(Ray ray, int which_triangle) {
    float ndotdir = face_normal[which_triangle].dot(ray.direction);

    if (fabs(ndotdir) < 0.000000001f) {
        // ray is parallel to triangle so does not intersect
        return 0;
    }

    float t, u, v;

    bool flag = rayTriangleIntersect(ray, vertex[triangle[which_triangle][0]], vertex[triangle[which_triangle][1]],
                                     vertex[triangle[which_triangle][2]], t, u, v);

    if (flag == false) return 0;

    Hit *hit = new Hit();

    hit->t = t;
    hit->what = this;
    hit->position = ray.position + t * ray.direction;

    if (smoothing) {
        hit->normal = u * vertex_normal[triangle[which_triangle][1]]
                      + v * vertex_normal[triangle[which_triangle][2]]
                      + (1 - u - v) * vertex_normal[triangle[which_triangle][0]];
    } else {
        hit->normal = face_normal[which_triangle];
    }

    hit->normal.normalise();

    if (hit->normal.dot(ray.direction) > 0.0) {
        hit->normal.negate();
    }

    return hit;
}


Hit *PolyMesh::intersection(Ray ray) {
    Hit *hits = 0;

    int i;

    // Check each triangle, add any hits to sorted list.

    for (i = 0; i < triangle_count; i += 1) {
        Hit *hit = triangle_intersection(ray, i);

        if (hit != 0) {
            if (hits != 0) {
                Hit *step = hits;
                Hit *prev = 0;
                while (step != 0) {
                    if (hit->t < step->t) {
                        // if the new hit is in front of the current step, it inserts before it.
                        hit->next = step;
                        if (prev != 0) {
                            prev->next = hit;
                        } else {
                            hits = hit;
                        }
                        break;
                    }

                    prev = step;
                    step = step->next;
                }

                if (step == 0) {
                    // hit if bigger than step, insert it afterwards
                    prev->next = hit;
                    hit->next = 0;
                }
            } else {
                hit->next = 0;
                hits = hit;
            }
        }
    }

    Hit *temp = hits;
    bool entering = true;

    while (temp != 0) {
        temp->entering = entering;
        entering = !entering;
        temp = temp->next;
    }

    return hits;
}

void PolyMesh::apply_transform(Transform &trans) {
    for (int i = 0; i < vertex_count; i += 1) {
        trans.apply(vertex[i]);
    }

    Transform ti = trans.inverse().transpose();

    if (smoothing) {

        for (int i = 0; i < vertex_count; i += 1) {
            ti.apply(vertex_normal[i]);
        }
    }

    for (int i = 0; i < triangle_count; i += 1) {
        ti.apply(face_normal[i]);
    }
}