#include "photon_map.h"

PhotonMap::PhotonMap(Object *p_objects, Light * p_lights) {
    object_list = p_objects;
    light_list = p_lights;
}

// emit photons from light sources
void PhotonMap::emit_photons(int number_photons, int recurse, PhotonMode mode) {
    if (mode == PHOTON_SHADOW) {
        printf("\n");
        printf("warn msg: %s\n", "Only emit normal or caustic photons (shadows handled at later step)");
        return;
    }

    Light *light = this->light_list;
    while (light != 0) {
        for (int i = 0; i < number_photons; i++) {
            if (mode == PHOTON_NORMAL) {
                // TODO: Currently this will only work for point lights
                Vector direction = Utils::get_random_direction();
                Ray photon_path = Ray(light->position, direction);
                Photon photon = Photon(photon_path, light->intensity, mode);
                this->trace_photon(photon, recurse, number_photons);
            } else if (mode == PHOTON_CAUSTIC) {
                Object *objects = this->object_list;
                while (objects != 0) {
                    if (!objects->material->transmissive) {
                        objects = objects->next;
                        continue;
                    }

                    Vector to_object = objects->centre - light->position;
                    float dist = to_object.length();
                    to_object.normalise();
                    float cone_angle = atan(objects->radius * (1 / dist));
                    Vector direction = Utils::get_random_direction_in_range(to_object, cone_angle);
                    Ray path = Ray(light->position, direction);
                    Photon caustic_photon = Photon(path, light->intensity, mode);
                    this->trace_photon(caustic_photon, recurse, number_photons);

                    objects = objects->next;
                }
            }
        }
        light = light->next;
    }

    this->build_kd_tree();
}

void PhotonMap::trace_photon(Photon photon, int recurse, int number_photons) {
    if (recurse < 1) return;

    Hit *best_hit = Utils::trace(photon.path, this->object_list);
    if (best_hit != 0) {
        photon.path.position = best_hit->position;

        Material *material = best_hit->what->material;

        if (material->probability_diffuse != 0.0f) {
            this->store_photon(photon, number_photons);
        }

//        if (photon.mode != PHOTON_CAUSTIC) {
//            this->emit_shadow_photon(photon);
//        }

        // Russian Roulette
        float probability = Utils::get_random_number(0, 1);
        if (probability <= material->probability_diffuse) {
            if (photon.mode == PHOTON_CAUSTIC) {
                delete best_hit;
                return;
            }

            // Diffuse reflection
            photon.path.direction = Utils::get_random_direction_in_range(best_hit->normal, M_PI);
            photon.intensity = material->compute_once(photon.path, *best_hit, 1) * (1 / material->probability_diffuse);
        } else if (probability <= material->probability_diffuse + material->probability_specular) {
            // Specular reflection
            Utils::reflect_ray(photon.path.direction, best_hit->normal, photon.path.direction);
            photon.intensity = material->compute_once(photon.path, *best_hit, 1) * (1 / material->probability_specular);
        } else if (probability <= material->probability_diffuse + material->probability_specular + material->probability_transmissive) {
            if (photon.mode != PHOTON_CAUSTIC) {
                delete best_hit;
                return;
            }

            // Transmission (refraction)
            Utils::refract_ray(photon.path.direction, best_hit->normal, photon.path.direction, material->ior);
            photon.path.direction.normalise();

            float cos_i = photon.path.direction.dot(best_hit->normal);
            Vector bias = 0.0001f * best_hit->normal;
            photon.path.position = cos_i < 0.0f ? best_hit->position - bias : best_hit->position + bias;
            photon.intensity = photon.intensity * (1 / material->probability_transmissive);

//            photon.intensity = material->compute_once(photon.path, *best_hit, 1) * (1 / material->probability_transmissive);
//            printf("here");
        } else {
            // If none of the conditions met then the photon is absorbed
            delete best_hit;
            return;
        }
        delete best_hit;
        this->trace_photon(photon, recurse - 1, number_photons);
    }
}

void PhotonMap::emit_shadow_photon(Photon photon) {
    Object *objects = object_list;

    while(objects != 0) {
        Hit *shadow_hit = objects->intersection(photon.path);
        if (shadow_hit != 0) {
            Photon shadow_photon = Photon(Ray(shadow_hit->position, photon.path.direction), Colour(0.0f, 0.0f, 0.0f), PHOTON_SHADOW);
            this->store_photon(shadow_photon, 1);
        }
        delete shadow_hit;
        objects = objects->next;
    }
}

void PhotonMap::build_kd_tree() {
    try {
        real_2d_array matrix;
        matrix.attach_to_ptr(this->points.size() / 3, 3, this->points.data());
        ae_int_t nx = 3;
        ae_int_t ny = 0;
        ae_int_t normtype = 2;
        real_1d_array x;
        integer_1d_array input;
        input.setcontent(this->points.size() / 3, this->photon_indexes.data());
        kdtreebuildtagged(matrix, input, nx, ny, normtype, this->tree);
    } catch(alglib::ap_error &e) {
        printf("\n");
        printf("error msg: %s\n", e.msg.c_str());
    }
}

// gather nearest neighbours at position
void PhotonMap::gather_photons(Vertex position, int neighbours, vector<Photon *> &local_photons) {
    // Check photons aren't empty in case of standard ray trace
    if (!this->photons.empty()) {
        vector<double> point = {position.x, position.y, position.z};
        real_1d_array point_array;
        point_array.setcontent(3, point.data());
        kdtreequeryknn(this->tree, point_array, neighbours);
        integer_1d_array local_photon_indexes = "[]";
        kdtreequeryresultstags(this->tree, local_photon_indexes);

        for (int i = 0; i < neighbours; i++) {
            local_photons.push_back(&this->photons[local_photon_indexes[i]]);
        }
    }
}

void PhotonMap::store_photon(Photon photon, int number_photons) {
    photon.intensity = photon.intensity * (1.0f / (float) number_photons);
    photon.path.direction.negate();
    this->photons.push_back(photon);
    this->photon_indexes.push_back(points.size() / 3);

    // store points individually due to ALGLIB implementation
    this->points.push_back(photon.path.position.x);
    this->points.push_back(photon.path.position.y);
    this->points.push_back(photon.path.position.z);
}
