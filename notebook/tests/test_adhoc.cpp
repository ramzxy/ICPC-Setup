// Checks the snippets of sections/adhoc.tex.
#include "check.hpp"
#include "../code/cpp/adhoc_grid.cpp"
#include "../code/cpp/adhoc_time.cpp"
#include "../code/cpp/adhoc_parse.cpp"
#include "../code/cpp/adhoc_base.cpp"

void testGrid() {
    vector<string> g = {"ab", "cd", "ef"};
    CHECK((rotateCW(g) == vector<string>{"eca", "fdb"}));
    CHECK((transpose(g) == vector<string>{"ace", "bdf"}));
    rep(iter, 0, 1000) {
        int R = rnd(1, 6), C = rnd(1, 6);
        vector<string> h(R, string(C, 'a'));
        for (auto& row : h) for (char& c : row) c = 'a' + rnd(0, 25);
        CHECK(rotateCW(rotateCW(rotateCW(rotateCW(h)))) == h);
        auto t = transpose(h);   // clockwise = transpose, then mirror
        for (auto& row : t) reverse(all(row));
        CHECK(t == rotateCW(h));
        auto u = transpose(h);   // counter-clockwise = transpose, then upside down
        reverse(all(u));
        CHECK(u == rotateCW(rotateCW(rotateCW(h))));
        CHECK(transpose(transpose(h)) == h);
    }
    set<pii> nb;
    rep(d, 0, 8) {
        CHECK(abs(dr8[d]) <= 1 && abs(dc8[d]) <= 1 && (dr8[d] || dc8[d]));
        nb.insert({dr8[d], dc8[d]});
    }
    CHECK(nb.size() == 8);
    puts("  grid rotate, transpose, 8 neighbours OK");
}

void testTime() {
    rep(m, 0, 1440) CHECK(toMin(toTime(m)) == m);
    CHECK(toTime(5) == "00:05" && toTime(23 * 60 + 59) == "23:59" && toMin("12:30") == 750);
    CHECK(leap(2000) && !leap(1900) && leap(2024) && !leap(2023));
    CHECK(daysIn(2, 2024) == 29 && daysIn(2, 2023) == 28 && daysIn(2, 1900) == 28);
    CHECK(daysIn(4, 2023) == 30 && daysIn(12, 2023) == 31 && daysIn(1, 2023) == 31);
    // walk day by day from Monday 1 January 1900 to the end of 2100
    int wd = 0;
    ll num = dayNum(1900, 1, 1);
    for (int y = 1900; y <= 2100; y++)
        for (int m = 1; m <= 12; m++)
            for (int d = 1; d <= daysIn(m, y); d++) {
                CHECK(dayNum(y, m, d) == num && weekday(y, m, d) == wd);
                num++;
                wd = (wd + 1) % 7;
            }
    CHECK(weekday(2026, 10, 31) == 5);   // BAPC 2026: a Saturday
    puts("  time, leap years, day numbers, weekday OK");
}

void testParse() {
    CHECK((split("a,b,,c", ',') == vector<string>{"a", "b", "", "c"}));
    CHECK((split("", ',') == vector<string>{""}));
    rep(iter, 0, 3000) {
        string s;
        rep(i, 0, rnd(0, 12)) s += "ab,"[rnd(0, 2)];
        auto parts = split(s, ',');
        CHECK((int)parts.size() == count(all(s), ',') + 1);
        string back;
        rep(i, 0, (int)parts.size()) {
            CHECK(parts[i].find(',') == string::npos);
            back += (i ? "," : "") + parts[i];
        }
        CHECK(back == s);
    }
    CHECK((numbersIn("12:30-14:05") == vi{12, 30, 14, 5}));
    CHECK(numbersIn("no numbers here").empty());
    CHECK((numbersIn("x=7, y=-42.") == vi{7, 42}));
    puts("  split, numbers in text OK");
}

void testBase() {
    CHECK(toBase(255, 16) == "ff" && toBase(5, 2) == "101" && toBase(0, 7) == "0");
    CHECK(fromBase("FF", 16) == 255 && fromBase("zz", 36) == 35 * 36 + 35);
    CHECK(bitset<64>(5).to_string().substr(61) == "101" && stoll("101", nullptr, 2) == 5);
    rep(iter, 0, 20000) {
        ll x = iter < 100 ? iter : rndll(0, (ll)1e18);
        int b = rnd(2, 36);
        string s = toBase(x, b);
        CHECK(fromBase(s, b) == x);
        CHECK(s == "0" || s[0] != '0');
        for (char c : s) {
            int d = isdigit(c) ? c - '0' : c - 'a' + 10;
            CHECK(d >= 0 && d < b);
        }
    }
    puts("  number bases OK");
}

int main() {
    testGrid();
    testTime();
    testParse();
    testBase();
}
