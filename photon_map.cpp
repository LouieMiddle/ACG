#include "photon_map.h"

PhotonMap::PhotonMap(Object *p_objects, Light * p_lights) {
    object_list = p_objects;
    light_list = p_lights;
}

// emit photons from light sources
void PhotonMap::emit_photons(int number_photons, int recurse) {
    Light *light = light_list;
    while (light != 0) {
        for (int i = 0; i < number_photons; i++) {
            // TODO: Currently this will only work for point lights
            Vector direction = utils::get_random_direction();
            Ray photon_path = Ray(light->position, direction);
            Photon photon = Photon(photon_path, light->intensity, PHOTON_NORMAL);
            this->trace_photon(photon, recurse);
        }
        for (Photon p: photons) {
            // scale by number of photons from light
            p.intensity = p.intensity * (float) (1 / number_photons);
        }
        light = light->next;
    }

    this->build_kd_tree();
}

void PhotonMap::trace_photon(Photon photon, int recurse) {
    if (recurse < 1) return;

    Hit *best_hit = utils::trace(photon.path, this->object_list);
    if (best_hit != 0) {
        photon.path.position = best_hit->position;

        Material *m = best_hit->what->material;
        this->store_photon(photon);

        // Russian Roulette
        float probability = utils::get_random_number(0, 1);
        if (probability <= m->kd) {
            // Diffuse reflection
            photon.path.direction = utils::get_random_direction_in_range(best_hit->normal, M_PI);
            photon.intensity = m->compute_ambient() * (1 / m->kd);
            this->trace_photon(photon, recurse - 1);
        } else if (probability <= m->kd + m->ks) {
            // Specular reflection
            best_hit->normal.reflection(photon.path.direction, photon.path.direction);
            photon.intensity = m->compute_ambient() * (1 / m->ks);
            this->trace_photon(photon, recurse - 1);
        }
        // If none of the conditions met then the photon is absorbed
    }
    delete best_hit;
}

void PhotonMap::build_kd_tree() {
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
void PhotonMap::gather_photons(Vertex position, int neighbours, vector<Photon *> &local_photons) {
    // Check photons aren't empty in case of standard ray trace
    if (photons.size() != 0) {
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
}

void PhotonMap::store_photon(Photon photon) {
    photon.path.direction.negate();
    photons.push_back(photon);
    tags.push_back(points.size() / 3);

    // store points individually due to ALGLIB implementation
    points.push_back(photon.path.position.x);
    points.push_back(photon.path.position.y);
    points.push_back(photon.path.position.z);
}
