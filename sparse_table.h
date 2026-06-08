#pragma once
#include <vector>
#include <algorithm>
#include "ops_counter.h"

struct SparseTable {
    int n, LOG;
    std::vector<std::vector<int>> table;
    std::vector<int> log2_floor;

    void build(const std::vector<int>& a) {
        n = (int)a.size();
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        table.assign(LOG, std::vector<int>(n));
        log2_floor.resize(n + 1);

        log2_floor[1] = 0;
        for (int i = 2; i <= n; i++)
            log2_floor[i] = log2_floor[i / 2] + 1;

        for (int i = 0; i < n; i++) {
            inc();
            table[0][i] = a[i];
        }
        for (int k = 1; k < LOG; k++)
            for (int i = 0; i + (1 << k) <= n; i++) {
                inc();
                table[k][i] = std::min(table[k-1][i],
                                       table[k-1][i + (1 << (k-1))]);
            }
    }

    int query(int l, int r) {
        int k = log2_floor[r - l + 1];
        inc(2);
        return std::min(table[k][l], table[k][r - (1 << k) + 1]);
    }
};
