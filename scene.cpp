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

void Scene::raytrace(Ray ray, int recurse, Colour &colour, float &depth) {
    // first step, find the closest primitive
    Hit *best_hit = Utils::trace(ray, this->object_list);

    // if we found a primitive then compute the colour we should see
    if (best_hit != 0) {
        int neighbours = 500;
        vector<Photon *> local_photons;
        if (this->photon_map != 0) {
            this->photon_map->gather_photons(best_hit->position, neighbours, local_photons);
            colour += this->estimate_radiance(ray, *best_hit, local_photons);

            if (this->caustic_photon_map != 0) {
                int caustic_neighbours = 100;
                vector<Photon *> caustic_local_photons;
                this->caustic_photon_map->gather_photons(best_hit->position, caustic_neighbours,  caustic_local_photons);
                colour += this->estimate_radiance(ray, *best_hit, caustic_local_photons);
            }
        } else {
            colour += best_hit->what->material->compute_once(ray, *best_hit,
                                                             recurse);
        }

        depth = best_hit->t;

        // next, compute the light contribution for each light in the scene.
        Light *light = light_list;
        while (light != (Light *) 0) {
            Vector viewer;
            Vector l_dir;

            viewer = ray.position - best_hit->position;
            viewer.normalise();

            bool lit;
            float dist;
            lit = light->get_direction(best_hit->position, l_dir, dist);

            if (l_dir.dot(best_hit->normal) > 0) {
                lit = false; //light is facing wrong way.
            }

            // Put the shadow check here, if lit==true and in shadow, set lit=false

            if (lit && in_shadow(neighbours, local_photons)) {
                Ray shadow_ray;

                shadow_ray.direction = -l_dir;

                shadow_ray.position = best_hit->position + (0.0001f * shadow_ray.direction);

                if (Utils::shadow_trace(shadow_ray, this->object_list, dist)) {
                    lit = false; //there's a shadow so no lighting, if realistically close
                }
            }

            if (lit) {
                Colour intensity;

                light->get_intensity(best_hit->position, intensity);

                if (this->photon_map != 0) {
                    colour += colour * best_hit->what->material->compute_per_light(viewer, *best_hit, l_dir);
                } else {
                    colour += intensity * best_hit->what->material->compute_per_light(viewer, *best_hit,
                                                                                              l_dir); // this is the per light local contrib e.g. diffuse, specular
                }
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

bool Scene::in_shadow(int neighbours, const vector<Photon *> &local_photons) {
    if (this->photon_map == 0) {
        return true;
    }

    int counter = 0;
    for (Photon *p : local_photons) {
        if (p->mode == PHOTON_SHADOW)
            counter++;
    }
    if (counter >= (neighbours / 2)) {
        return true;
    }
//    return false;
    // TODO: Currently getting segmentation faults with shadow photons, once working reset to return false
    return true;
}

// estimate radiance for a given point
Colour Scene::estimate_radiance(Ray &ray, Hit &hit, const vector<Photon *> &local_photons) {
    Colour radiance = Colour(0.0f, 0.0f, 0.0f);
    float max_dist;
    for (Photon *photon: local_photons) {
        float dist = (photon->path.position - hit.position).length();
        if (dist > max_dist)
            max_dist = dist;
    }

    for (Photon *photon: local_photons) {
        float dist = (photon->path.position - hit.position).length();

        // gaussian filtering
        float alpha = 0.918;
        float beta = 1.953;
        float gaussian = alpha * (1 - (1 - exp(-beta * ((dist * dist) / (2 * max_dist * max_dist)))) / (1 - exp(-beta)));

        radiance += photon->intensity * (hit.what->material->compute_once(ray, hit, 1) + hit.what->material->compute_per_light(ray.direction, hit, photon->path.direction)) * gaussian;
    }
    // scale by area sampled
    radiance = radiance * (1 / ((float) M_PI * max_dist * max_dist));
    return radiance;
}

void Scene::add_object(Object *obj) {
    obj->next = this->object_list;
    this->object_list = obj;
}

void Scene::add_light(Light *light) {
    light->next = this->light_list;
    this->light_list = light;
}

void Scene::set_photon_map(PhotonMap *p_photon_map) {
    photon_map = p_photon_map;
}

void Scene::set_caustic_photon_map(PhotonMap *p_caustic_photon_map) {
    caustic_photon_map = p_caustic_photon_map;
}
