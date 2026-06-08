#pragma once
#include <vector>
#include "ops_counter.h"

struct RSQ2D_Prefix {
    std::vector<std::vector<long long>> pre;
    int rows, cols;

    void build(const std::vector<std::vector<int>>& mat) {
        rows = (int)mat.size();
        cols = rows ? (int)mat[0].size() : 0;
        pre.assign(rows + 1, std::vector<long long>(cols + 1, 0));
        for (int i = 1; i <= rows; i++)
            for (int j = 1; j <= cols; j++) {
                inc();
                pre[i][j] = mat[i-1][j-1]
                           + pre[i-1][j]
                           + pre[i][j-1]
                           - pre[i-1][j-1];
            }
    }

    long long query(int r1, int c1, int r2, int c2) {
        inc(3);
        return pre[r2+1][c2+1]
             - pre[r1][c2+1]
             - pre[r2+1][c1]
             + pre[r1][c1];
    }
};
