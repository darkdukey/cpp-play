#include <glm/glm.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace glm;

bool weighted_avg(const vec3& pa, const vec3& pb, const vec3& pc, const vec3& target) {
    mat3 m(pa, pb, pc);
    mat3 inv_m = inverse(m);
    vec3 res = inv_m * target;
    if (res.x >= 0 && res.y >= 0 && res.z >= 0) {
        return true;
    }
    return false;
}

int main(int argc, char const* argv[]) {
    vec3 pa1(4, 2, 4);
    vec3 pb1(5, -5, 5);
    vec3 pc1(3, -2, 6);
    vec3 ta1(3.1f, -4.3f, 4.9);
    bool is_inside = weighted_avg(pa1, pb1, pc1, ta1);
    cout << (is_inside ? "inside" : "outside") << endl;

    vec3 pa2(4, 2, 4);
    vec3 pb2(5, -5, 5);
    vec3 pc2(3, -2, 6);
    vec3 ta2(3.7f, -1.4f, 5.2);
    is_inside = weighted_avg(pa2, pb2, pc2, ta2);
    cout << (is_inside ? "inside" : "outside") << endl;

    vec3 pa3(-2, 2, 2);
    vec3 pb3(3, -3, -2);
    vec3 pc3(4, 1, -4);
    vec3 ta3(-0.25f, -2.75f, 0);
    is_inside = weighted_avg(pa3, pb3, pc3, ta3);
    cout << (is_inside ? "inside" : "outside") << endl;

    vec3 pa4(2, 1, 1);
    vec3 pb4(2, 2, 2);
    vec3 pc4(4, 4, 2);
    vec3 ta4(3, 2.8, 1.8);
    is_inside = weighted_avg(pa4, pb4, pc4, ta4);
    cout << (is_inside ? "inside" : "outside") << endl;

    vec3 pa5(5, 2, 5);
    vec3 pb5(-3, 3, 5);
    vec3 pc5(4, -4, 4);
    vec3 ta5(0.9, 1.9, 4.9);
    is_inside = weighted_avg(pa5, pb5, pc5, ta5);
    cout << (is_inside ? "inside" : "outside") << endl;

    vec3 pa6(1, 2, 2);
    vec3 pb6(3, 3, 3);
    vec3 pc6(4, 5, 4);
    vec3 ta6(2.25, 3, 2.75);
    is_inside = weighted_avg(pa6, pb6, pc6, ta6);
    cout << (is_inside ? "inside" : "outside") << endl;

    return 0;
}
