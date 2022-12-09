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

/* CSG is an object that is built by Constructive Solid Geometry from two sub-object_list.*/

using namespace std;

#include <string.h>
#include "csg_object.h"

CSG::CSG(CSGMode p_mode, Object *p_left, Object *p_right) {
    mode = p_mode;
    left = p_left;
    right = p_right;
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

bool includes_csg(CSG *parent, Object *child) {
    if (parent->left == child) {
        return true;
    }
    if (parent->right == child) {
        return true;
    }

    bool result = false;
    if (strcmp(typeid(*parent->left).name(), CSG_TYPE) == 0) {
        result = includes_csg(dynamic_cast<CSG*>(parent->left), child);
    }
    if (strcmp(typeid(*parent->right).name(), CSG_TYPE) == 0 && !result) {
        result = includes_csg(dynamic_cast<CSG*>(parent->right), child);
    }

    return result;
}

bool includes(Object *parent, Object *child) {
    if (parent == child) {
        return true;
    }

    if (strcmp(typeid(*parent).name(), CSG_TYPE) == 0) {
        return includes_csg(dynamic_cast<CSG*>(parent), child);
    }

    return false;
}

Hit *CSG::filter_hits(Hit *hits) {
    bool inl = false;
    bool inr = false;

    Hit *result = 0;

    while (hits != 0) {
        bool l_hit = includes(left, hits->what);

        if (operation_allowed(l_hit, inl, inr)) {
            if (result == 0) {
                result = hits;
                hits = hits->next;
                result->next = 0;
            } else {
                Hit *temp = hits;
                hits = hits->next;
                temp->next = 0;
                utils::append_hits(&result, temp);
            }
        } else {
            Hit *temp = hits;
            hits = hits->next;
            delete temp;
        }

        if (l_hit) {
            inl = !inl;
        } else {
            inr = !inr;
        }
    }

    return result;
}

Hit *CSG::intersection(Ray ray) {
    Hit *hits = left->intersection(ray);
    utils::append_hits(&hits, right->intersection(ray));
    hits = utils::merge_sort_hits(hits);
    return filter_hits(hits);
}

void CSG::apply_transform(Transform &transform) {
    left->apply_transform(transform);
    right->apply_transform(transform);
}
