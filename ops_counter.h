#pragma once

static long long g_ops = 0;

inline void inc(long long n = 1) { g_ops += n; }
inline void reset_ops() { g_ops = 0; }
inline long long get_ops() { return g_ops; }
