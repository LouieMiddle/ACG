#include "photon_map.h"

PhotonMap::PhotonMap(Object *p_objects, Light * p_lights) {
    objects = p_objects;
    lights = p_lights;
}

void PhotonMap::build_kd_tree(vector<double> &points, kdtree &tree, vector<long long> &tags) {
    try {
        real_2d_array matrix;
        matrix.attach_to_ptr(points.size() / 3, 3, points.data());
        ae_int_t nx = 3;
        ae_int_t ny = 0;
        ae_int_t normtype = 2;
        real_1d_array x;
        integer_1d_array input;
        input.setcontent(points.size() / 3, tags.data());
        kdtreebuildtagged(matrix, input, nx, ny, normtype, tree);
    } catch(alglib::ap_error &e) {
        printf("\n");
        printf("error msg: %s\n", e.msg.c_str());
    }
}

// gather nearest neighbours at position
void PhotonMap::gather_photons(Vertex position, int neighbours, kdtree &tree, vector<Photon> &photons, vector<Photon *> &local_photons) {
    vector<double> point = {position.x, position.y, position.z};
    real_1d_array point_array;
    point_array.setcontent(3, point.data());
    kdtreequeryknn(tree, point_array, neighbours);
    integer_1d_array output_tags = "[]";
    kdtreequeryresultstags(tree, output_tags);

    for (int i = 0; i < neighbours; i++) {
        local_photons.push_back(&photons[output_tags[i]]);
    }
}

// estimate radiance for a given point
void PhotonMap::estimate_radiance(Hit &hit, const vector<Photon *> &local_photons, Colour &radiance) {
    float max_dist;
    for (Photon *p: local_photons) {
        float dist = (p->path.position - hit.position).length();
        if (dist > max_dist)
            max_dist = dist;
    }

    for (Photon *p: local_photons) {
        float dist = (p->path.position - hit.position).len_sqr();

        radiance += hit.what->material->compute_ambient();

        // gaussian filtering
        float alpha = 0.918;
        float beta = 1.953;
        float gaussian = alpha * (1 - (1 - exp(-beta * ((dist * dist) / (2 * max_dist * max_dist)))) / (1 - exp(-beta)));

        radiance += p->intensity * hit.what->material->compute_diffuse(hit, p->path.direction) * gaussian;
    }
    // scale by area sampled
    radiance = radiance * (1 / ((float) M_PI * max_dist * max_dist));
}

void PhotonMap::store_photon(Photon p, vector<double> &points, vector<Photon> &photons, vector<long long> &tags) {
    p.path.direction.negate();
    photons.push_back(p);
    tags.push_back(points.size() / 3);

    // store points individually due to ALGLIB implementation
    points.push_back(p.path.position.x);
    points.push_back(p.path.position.y);
    points.push_back(p.path.position.z);
}
