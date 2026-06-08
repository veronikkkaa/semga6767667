#pragma once
#include <vector>
#include "ops_counter.h"

struct FenwickTree {
    int n;
    std::vector<long long> bit;

    void build(const std::vector<int>& a) {
        n = (int)a.size();
        bit.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            add(i, a[i]);
        }
    }

    void add(int pos, long long val) {
        for (int i = pos + 1; i <= n; i += i & (-i)) {
            inc();
            bit[i] += val;
        }
    }

    long long prefix(int pos) {
        long long res = 0;
        for (int i = pos + 1; i > 0; i -= i & (-i)) {
            inc();
            res += bit[i];
        }
        return res;
    }

    long long query(int l, int r) {
        if (l == 0) return prefix(r);
        return prefix(r) - prefix(l - 1);
    }

    void update(int pos, long long old_val, long long new_val) {
        add(pos, new_val - old_val);
    }
};
