#pragma once;

#include "hit.h";
#include <random>

class utils {
public:
    // TODO: Cite https://codeforgeek.com/sort-linked-lists-cpp/
    static Hit *merge_sort_hits(Hit *head);
    static Hit *merge_hits(Hit *head1, Hit *head2);
    static Hit *mid_point_hits(Hit *head);

    // TODO: Cite https://www.geeksforgeeks.org/cpp-program-for-inserting-a-node-in-a-linked-list/
    static void append_hits(Hit **head_ref, Hit *new_data);

    static float get_random_number(float min, float max);

    static Vector get_random_direction();

    static Vector get_random_direction_in_range(Vector direction, float theta);
};
