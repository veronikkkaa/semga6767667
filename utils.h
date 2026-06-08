#pragma once
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <iomanip>
#include <algorithm>

struct Measurement {
    int n;
    long long build_ops;
    double avg_query_ops;
};

inline std::vector<int> rand_vec(int n, int maxv = 1000) {
    std::vector<int> v(n);
    std::mt19937 rng(42);
    for (auto& x : v) x = rng() % maxv + 1;
    return v;
}

inline void print_table(const std::string& name,
                        const std::vector<Measurement>& data,
                        bool has_update = false) {
    std::cout << "\n=== " << name << " ===\n";
    std::cout << std::setw(8) << "n"
              << std::setw(14) << "build_ops"
              << std::setw(18) << "avg_query_ops\n";
    std::cout << std::string(40, '-') << "\n";
    for (auto& m : data) {
        std::cout << std::setw(8) << m.n
                  << std::setw(14) << m.build_ops
                  << std::setw(18) << std::fixed << std::setprecision(1) << m.avg_query_ops
                  << "\n";
    }
}

inline void print_chart(const std::string& title,
                        const std::vector<Measurement>& data,
                        bool use_build = true) {
    const int W = 50, H = 12;
    long long maxval = 0;
    for (auto& m : data)
        maxval = std::max(maxval, use_build ? m.build_ops : (long long)m.avg_query_ops);

    std::cout << "\n" << title << "\n";
    for (int row = H; row >= 0; row--) {
        long long threshold = (long long)maxval * row / H;
        std::cout << std::setw(10) << threshold << " |";
        for (auto& m : data) {
            long long val = use_build ? m.build_ops : (long long)m.avg_query_ops;
            std::cout << (val >= threshold ? "###" : "   ");
        }
        std::cout << "\n";
    }
    std::cout << std::string(12, ' ');
    for (auto& m : data) std::cout << std::setw(3) << m.n;
    std::cout << "  (n)\n";
}
