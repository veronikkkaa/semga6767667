#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include "ops_counter.h"

template<typename T, typename Op>
struct SqrtDecomp {
    int n, block;
    std::vector<T> data;
    std::vector<T> blocks;
    Op op;
    T identity;

    SqrtDecomp(Op operation, T id) : op(operation), identity(id) {}

    void build(const std::vector<T>& a) {
        n = (int)a.size();
        block = std::max(1, (int)std::sqrt((double)n));
        data = a;
        int num_blocks = (n + block - 1) / block;
        blocks.assign(num_blocks, identity);
        for (int i = 0; i < n; i++) {
            inc();
            blocks[i / block] = op(blocks[i / block], data[i]);
        }
    }

    T query(int l, int r) {
        T res = identity;
        int bl = l / block, br = r / block;
        if (bl == br) {
            for (int i = l; i <= r; i++) { inc(); res = op(res, data[i]); }
        } else {
            for (int i = l; i < (bl + 1) * block; i++) { inc(); res = op(res, data[i]); }
            for (int b = bl + 1; b < br; b++) { inc(); res = op(res, blocks[b]); }
            for (int i = br * block; i <= r; i++) { inc(); res = op(res, data[i]); }
        }
        return res;
    }

    void update(int pos, T val) {
        data[pos] = val;
        int b = pos / block;
        blocks[b] = identity;
        int lo = b * block, hi = std::min(n - 1, lo + block - 1);
        for (int i = lo; i <= hi; i++) { inc(); blocks[b] = op(blocks[b], data[i]); }
    }
};
