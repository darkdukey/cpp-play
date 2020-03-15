#include <glm/glm.hpp>
#include <vector>
#include "loguru.hpp"

using namespace glm;
using namespace std;

class LineEQ {
   public:
    // a * x + b * y + c = 0
    //(p2x - p1x) * y = (p2y - p1y)x + b * (p2x - p1x)
    //(p1y - p2y)x + (p2x - p1x)y + b * (p1x - p2x) = 0
    LineEQ(vec2 p1, vec2 p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = -a * p1.x - b * p1.y;
    }

    int test(vec2 p) {
        return a * p.x + b * p.y + c;
    }

   private:
    float a;
    float b;
    float c;
    bool isVertical;
};

bool hex_check(const vec2& point, const vector<vec2>& hex) {
    //Build line quations
    vector<LineEQ> eqs;
    bool inside = true;
    for (int i = 0; i < hex.size() - 1; i++) {
        vec2 v1 = hex[i];
        vec2 v2 = hex[i + 1];
        eqs.emplace_back(v1, v2);
    }

    for (auto& e : eqs) {
        if (e.test(point) > 0) {
            inside = false;
        }
    }

    return inside;
}

int main(int argc, char const* argv[]) {
    loguru::init(argc, argv);

    LineEQ eq1({0.0f, 0.0f}, {1.0f, 1.0f});
    vec2 p1(0.5f, 0.5f);
    vec2 p2(2.0f, 1.0f);
    vec2 p3(1.0f, 2.0f);

    int test = eq1.test(p1);
    LOG_S(INFO) << "EQ1 tset p1: " << test;

    test = eq1.test(p2);
    LOG_S(INFO) << "EQ1 tset p2: " << test;

    test = eq1.test(p3);
    LOG_S(INFO) << "EQ1 tset p3: " << test;

    vector<vec2> hex{
        {1.0f, 0.5f},
        {1.0f, -0.5f},
        {0.0f, -1.0f},
        {-1.0f, -0.5f},
        {-1.0f, 0.5f},
        {0.0f, 1.0f}};

    bool p1_check = hex_check(p1, hex);
    bool p2_check = hex_check(p2, hex);
    LOG_S(INFO) << "P1: " << (p1_check ? "inside" : "outside");
    LOG_S(INFO) << "P2: " << (p2_check ? "inside" : "outside");

    return 0;
}