//#include "photon_map.h"
//
//PhotonMap::PhotonMap(Object *p_objects, Light * p_lights) {
//    objects = p_objects;
//    lights = p_lights;
//}
//
//void PhotonMap::build_kd_tree(vector<double> &points, kdtree &tree, vector<long> &tags) {
//    real_2d_array matrix;
//    matrix.attach_to_ptr(points.size() / 3, 3, points.data());
//    ae_int_t nx = 3;
//    ae_int_t ny = 0;
//    ae_int_t normtype = 2;
//    real_1d_array x;
//    integer_1d_array input;
//    // TODO: Need to figure out this error
//    input.setcontent(points.size() / 3, tags.data());
//    kdtreebuildtagged(matrix, input, nx, ny, normtype, tree);
//}
//
//// estimate radiance for a given point
//Colour PhotonMap::estimate_radiance(Ray &ray, Hit &hit, kdtree &tree, vector<Photon> &photons, vector<Photon *> local_photons) {
//    Colour colour = Colour(0.0f, 0.0f, 0.0f);
//    Material *hit_material = hit.what->material;
//
//    // find max distance
//    float max_dist;
//    for (Photon *p: local_photons) {
//        float dist = (p->path.position - hit.position).len_sqr();
//        if (dist > max_dist)
//            max_dist = dist;
//    }
//
//    for (Photon *p: local_photons) {
//        float dist = (p->path.position - hit.position).len_sqr();
//
//        // TODO: In example base colour I believe is the equivalent to the phong ambient, need to figure out how to return
//        //  this for global material and compound materials - can't do a standard ray trace I don't think??
//        Colour base_colour = ??
//
//        // gaussian filtering per Henrik Jensen's recommendations.
//        float alpha = 0.918;
//        float beta = 1.953;
//        float gaussian = alpha * (1 - (1 - exp(-beta * ((dist * dist) / (2 * max_dist * max_dist)))) / (1 - exp(-beta)));
//
//        // calculate photon contribution based on BRDF and photon intensity
//        // TODO: I believe this is correct but obviously need to fix once above is fixed
//        colour += p->intensity * hit_material->compute_per_light(ray.direction, hit, p->path.direction) * gaussian;
//    }
//    // divide through by max volume of disc sampled within
//    colour = colour * (1 / ((float) M_PI * max_dist * max_dist));
//    return colour;
//}
//
//
//// gather k nearest neighbour photons
//vector<Photon *> PhotonMap::gather_photons(Vertex p, int k, kdtree &tree, vector<Photon> &photons) {
//    // boilerplate code to query kd tree from ALGLIB
//    vector<double> point = {p.x, p.y, p.z};
//    real_1d_array x;
//    x.setcontent(3, point.data());
//    kdtreequeryknn(tree, x, k);
//    real_2d_array r = "[[]]";
//    integer_1d_array output_tags = "[]";
//    kdtreequeryresultstags(tree, output_tags);
//
//    vector<Photon *> local_photons;
//    for (int i = 0; i < k; i++) {
//        local_photons.push_back(&photons[output_tags[i]]);
//    }
//    return local_photons;
//}
//
//void PhotonMap::store_photon(Photon p, vector<double> &points, vector<Photon> &photons, vector<long> &tags) {
//    p.ray.direction.negate();
//    photons.push_back(p);
//    // finding the index of the photon, and then pushing to array so it can be accessed later
//    tags.push_back(points.size() / 3);
//
//    // store points individually due to limitation with ALGLIB
//    points.push_back(p.ray.position.x);
//    points.push_back(p.ray.position.y);
//    points.push_back(p.ray.position.z);
//}
