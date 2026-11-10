// "HH:MM" -> minutes since midnight, and back
int toMin(const string& t) {
    return stoi(t.substr(0, 2)) * 60 + stoi(t.substr(3, 2));
}
string toTime(int m) {   // 0 <= m < 1440
    char buf[16];
    snprintf(buf, sizeof buf, "%02d:%02d", m / 60, m % 60);
    return buf;
}
// minutes from a to b, also past midnight: (b - a + 1440) % 1440
bool leap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}
int daysIn(int m, int y) {   // month m = 1..12
    static const int d[] = {31, 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31};
    return d[m - 1] + (m == 2 && leap(y));
}
// a day number: days between two dates = dayNum(b) - dayNum(a)
ll dayNum(ll y, ll m, ll d) {
    if (m < 3) y--, m += 12;
    return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5
           + d - 306;
}
// day of the week: 0 = Monday, ..., 6 = Sunday
int weekday(ll y, ll m, ll d) { return (dayNum(y, m, d) + 6) % 7; }
