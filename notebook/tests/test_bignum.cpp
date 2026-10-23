// Checks code/cpp/math_bigint.cpp against __int128 and with identities on long numbers.
#include "check.hpp"
#include "../code/cpp/math_bigint.cpp"

using u128 = unsigned __int128;
string str128(u128 x) {
    if (x == 0) return "0";
    string s;
    while (x > 0) s += char('0' + (int)(x % 10)), x /= 10;
    reverse(all(s));
    return s;
}
ll rndDigits() {   // a random number with 1 to 18 digits
    ll hi = 1;
    rep(i, 0, rnd(1, 18)) hi *= 10;
    return rndll(0, hi - 1);
}
string rndBig(int maxLen) {
    string s(1, char('1' + rnd(0, 8)));
    rep(i, 1, rnd(1, maxLen)) s += char('0' + rnd(0, 9));
    return s;
}

int main() {
    rep(iter, 0, 30000) {
        ll a = rndDigits(), b = rndDigits();
        string A = to_string(a), B = to_string(b);
        CHECK(addBig(A, B) == str128((u128)a + b));
        CHECK(mulBig(A, B) == str128((u128)a * b));
        CHECK(lessBig(A, B) == (a < b));
        if (a >= b) CHECK(subBig(A, B) == to_string(a - b));
        else CHECK(subBig(B, A) == to_string(b - a));
        ll m = rndll(1, iter % 2 ? 100 : (ll)1e17), rem;
        CHECK(divSmall(A, m, rem) == to_string(a / m) && rem == a % m);
    }
    rep(iter, 0, 300) {
        string x = rndBig(300), y = rndBig(300), z = rndBig(300);
        CHECK(subBig(addBig(x, y), y) == x);
        CHECK(subBig(x, x) == "0");
        CHECK(mulBig(x, "1") == x && mulBig(x, "0") == "0" && mulBig(x, "10") == x + "0");
        ll rem;
        CHECK(divSmall(mulBig(x, "7"), 7, rem) == x && rem == 0);
        CHECK(mulBig(x, addBig(y, z)) == addBig(mulBig(x, y), mulBig(x, z)));
        CHECK(lessBig(x, addBig(x, "1")) && !lessBig(x, x));
    }
    puts("  big numbers: add, sub, mul, divSmall, compare OK");
}
