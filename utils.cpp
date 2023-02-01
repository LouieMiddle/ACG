#include "utils.h"

// Mostly a copy of https://codeforgeek.com/sort-linked-lists-cpp/ as Hit lists are basically singly linked lists
Hit *Utils::merge_hits(Hit *head1, Hit *head2) {
    if (head1 == 0)
        return head2;
    if (head2 == 0)
        return head1;

    Hit *temp;
    if (head1->t < head2->t) {
        temp = head1;
        temp->next = merge_hits(head1->next, head2);
    } else {
        temp = head2;
        temp->next = merge_hits(head1, head2->next);
    }

    return temp;
}

// Mostly a copy of https://codeforgeek.com/sort-linked-lists-cpp/ as Hit lists are basically singly linked lists
Hit *Utils::mid_point_hits(Hit *head) {
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

// Mostly a copy of https://codeforgeek.com/sort-linked-lists-cpp/ as Hit lists are basically singly linked lists
Hit *Utils::merge_sort_hits(Hit *head) {
    if (head == 0 || head->next == 0)
        return head;

    Hit *mid = mid_point_hits(head);
    Hit *a = head;
    Hit *b = mid->next;

    mid->next = 0;

    a = merge_sort_hits(a);
    b = merge_sort_hits(b);

    Hit *c = merge_hits(a, b);

    return c;
}

// Mostly a copy of https://www.geeksforgeeks.org/cpp-program-for-inserting-a-node-in-a-linked-list/ as Hit lists are basically singly linked lists
void Utils::append_hits(Hit **head_ref, Hit *new_data) {
    Hit *last = *head_ref;

    if (*head_ref == 0) {
        *head_ref = new_data;
        return;
    }

    while (last->next != 0)
        last = last->next;

    last->next = new_data;
}

float Utils::get_random_number(float min, float max) {
    random_device device;
    mt19937 random(device());
    uniform_real_distribution<> distribution(min, max);
    auto n = distribution(random);
    return n;
}

Vector Utils::get_random_direction() {
    float x = get_random_number(-1.0f, 1.0f);
    float y = get_random_number(-1.0f, 1.0f);
    float z = get_random_number(-1.0f, 1.0f);

    Vector direction = Vector(x, y, z);
    direction.normalise();

    return direction;
}

// generate random direction in direction within range defined by theta
Vector Utils::get_random_direction_in_range(Vector direction, float theta) {
    Vector basis_z = direction;
    basis_z.normalise();

    Vector basis_x;
    Vector different = (0.5 > fabs(basis_z.x)) ? Vector(1, 0, 0) : Vector(0, 1, 0);
    basis_z.cross(different, basis_x);
    basis_x.normalise();

    Vector basis_y;
    basis_x.cross(basis_z, basis_y);

    // height
    float z = Utils::get_random_number(cos(theta), 1);

    // rotation
    float phi = Utils::get_random_number(-M_PI, +M_PI);
    float x = sqrt(1 - z * z) * cos(phi);
    float y = sqrt(1 - z * z) * sin(phi);

    // combine
    Vector result = x * basis_x + y * basis_y + z * basis_z;
    result.normalise();
    return result;
}

Hit *Utils::trace(Ray ray, Object *object_list) {
    Hit *best_hit = 0;

    Object *objects = object_list;

    while (objects != 0) {
        Hit *hit = Utils::select_first(objects->intersection(ray));

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

bool Utils::shadow_trace(Ray ray, Object *object_list, float limit) {
    Object *objects = object_list;

    while (objects != 0) {
        Hit *hit = Utils::select_first(objects->intersection(ray));

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

Hit *Utils::select_first(Hit *list) {
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

// With code inspired from https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
void Utils::refract_ray(Vector &view, Vector &normal, Vector &refract_ray, float ior) {
    float cos_i = view.dot(normal);

    float eta_i = 1.0f, eta_t = ior;
    Vector n = normal;
    if (cos_i < 0.0f) {
        cos_i = -cos_i;
    } else {
        // The ray starts in the medium that is not air so swap the refractive indexes and negate normal
        swap(eta_i, eta_t);
        n = -normal;
    }

    float eta = eta_i / eta_t;
    float cos_t = sqrt(1.0f - (eta * eta) * (1.0f - cos_i * cos_i));
    refract_ray = eta * view + (eta * cos_i - cos_t) * n;
}

void Utils::reflect_ray(Vector &view, Vector &normal, Vector &reflect_ray) {
    float cos_i = view.dot(normal);

    reflect_ray = view - 2.0f * cos_i * normal;
}
