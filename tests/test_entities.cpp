#include "entities/entities.hpp"
using namespace PLR::Entities;

auto main() -> int {
    Point<> p1(1, 1);
    p1.dump();
    Point<> p2(2, 3);
    p2.dump();

    auto line = Line<>::make_line(p1, p2);
    line->dump();

    auto seg1 = Segment<>::make_segment(*line, 1);
    seg1->dump();

    auto seg2 = Segment<>::make_segment(p1, p2);
    seg2->dump();

    return 0;
}
