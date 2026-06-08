#pragma once
#include <vector>
#include "ops_counter.h"

struct RSQ1D_Prefix {
    std::vector<long long> pre;

    void build(const std::vector<int>& a) {
        int n = (int)a.size();
        pre.resize(n + 1, 0);
        for (int i = 0; i < n; i++) {
            inc();
            pre[i + 1] = pre[i] + a[i];
        }
    }

    long long query(int l, int r) {
        inc();
        return pre[r + 1] - pre[l];
    }
};
