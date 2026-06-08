#pragma once
#include <vector>
#include <climits>
#include "ops_counter.h"

struct RMQ1D_Brute {
    int n;
    std::vector<std::vector<int>> mn;

    void build(const std::vector<int>& a) {
        n = (int)a.size();
        mn.assign(n, std::vector<int>(n, INT_MAX));
        for (int l = 0; l < n; l++) {
            mn[l][l] = a[l];
            inc();
            for (int r = l + 1; r < n; r++) {
                inc();
                mn[l][r] = std::min(mn[l][r-1], a[r]);
            }
        }
    }

    int query(int l, int r) {
        inc();
        return mn[l][r];
    }
};
