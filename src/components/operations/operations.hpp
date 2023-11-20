#ifndef __PLR__OPERATIONS__OPERATIONS__
#define __PLR__OPERATIONS__OPERATIONS__
#include "entities/entities.hpp"

namespace PLR::Operations {
    using namespace Entities;
    using namespace Types;

    template <Numeric N>
    auto slope(const Point<N> &p1, const Point<N> &p2) -> N {
        return (p1.y - p2.y) / (p1.x - p2.x);
    }

    template <Numeric N>
    auto interception_of(const Line<N> &l1, const Line<N> &l2) -> Point<N> {
        auto x = (l2.intercept - l1.intercept) / (l1.slope - l2.slope);
        auto y = l1.get_y(x);
        return Point(x, y);
    }

    template <Numeric N>
    auto above_line(const Point<N> &p, const Line<N> &line) -> bool {
        auto predicted = line.get_y(p.x);
        return p.y > predicted;
    }

    template <Numeric N>
    auto below_line(const Point<N> &p, const Line<N> &line) -> bool {
        auto predicted = line.get_y(p.x);
        return p.y < predicted;
    }

    template <Numeric N>
    auto error_upper(const Point<N> &p, const N &error) -> Point<N> {
        return Point(p.x, p.y + error);
    }

    template <Numeric N>
    auto error_lower(const Point<N> &p, const N &error) -> Point<N> {
        return Point(p.x, p.y - error);
    }

    template <Numeric N>
    auto get_average_slope(const Line<N> &l1, const Line<N> &l2) -> N {
        return (l1.slope + l2.slope) / 2.0;
    }
}
#endif
