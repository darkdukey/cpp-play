#include <glm/glm.hpp>
#include <vector>
#include "loguru.hpp"

using namespace glm;
using namespace std;

class LineEQ {
   public:
    LineEQ(vec2 p1, vec2 p2) {
        m = (p2.y - p1.y) / (p2.x - p1.x);
        b = p1.y - m * p1.x;
    }

   private:
    float m;
    float b;
    bool isVertical;
};

bool hex_check(const vec2& point, const vector<vec2>& hex) {
    //Build line quations

    return false;
}

int main(int argc, char const* argv[]) {
    loguru::init(argc, argv);

    vector<vec2> hex{
        {1.0f, 0.5f},
        {1.0f, -0.5f},
        {0.0f, -1.0f},
        {-1.0f, -0.5f},
        {-1.0f, 0.5f},
        {0.0f, 1.0f}};

    vec2 p1(0.5f, 0.5f);
    vec2 p2(1.0f, 1.0f);
    bool p1_check = hex_check(p1, hex);
    bool p2_check = hex_check(p2, hex);
    LOG_S(INFO) << "P1: " << p1_check;
    LOG_S(INFO) << "P2: " << p2_check;

    return 0;
}
