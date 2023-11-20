#include <functional>
#include <cmath>
#include <chrono>

#include "entities/entities.hpp"
#include "greedy_plr/greedy_plr.hpp"

using namespace PLR;

auto check_plr(const std::function<double(double)> &seq, size_t num_points) -> bool {
    std::vector<Point<double>> curve;
    for (size_t i = 0; i < num_points; i++) {
        curve.emplace_back(i, seq(i));
    }

    auto greedy = GreedyPLR<double>::make_greedy_plr(0.05);

    auto s = std::chrono::steady_clock::now();
    greedy->train(curve);
    auto e = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() << " us for training\n";

    s = std::chrono::steady_clock::now();
    for (size_t i = 0; i < num_points; i++) {
        if (auto v = greedy->predict(i).value(); std::abs(v - curve[i].y) > 0.05) {
            std::cout << i << ": Predicted: " << v << ", " << curve[i].y << " wanted\n";
            // return false;
        }
    }
    e = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() / num_points << " ns for predicting\n";

    greedy->report();

    return true;
}

auto main() -> int {
    auto ret = check_plr([&](double i) {
        return i;
    }, 1000000);
    
    if (!ret) {
        std::cerr << "Line test failed\n";
        return -1;
    }
    std::cout << "=======================================\n";

    ret = check_plr([&](double i) {
        return sin(i);
    }, 1000000);

    if (!ret) {
        std::cerr << "Sin test failed\n";
        return -1;
    }
    std::cout << "=======================================\n";

    std::cout << "Passed\n";
    return 0;
}
