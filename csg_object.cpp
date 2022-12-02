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

#include <string.h>
#include "csg_object.h"

Hit *merge(Hit *head1, Hit *head2) {
    if (head1 == 0)
        return head2;
    if (head2 == 0)
        return head1;

    Hit *temp;
    if (head1->t < head2->t) {
        temp = head1;
        temp->next = merge(head1->next, head2);
    } else {
        temp = head2;
        temp->next = merge(head1, head2->next);
    }

    return temp;
}

Hit *mid_point(Hit *head) {
    if (head == 0 || head->next == 0)
        return head;

    Hit *fast = head;
    Hit *slow = head;

    while (fast != 0 && fast->next != 0) {
        fast = fast->next;

        if (fast->next == 0)
            break;

        fast = fast->next;
        slow = slow->next;
    }

    return slow;
}

// TODO: Cite https://codeforgeek.com/sort-linked-lists-cpp/
// TODO: Add to utils class
Hit *merge_sort(Hit *head) {
    if (head == 0 || head->next == 0)
        return head;

    Hit *mid = mid_point(head);
    Hit *a = head;
    Hit *b = mid->next;

    mid->next = 0;

    a = merge_sort(a);
    b = merge_sort(b);

    Hit *c = merge(a, b);

    return c;
}

// TODO: Cite https://www.geeksforgeeks.org/cpp-program-for-inserting-a-node-in-a-linked-list/
// TODO: Add to utils class
void append(Hit **head_ref, Hit *new_data) {
    Hit *last = *head_ref;

    if (*head_ref == 0) {
        *head_ref = new_data;
        return;
    }

    while (last->next != 0)
        last = last->next;

    last->next = new_data;
}

CSG::CSG(Mode p_mode, Object *p_left, Object *p_right) {
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
                append(&result, temp);
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

// TODO: Add to utils
void printlist(Hit *head) {
    while (head != 0) {
        cout << head->t << " ";
        head = head->next;
    }
    cout << endl;
}

Hit *CSG::intersection(Ray ray) {
    Hit *hits = left->intersection(ray);
    append(&hits, right->intersection(ray));

//    cout << "Given Linked List is " << endl;
//    printlist(hits);

    hits = merge_sort(hits);

//    cout << "Sorted Linked List is " << endl;
//    printlist(hits);

//    Hit *tmp = hits;
//    int count = 0;
//    while (tmp !=  0) {
//        tmp = tmp->next;
//        count++;
//    }
//    if (count > 100) {
//        printf("here");
//    }

    return filter_hits(hits);
}

void CSG::apply_transform(Transform &transform) {
    left->apply_transform(transform);
    right->apply_transform(transform);
}
