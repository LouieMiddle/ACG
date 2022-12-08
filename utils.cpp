#include "utils.h"

Hit *utils::merge_hits(Hit *head1, Hit *head2) {
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

Hit *utils::mid_point_hits(Hit *head) {
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

Hit *utils::merge_sort_hits(Hit *head) {
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

void utils::append_hits(Hit **head_ref, Hit *new_data) {
    Hit *last = *head_ref;

    if (*head_ref == 0) {
        *head_ref = new_data;
        return;
    }

    while (last->next != 0)
        last = last->next;

    last->next = new_data;
}

float utils::get_random_number(float min, float max) {
    random_device device;
    mt19937 random(device());
    uniform_real_distribution<> distribution(min, max);
    auto n = distribution(random);
    return n;
}

Vector utils::get_random_direction() {
    float x = get_random_number(-1.0f, 1.0f);
    float y = get_random_number(-1.0f, 1.0f);
    float z = get_random_number(-1.0f, 1.0f);

    Vector direction = Vector(x, y, z);
    direction.normalise();

    return direction;
}

// generate random direction in direction within range defined by theta
Vector utils::get_random_direction_in_range(Vector direction, float theta) {
    Vector basis_z = direction;
    basis_z.normalise();

    Vector basis_x;
    Vector different = (0.5 > fabs(basis_z.x)) ? Vector(1, 0, 0) : Vector(0, 1, 0);
    basis_z.cross(different, basis_x);
    basis_x.normalise();

    Vector basis_y;
    basis_x.cross(basis_z, basis_y);

    // height
    float z = utils::get_random_number(cos(theta), 1);

    // rotation
    float phi = utils::get_random_number(-M_PI, +M_PI);
    float x = sqrt(1 - z * z) * cos(phi);
    float y = sqrt(1 - z * z) * sin(phi);

    // combine
    Vector result = x * basis_x + y * basis_y + z * basis_z;
    result.normalise();
    return result;
}

