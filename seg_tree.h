#pragma once
#include <vector>
#include "ops_counter.h"

template<typename T, typename Op>
struct SegTree {
    int n;
    std::vector<T> tree;
    Op op;
    T identity;

    SegTree(Op operation, T id) : op(operation), identity(id) {}

    void build(const std::vector<T>& a) {
        n = (int)a.size();
        tree.assign(4 * n, identity);
        build_impl(a, 1, 0, n - 1);
    }

    void build_impl(const std::vector<T>& a, int v, int tl, int tr) {
        inc();
        if (tl == tr) { tree[v] = a[tl]; return; }
        int tm = (tl + tr) / 2;
        build_impl(a, 2*v,   tl,    tm);
        build_impl(a, 2*v+1, tm+1,  tr);
        tree[v] = op(tree[2*v], tree[2*v+1]);
    }

    T query(int l, int r) {
        return query_impl(1, 0, n - 1, l, r);
    }

    T query_impl(int v, int tl, int tr, int l, int r) {
        inc();
        if (l > tr || r < tl) return identity;
        if (l <= tl && tr <= r) return tree[v];
        int tm = (tl + tr) / 2;
        return op(query_impl(2*v,   tl,    tm, l, r),
                  query_impl(2*v+1, tm+1,  tr, l, r));
    }

    void update(int pos, T val) {
        update_impl(1, 0, n - 1, pos, val);
    }

    void update_impl(int v, int tl, int tr, int pos, T val) {
        inc();
        if (tl == tr) { tree[v] = val; return; }
        int tm = (tl + tr) / 2;
        if (pos <= tm) update_impl(2*v,   tl,    tm, pos, val);
        else           update_impl(2*v+1, tm+1,  tr, pos, val);
        tree[v] = op(tree[2*v], tree[2*v+1]);
    }
};
