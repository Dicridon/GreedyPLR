#ifndef __PLR__GREEDY_PLR__GREEDY_PLR__
#define __PLR__GREEDY_PLR__GREEDY_PLR__
#include <cstddef>
#include <iostream>
#include <vector>

#include "types/types.hpp"
#include "entities/entities.hpp"
#include "operations/operations.hpp"

namespace PLR {
    using namespace Types;
    using namespace Entities;
    using namespace Operations;

    template <Numeric N = float>
    class GreedyPLR {
    public:
        GreedyPLR() = delete;
        GreedyPLR(const N &err) : error(err) {};
        GreedyPLR(const GreedyPLR<N> &) = delete;
        GreedyPLR(GreedyPLR<N> &&) = default;
        auto operator=(const GreedyPLR<N> &) -> GreedyPLR<N> & = delete;
        auto operator=(GreedyPLR<N> &&) -> GreedyPLR<N> & = default;

        static auto make_greedy_plr(const N& err) -> std::unique_ptr<GreedyPLR<N>> {
            return std::make_unique<GreedyPLR<N>>(err);
        }

        auto predict(const N& x) -> std::optional<N> {
            if (need != -1) {
                return {};
            }

            auto sz = segments.size();

            for (size_t i = 0; i < sz - 1; i++) {
                if (segments[i].start <= x && segments[i + 1].start > x) {
                    return segments[i].predict(x);
                }
            }

            if (dangle && x >= tail.x) {
                return tail.y;                
            }

            return segments[sz - 1].predict(x);
        }

        // if all data points are ready, use this
        // true: trainning is sucessful
        // false: something goes wrong
        auto train(const std::vector<Point<N>> &curve) -> bool {
            auto ret = true;
            for (const auto &p : curve) {
                ret &= iterate_on(p);
            }

            finish();
            return ret;
        }

        // if the data pointer are delivered via an iterator
        // true: trainning is sucessful
        // false: something goes wrong
        // remember to call finish() after iteratin over all the points
        auto iterate_on(const Point<N> &p) -> bool {
            switch (need) {
            case 2: {
                s1 = p;
                need = 1;
                return true;
            }
            case 1: {
                s2 = p;
                need = 0;
                // here we follow the naming in the paper
                auto sa = error_upper(s1, error);
                auto sb = error_lower(s1, error);
                auto sc = error_lower(s2, error);
                auto sd = error_upper(s2, error);

                plr_lower.initialize_from(sa, sc);
                plr_upper.initialize_from(sb, sd);
                s0 = interception_of(plr_lower, plr_upper);
                return true;
            }
            case 0: {
                if (try_consume(p)) {
                    return true;
                } else {
                    need = 1;
                    s1 = p;
                    return true;
                }
            }
            default: {
                std::cerr << "Unexpected value of need: " << need << "\n";
                return false;
            }
            }
        }

        auto dump() const noexcept {
            for (const auto & seg : segments) {
                seg.dump();
            }

            if (dangle) {
                tail.dump();
            } else {
                std::cout << "No dangling point\n";
            }
        }

        auto report() const noexcept {
            std::cout << segments.size() << " segments are trained\n";
            if (dangle) {
                std::cout << "Tail is usable: ";
                tail.dump();
            }
        }
        
    private:
        auto try_consume(const Point<N> &p) -> bool {
            auto should_yield = above_line(p, plr_upper) || below_line(p, plr_lower);
            if (should_yield) {
                // this point is out of the bound
                // start a new segment and return
                yield_current_segment();
                return false;
            }

            auto p_upper = error_upper(p, error);
            auto p_lower = error_lower(p, error);

            if (below_line(p_upper, plr_upper)) {
                plr_upper.update_to(s0, p_upper);
            }

            if (above_line(p_lower, plr_lower)) {
                plr_lower.update_to(s0, p_lower);
            }

            return true;
        }

        auto yield_current_segment() -> bool {
            switch (need) {
            case 2: {
                // no segment now, just leave
                return false;
            }
            case 1: {
                // a dangling point
                tail = s1;
                dangle = true;
                return true;
            }
            case 0: {
                current_segment.line.initialize_from(get_average_slope(plr_lower, plr_upper), s0);
                current_segment.start = s1.x;
                segments.push_back(current_segment);
                return true;
            }
            default: {
                std::cerr << "Unexpected value of need: " << need << "\n";
                return false;
            }
            }
        }

        // stop right now, yielding the traning segment unconditionally
        auto finish() -> void {
            yield_current_segment();
            need = -1;
        }

    private:
        N error;
        std::vector<Segment<N>> segments;
        Segment<N> current_segment;
        Point<N> s1, s2, s0;
        Line<N> plr_lower, plr_upper;

        // if the sequece has a dangling point, store it here
        Point<N> tail;
        bool dangle = false;
        int need = 2;
    };
}
#endif
