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

/* CSG is an object that is built by Constructive Solid Geometry from two sub-objects.*/

using namespace std;

#include "csg_object.h"

// TODO: Cite https://www.geeksforgeeks.org/merge-k-sorted-linked-lists/
// Takes two lists of hits sorted in increasing order, and merge their hits together to make one big sorted hit.
Hit *SortedMerge(Hit *a, Hit *b) {
    Hit *result = 0;

    /* Base cases */
    if (a == 0)
        return (b);
    else if (b == 0)
        return (a);

    /* Pick either a or b, and recur */
    if (a->t <= b->t) {
        result = a;
        result->next = SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }

    return result;
}

// TODO: Cite https://www.geeksforgeeks.org/merge-k-sorted-linked-lists/
// Takes an array of lists and generates sorted output of hits
Hit *mergeKLists(Hit *arr[], int last) {
    // repeat until only one list is left
    while (last != 0) {
        int i = 0, j = last;

        // (i, j) forms a pair
        while (i < j) {
            // merge List i with List j and store
            // merged list in List i
            arr[i] = SortedMerge(arr[i], arr[j]);

            // consider next pair
            i++, j--;

            // If all pairs are merged, update last
            if (i >= j)
                last = j;
        }
    }

    return arr[0];
}

CSG::CSG(Mode p_mode, Object *p_left, Object *p_right) {
    mode = p_mode;
    left = p_left;
    right = p_right;
    left->parent = this;
    right->parent = this;
}

bool CSG::operation_allowed(bool lhit, bool inl, bool inr) {
    if (mode == CSG_UNION) {
        return (lhit && !inr) || (!lhit && !inl);
    } else if (mode == CSG_INTER) {
        return (lhit && inr) || (!lhit && inl);
    } else if (mode == CSG_DIFF) {
        return (lhit && !inr) || (!lhit && inl);
    }

    return false;
}

Hit *CSG::filter_hits(Hit *hits) {
    bool inl = false;
    bool inr = false;

    Hit *tmp = 0;

    while (hits != 0) {
        bool lhit = hits->what == left;

        if (operation_allowed(lhit, inl, inr)) {
            if (tmp == 0) {
                tmp = hits;
            } else {
                tmp->next = hits;
            }
        }

        if (lhit) {
            inl = !inl;
        } else {
            inr = !inr;
        }

        hits = hits->next;
    }

    return tmp;
}

Hit *CSG::intersection(Ray ray) {
    Hit *hits[2];
    hits[0] = left->intersection(ray);
    hits[1] = right->intersection(ray);
    Hit *first_hit = mergeKLists(hits, 1);

    return filter_hits(first_hit);
}

void CSG::apply_transform(Transform &transform) {
}
