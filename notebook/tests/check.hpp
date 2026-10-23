// Shared by every test file: the template (without main), random numbers
// and a CHECK macro that stops with the failing file and line.
#include "prelude.hpp"   // template.cpp without main, made by run.sh

mt19937 rng(12345);
int rnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
ll rndll(ll a, ll b) { return uniform_int_distribution<ll>(a, b)(rng); }

#define CHECK(cond) do { if (!(cond)) { \
    fprintf(stderr, "FAIL %s line %d: %s\n", __FILE__, __LINE__, #cond); \
    exit(1); } } while (0)
