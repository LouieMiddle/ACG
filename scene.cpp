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

#include "scene.h"

Scene::Scene() {
    object_list = 0;
    light_list = 0;
}

bool Scene::shadowtrace(Ray ray, float limit) {
    Object *objects = this->object_list;

    while (objects != 0) {
        Hit *hit = this->select_first(objects->intersection(ray));

        if (hit != 0) {
            if ((hit->t > 0.00000001f) && (hit->t < limit)) {
                delete hit;
                return true;
            }
            delete hit;
        }

        objects = objects->next;
    }

    return false;
}

Hit *Scene::trace(Ray ray) {
    Hit *best_hit = 0;

    Object *objects = this->object_list;

    while (objects != 0) {
        Hit *hit = this->select_first(objects->intersection(ray));

        if (hit != 0) {
            if (best_hit == 0) {
                best_hit = hit;
            } else if (hit->t < best_hit->t) {
                delete best_hit;
                best_hit = hit;
            } else {
                delete hit;
            }
        }

        objects = objects->next;
    }

    return best_hit;
}

Hit *Scene::select_first(Hit *list) {
    Hit *result = 0;

    while (list != 0) {
        if (list->t >= 0.0f) {
            result = list;
            list = list->next;
            break;
        }

        Hit *temp = list;
        list = list->next;
        delete temp;
    }

    while (list != 0) {
        Hit *temp = list;
        list = list->next;
        delete temp;
    }

    return result;
}

void Scene::raytrace(Ray ray, int recurse, Colour &colour, float &depth) {
    // first step, find the closest primitive
    Hit *best_hit = this->trace(ray);

    // if we found a primitive then compute the colour we should see
    if (best_hit != 0) {
        int neighbours = 700;
        vector<Photon *> local_photons = photon_map->gather_photons(best_hit->position, neighbours, tree, photons);

        depth = best_hit->t;

        colour = colour + best_hit->what->material->compute_once(ray, *best_hit,
                                                                 recurse); // this will be the global components such as ambient or reflect/refract

        colour = colour + photon_map->estimate_radiance(*best_hit, local_photons);

        // next, compute the light contribution for each light in the scene.
        Light *light = light_list;
        while (light != (Light *) 0) {
            Vector viewer;
            Vector ldir;

            viewer = ray.position - best_hit->position;
            viewer.normalise();

            bool lit;
            float dist;
            lit = light->get_direction(best_hit->position, ldir, dist);

            if (ldir.dot(best_hit->normal) > 0) {
                lit = false; //light is facing wrong way.
            }

            // Put the shadow check here, if lit==true and in shadow, set lit=false

            if (lit) {
                Ray shadow_ray;

                shadow_ray.direction = -ldir;

                shadow_ray.position = best_hit->position + (0.0001f * shadow_ray.direction);

                if (this->shadowtrace(shadow_ray, dist)) {
                    lit = false; //there's a shadow so no lighting, if realistically close
                }
            }

            if (lit) {
                Colour intensity;

                light->get_intensity(best_hit->position, intensity);

                colour = colour + intensity * best_hit->what->material->compute_per_light(viewer, *best_hit,
                                                                                          ldir); // this is the per light local contrib e.g. diffuse, specular
            }

            light = light->next;
        }

        delete best_hit;
    } else {
        colour.r = 0.0f;
        colour.g = 0.0f;
        colour.b = 0.0f;
        colour.a = 1.0f;
    }
}

// TODO: Add to utils
// generate random float between two values
float get_random_number(float min, float max) {
    random_device device;
    mt19937 random(device());
    uniform_real_distribution<> distribution(min, max);
    auto n = distribution(random);
    return n;
}

// TODO: Add to utils class
Vector get_random_direction() {
    float x = get_random_number(-1.0f, 1.0f);
    float y = get_random_number(-1.0f, 1.0f);
    float z = get_random_number(-1.0f, 1.0f);

    Vector direction = Vector(x, y, z);
    direction.normalise();

    return direction;
}

// generate random direction in direction within cone (defined by size of theta)
Vector random_direction(Vector direction, float theta) {
    // make basis vectors based on direction
    Vector basis_z = direction;
    basis_z.normalise();

    Vector basis_x;
    Vector different = (0.5 > fabs(basis_z.x)) ? Vector(1, 0, 0) : Vector(0, 1, 0);
    basis_z.cross(different, basis_x);
    basis_x.normalise();

    Vector basis_y;
    basis_x.cross(basis_z, basis_y);

    // height
    float z = get_random_number(cos(theta), 1);

    // rotation
    float phi = get_random_number(-M_PI, +M_PI);
    float x = sqrt(1 - z * z) * cos(phi);
    float y = sqrt(1 - z * z) * sin(phi);

    // combine
    Vector result = x * basis_x + y * basis_y + z * basis_z;
    result.normalise();
    return result;
}

void Scene::trace_photon(Photon photon, int recurse, vector<double> &points, vector<Photon> &photons, vector<long long> &tags) {
    if (recurse < 1) return;

    Hit *best_hit = trace(photon.path);
    if (best_hit != 0) {
        photon.path.position = best_hit->position;

        Material *m = best_hit->what->material;
        photon_map->store_photon(photon, points, photons, tags);

        // Russian Roulette
        float probability = get_random_number(0, 1);
        if (probability <= m->kd) {
            // DIFFUSE REFLECTION
            photon.path.direction = random_direction(best_hit->normal, M_PI);
            photon.intensity = m->compute_ambient() * (1 / m->kd);
            trace_photon(photon, recurse - 1, points, photons, tags);
        } else if (probability <= m->kd + m->ks) {
            // SPECULAR REFLECTION
            best_hit->normal.reflection(photon.path.direction, photon.path.direction);
            photon.intensity = m->compute_ambient() * (1 / m->ks);
            trace_photon(photon, recurse - 1, points, photons, tags);
        } else {
            // PHOTON ABSORBED
            return;
        }
    }
}

// emit photons from light sources
void Scene::emit(int number_photons, int recurse, vector<double> &points, vector<Photon> &photons, vector<long long> &tags) {
    Light *light = light_list;
    while (light != 0) {
        for (int i = 0; i < number_photons; i++) {
            Vector direction = get_random_direction();
            Ray photon_path = Ray(light->position, direction);
            Photon photon = Photon(photon_path, light->intensity, PHOTON_NORMAL);
            trace_photon(photon, recurse, points, photons, tags);
        }
        for (Photon p: photons) {
            // scale by number of photons from light
            p.intensity = p.intensity * (1 / number_photons);
        }
        light = light->next;
    }
}

void Scene::add_object(Object *obj) {
    obj->next = this->object_list;
    this->object_list = obj;
}

void Scene::add_light(Light *light) {
    light->next = this->light_list;
    this->light_list = light;
}

void Scene::set_photon_map() {
    photon_map = new PhotonMap(object_list, light_list);
    emit(50000, 50, points, photons, tags);
    photon_map->build_kd_tree(points, tree, tags);
}
