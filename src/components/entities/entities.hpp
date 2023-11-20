#ifndef __PLR__ENTITIES__ENTITIES__
#define __PLR__ENTITIES__ENTITIES__
#include <concepts>
#include <memory>
#include <iostream>

#include "types/types.hpp"

namespace PLR::Entities {
    using namespace PLR::Types;
    template <Numeric Pos = float>
    struct Point {
        Pos x = 0;
        Pos y = 0;

        explicit Point(Pos x_, Pos y_) : x(x_), y(y_) {};
        Point() = default;
        Point(const Point<Pos> &) = default;
        Point(Point<Pos> &&) = default;
        auto operator=(const Point<Pos> &) -> Point<Pos> & = default;
        auto operator=(Point<Pos> &&) -> Point<Pos> & = default;

        static auto make_point(Pos x, Pos y) -> std::unique_ptr<Point> {
            return std::make_unique<Point>(x, y);
        }

        auto dump() const noexcept -> void {
            std::cout << "Point: (" << x << ", " << y << ")\n";
        }
    };

    template <Numeric N = float>
    struct Line {
        N slope = 0;
        N intercept = 0;

        explicit Line(N s, N i) : slope(s), intercept(i) {};
        Line() = default;
        Line(const Line<N> &) = default;
        Line(Line<N> &&) = default;
        auto operator=(const Line<N> &) -> Line<N>& = default;
        auto operator=(Line<N> &&) -> Line<N>& = default;

        static auto make_line(const N &s, const N &i) -> std::unique_ptr<Line<N>> {
            return std::make_unique<Line<N>>(s, i);
        }

        static auto make_line(const Point<N> &p1, const Point<N> &p2)
            -> std::unique_ptr<Line<N>>
        {
            if (p1.x == p2.x)
                return nullptr;
            
            auto slope = (p1.y - p2.y) / (p1.x - p2.x);
            auto intercept = p1.y - slope * p1.x;
            return std::make_unique<Line>(slope, intercept);
        }

        auto initialize_from(const Point<N> &p1, const Point<N> &p2) -> bool {
            if (p1.x == p2.x)
                return false;

            slope = (p1.y - p2.y) / (p1.x - p2.x);
            intercept = p1.y - slope * p1.x;
            return false;
        }

        auto initialize_from(const N& s, const Point<N> &p) -> void {
            intercept = p.y - s * p.x;
            slope = s;
        }

        auto update_to(const N& s, const N& i) -> void {
            slope = s;
            intercept = i;
        }

        auto update_to(const Point<N> &p1, const Point<N> &p2) -> bool {
            return initialize_from(p1, p2);
        }

        auto get_y(const N& x) const noexcept -> N {
            return slope * x + intercept;
        }

        auto dump() const noexcept -> void {
            std::cout << "Line: [" << slope << ", " << intercept << "]\n";
        }
    };

    template <Numeric N = float>
    struct Segment {
        Line<N> line;
        N start = 0;

        explicit Segment(const Line<N> &l, N s) : line(l), start(s) {};
        Segment() = default;
        Segment(const Segment &) = default;
        Segment(Segment &&) = default;
        auto operator=(const Segment &) -> Segment<N>& = default;
        auto operator=(Segment &&) -> Segment<N>& = default;

        // make a new segment with slope l starting at point s
        static auto make_segment(const Line<N> &l, N s) -> std::unique_ptr<Segment> {
            return std::make_unique<Segment>(l, s);
        }

        // make a new segment passing two points, with the first point's x being the start
        static auto make_segment(const Point<N> &p1, const Point<N> &p2)
            -> std::unique_ptr<Segment>
        {
            auto seg = std::make_unique<Segment<N>>();
            seg->line.initialize_from(p1, p2);
            seg->start = p1.x;
            return seg;
        }

        auto initialize_from(const N& slope, const Point<N>& s0) -> void {
            line.initialize_from(slope, start);
        }

        auto predict(const N &x) -> N {
            return line.get_y(x);
        }

        auto dump() const noexcept -> void {
            std::cout << "Segment: {" << line.slope << ", " << line.intercept
                      << ", " << start << "}\n";
        }
    };
}
#endif
