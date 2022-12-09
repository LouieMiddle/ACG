#pragma once;

#include "object.h";
#include "hit.h";
#include "ray.h";
#include <random>

class utils {
public:
    static Hit *merge_sort_hits(Hit *head);
    static Hit *merge_hits(Hit *head1, Hit *head2);
    static Hit *mid_point_hits(Hit *head);

    static void append_hits(Hit **head_ref, Hit *new_data);

    static float get_random_number(float min, float max);

    static Vector get_random_direction();

    static Vector get_random_direction_in_range(Vector direction, float theta);

    // Trace a ray through the scene and find the closest if any object
    // intersection in front of the ray.
    static Hit *trace(Ray ray, Object *object_list);

    // raytrace a shadow ray.
    static bool shadow_trace(Ray, Object *object_list, float limit);

    // Filter the list of returned hits to the closest +ve
    static Hit *select_first(Hit *list);
};
