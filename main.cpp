#include <bits/stdc++.h>
#include "ops_counter.h"
#include "rsq1d_prefix.h"
#include "rsq2d_prefix.h"
#include "rmq1d_brute.h"
#include "sqrt_decomp.h"
#include "seg_tree.h"
#include "fenwick.h"
#include "sparse_table.h"
#include "utils.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const vector<int> SIZES = {100, 200, 500, 1000, 2000, 5000, 10000};
    const int Q = 200;
    mt19937 rng(123);

    {
        vector<Measurement> data;
        for (int n : SIZES) {
            auto a = rand_vec(n);
            RSQ1D_Prefix rsq;

            reset_ops();
            rsq.build(a);
            long long build = get_ops();

            long long qsum = 0;
            for (int q = 0; q < Q; q++) {
                int l = rng() % n, r = rng() % n;
                if (l > r) swap(l, r);
                reset_ops();
                rsq.query(l, r);
                qsum += get_ops();
            }
            data.push_back({n, build, (double)qsum / Q});
        }
        print_table("RSQ 1D Prefix Sum", data);
        print_chart("RSQ 1D | Build ops (O(n))", data, true);
        print_chart("RSQ 1D | Query ops (O(1))", data, false);
    }

    {
        vector<Measurement> data;
        const vector<int> SQ = {10,15,20,30,50,70,100};
        for (int sz : SQ) {
            vector<vector<int>> mat(sz, vector<int>(sz));
            for (auto& row : mat)
                for (auto& x : row) x = rng() % 100 + 1;

            RSQ2D_Prefix rsq2d;
            reset_ops();
            rsq2d.build(mat);
            long long build = get_ops();

            long long qsum = 0;
            for (int q = 0; q < Q; q++) {
                int r1 = rng()%sz, r2 = rng()%sz, c1 = rng()%sz, c2 = rng()%sz;
                if (r1>r2) swap(r1,r2); if (c1>c2) swap(c1,c2);
                reset_ops();
                rsq2d.query(r1,c1,r2,c2);
                qsum += get_ops();
            }
            data.push_back({sz*sz, build, (double)qsum / Q});
        }
        print_table("RSQ 2D Prefix Sum (n = rows*cols)", data);
        print_chart("RSQ 2D | Build ops (O(n*m))", data, true);
    }

    {
        vector<Measurement> data;
        const vector<int> SM = {50, 100, 200, 500, 1000, 2000, 3000};
        for (int n : SM) {
            auto a = rand_vec(n);
            RMQ1D_Brute rmq;

            reset_ops();
            rmq.build(a);
            long long build = get_ops();

            long long qsum = 0;
            for (int q = 0; q < Q; q++) {
                int l = rng()%n, r = rng()%n;
                if (l>r) swap(l,r);
                reset_ops();
                rmq.query(l,r);
                qsum += get_ops();
            }
            data.push_back({n, build, (double)qsum / Q});
        }
        print_table("RMQ 1D Brute (all pairs)", data);
        print_chart("RMQ 1D Brute | Build ops (O(n^2))", data, true);
        print_chart("RMQ 1D Brute | Query ops (O(1))", data, false);
    }

    {
        auto sumOp = [](long long a, long long b){ return a + b; };
        vector<Measurement> data_rsq, data_rmq;

        for (int n : SIZES) {
            auto a = rand_vec(n);
            vector<long long> al(a.begin(), a.end());

            SqrtDecomp<long long, decltype(sumOp)> sq(sumOp, 0LL);
            reset_ops(); sq.build(al); long long build = get_ops();

            long long qsum = 0;
            for (int q = 0; q < Q; q++) {
                int l = rng()%n, r = rng()%n; if(l>r) swap(l,r);
                reset_ops(); sq.query(l,r); qsum += get_ops();
            }
            data_rsq.push_back({n, build, (double)qsum/Q});
        }

        auto minOp = [](int a, int b){ return min(a,b); };
        for (int n : SIZES) {
            auto a = rand_vec(n);
            SqrtDecomp<int, decltype(minOp)> sq(minOp, INT_MAX);
            reset_ops(); sq.build(a); long long build = get_ops();

            long long qsum = 0;
            for (int q = 0; q < Q; q++) {
                int l = rng()%n, r = rng()%n; if(l>r) swap(l,r);
                reset_ops(); sq.query(l,r); qsum += get_ops();
            }
            data_rmq.push_back({n, build, (double)qsum/Q});
        }

        print_table("RSQ Sqrt Decomp", data_rsq);
        print_chart("RSQ Sqrt | Query ops (O(sqrt(n)))", data_rsq, false);
        print_table("RMQ Sqrt Decomp", data_rmq);
        print_chart("RMQ Sqrt | Query ops (O(sqrt(n)))", data_rmq, false);
    }

    {
        auto sumOp = [](long long a, long long b){ return a+b; };
        auto minOp = [](int a, int b){ return min(a,b); };
        vector<Measurement> data_rsq, data_rmq, data_upd;

        for (int n : SIZES) {
            auto a = rand_vec(n);
            vector<long long> al(a.begin(), a.end());

            SegTree<long long, decltype(sumOp)> st_sum(sumOp, 0LL);
            reset_ops(); st_sum.build(al); long long b1 = get_ops();
            long long qs = 0;
            for (int q = 0; q < Q; q++) {
                int l=rng()%n, r=rng()%n; if(l>r) swap(l,r);
                reset_ops(); st_sum.query(l,r); qs += get_ops();
            }
            data_rsq.push_back({n, b1, (double)qs/Q});

            SegTree<int, decltype(minOp)> st_min(minOp, INT_MAX);
            reset_ops(); st_min.build(a); long long b2 = get_ops();
            long long qm = 0, qu = 0;
            for (int q = 0; q < Q; q++) {
                int l=rng()%n, r=rng()%n; if(l>r) swap(l,r);
                reset_ops(); st_min.query(l,r); qm += get_ops();
                reset_ops(); st_min.update(rng()%n, rng()%1000); qu += get_ops();
            }
            data_rmq.push_back({n, b2, (double)qm/Q});
            data_upd.push_back({n, b2, (double)qu/Q});
        }

        print_table("SegTree RSQ", data_rsq);
        print_chart("SegTree RSQ | Build (O(n))", data_rsq, true);
        print_chart("SegTree RSQ | Query (O(log n))", data_rsq, false);
        print_table("SegTree RMQ", data_rmq);
        print_table("SegTree Update", data_upd);
        print_chart("SegTree | Update (O(log n))", data_upd, false);
    }

    {
        vector<Measurement> data_build, data_query, data_upd;
        for (int n : SIZES) {
            auto a = rand_vec(n);
            FenwickTree fen;

            reset_ops(); fen.build(a); long long build = get_ops();

            long long qs = 0, qu = 0;
            vector<long long> cur(a.begin(), a.end());
            for (int q = 0; q < Q; q++) {
                int l=rng()%n, r=rng()%n; if(l>r) swap(l,r);
                reset_ops(); fen.query(l,r); qs += get_ops();

                int pos = rng()%n;
                long long nv = rng()%1000+1;
                reset_ops(); fen.update(pos, cur[pos], nv); qu += get_ops();
                cur[pos] = nv;
            }
            data_build.push_back({n, build, 0});
            data_query.push_back({n, build, (double)qs/Q});
            data_upd.push_back({n, build, (double)qu/Q});
        }

        print_table("Fenwick Tree RSQ", data_query);
        print_chart("Fenwick | Build (O(n log n))", data_build, true);
        print_chart("Fenwick | Query (O(log n))", data_query, false);
        print_chart("Fenwick | Update (O(log n))", data_upd, false);
    }

    {
        vector<Measurement> data_build, data_query;
        for (int n : SIZES) {
            auto a = rand_vec(n);
            SparseTable st;

            reset_ops(); st.build(a); long long build = get_ops();

            long long qs = 0;
            for (int q = 0; q < Q; q++) {
                int l=rng()%n, r=rng()%n; if(l>r) swap(l,r);
                reset_ops(); st.query(l,r); qs += get_ops();
            }
            data_build.push_back({n, build, 0});
            data_query.push_back({n, build, (double)qs/Q});
        }

        print_table("Sparse Table RMQ", data_query);
        print_chart("Sparse Table | Build (O(n log n))", data_build, true);
        print_chart("Sparse Table | Query (O(1))", data_query, false);
    }

    cout << "\n[DONE] Все замеры завершены.\n";
    return 0;
}
