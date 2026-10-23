// Checks the snippets of sections/geometry.tex against brute force.
// The point snippet is read twice: in namespace I with T = ll (as
// printed) and in namespace D with T = double (ll is #defined to
// double while geo_point.cpp is read, so its "using T = ll" changes).
#include "check.hpp"

namespace I {
#include "../code/cpp/geo_point.cpp"
#include "../code/cpp/geo_segment.cpp"
#include "../code/cpp/geo_line.cpp"
#include "../code/cpp/geo_polygon.cpp"
#include "../code/cpp/geo_hull.cpp"
#include "../code/cpp/geo_closest.cpp"
#include "../code/cpp/geo_angle.cpp"
}
namespace D {
#define ll double
#include "../code/cpp/geo_point.cpp"
#undef ll
#include "../code/cpp/geo_segment.cpp"
#include "../code/cpp/geo_line.cpp"
#include "../code/cpp/geo_polygon.cpp"
#include "../code/cpp/geo_angle.cpp"
#include "../code/cpp/geo_rotate.cpp"
#include "../code/cpp/geo_circle.cpp"
}
static_assert(is_same<I::T, long long>::value, "I uses ll");
static_assert(is_same<D::T, double>::value, "D uses double");

using IP = I::P;
using DP = D::P;
IP rndP(int lo, int hi) { return {rnd(lo, hi), rnd(lo, hi)}; }
DP toD(IP p) { return {(double)p.x, (double)p.y}; }
// cross product written out, so the brute force does not use the snippet
ll crossB(IP a, IP b, IP c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
ll dotB(IP a, IP b) { return a.x * b.x + a.y * b.y; }

// every integer point on the segment [a, b]
vector<IP> latticeOn(IP a, IP b) {
    ll dx = b.x - a.x, dy = b.y - a.y, g = gcd(llabs(dx), llabs(dy));
    if (g == 0) return {a};
    vector<IP> r;
    rep(k, 0, g + 1) r.push_back({a.x + dx / g * k, a.y + dy / g * k});
    return r;
}
bool onLattice(IP q, IP a, IP b) {
    auto L = latticeOn(a, b);
    return find(all(L), q) != L.end();
}
// Do [a, b] and [c, d] meet? If the lines are not parallel they meet in one
// point, which has integer coordinates after scaling everything by |den|.
// If they are parallel (or a segment is a single point), a common point can
// be taken to be an endpoint. So: compare the integer points of both.
bool bruteSegs(IP a, IP b, IP c, IP d) {
    ll den = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
    ll s = den == 0 ? 1 : llabs(den);
    auto A = latticeOn(a * s, b * s), C = latticeOn(c * s, d * s);
    sort(all(A));
    for (IP p : C) if (binary_search(all(A), p)) return true;
    return false;
}

void testPoint() {
    using namespace I;
    rep(iter, 0, 5000) {
        P a = rndP(-9, 9), b = rndP(-9, 9), c = rndP(-9, 9);
        ll want = crossB(a, b, c);
        CHECK(cross(a, b, c) == want && cross(b, c, a) == want);
        CHECK(cross(b, a, c) == -want);
        CHECK(sgn(want) == (want > 0) - (want < 0));
        CHECK(dot(a, b) == dotB(a, b));
        CHECK(norm(a - b) == dotB(a - b, a - b));
        CHECK(fabs(len(a - b) - hypot(a.x - b.x, a.y - b.y)) < 1e-12);
        if (want != 0) {   // left turn: angle from b - a to c - a is in (0, pi)
            double t = atan2(c.y - a.y, c.x - a.x) - atan2(b.y - a.y, b.x - a.x);
            while (t <= -M_PI) t += 2 * M_PI;
            while (t > M_PI) t -= 2 * M_PI;
            CHECK((t > 0) == (want > 0));
        }
        CHECK(a + b - b == a && (a * 3) / 3 == a && (a < b) == (pii(a.x, a.y) < pii(b.x, b.y)));
    }
    CHECK(cross(P{0, 0}, P{1, 0}, P{0, 1}) > 0);   // left turn
    CHECK(cross(P{0, 0}, P{1, 0}, P{1, -1}) < 0);  // right turn
    CHECK(cross(P{0, 0}, P{2, 2}, P{5, 5}) == 0);
    ll big = 1000000000;   // the largest coordinates the comment promises
    CHECK(cross(P{-big, -big}, P{big, -big}, P{big, big}) == 4 * big * big);
    CHECK(D::sgn(1e-12) == 0 && D::sgn(-1e-6) == -1 && D::sgn(1e-6) == 1);
    istringstream in("3 -4  1.5 2");
    P p;
    in >> p;
    CHECK(p == (P{3, -4}) && len(p) == 5);
    DP q;
    in >> q;
    CHECK(q.x == 1.5 && q.y == 2);
    puts("  point, cross product, orientation OK");
}

void testSegments() {
    rep(iter, 0, 30000) {
        int r = rnd(1, 3);   // tiny ranges: many touching / collinear cases
        IP a = rndP(-r, r), b = rndP(-r, r), c = rndP(-r, r), d = rndP(-r, r);
        if (rnd(0, 3) == 0) {   // c and d on the line through a and b
            IP v = b - a;
            c = a + v * rnd(-2, 2), d = a + v * rnd(-2, 2);
        }
        bool want = bruteSegs(a, b, c, d);
        CHECK(I::segsIntersect(a, b, c, d) == want);
        CHECK(I::segsIntersect(d, c, b, a) == want);
        CHECK(D::segsIntersect(toD(a), toD(b), toD(c), toD(d)) == want);
        auto quarter = [](IP p) { return DP{p.x / 4.0, p.y / 4.0}; };
        CHECK(D::segsIntersect(quarter(a), quarter(b), quarter(c), quarter(d)) == want);
        CHECK(I::onSeg(c, a, b) == onLattice(c, a, b));
        CHECK(D::onSeg(toD(c), toD(a), toD(b)) == onLattice(c, a, b));
    }
    puts("  onSeg, segment intersection (ll and double) OK");
}

// closest point to p on {a + t (b - a) : lo <= t <= hi}: the distance is
// convex in t, so ternary search finds it
DP closestBrute(DP p, DP a, DP b, double lo, double hi) {
    auto f = [&](double t) { return D::len(a + (b - a) * t - p); };
    rep(i, 0, 300) {
        double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) hi = m2;
        else lo = m1;
    }
    return a + (b - a) * lo;
}

void testLines() {
    using namespace D;
    rep(iter, 0, 20000) {
        IP ia = rndP(-6, 6), ib = rndP(-6, 6), ic = rndP(-6, 6), id = rndP(-6, 6);
        P a = toD(ia), b = toD(ib), c = toD(ic), d = toD(id);
        ll den = (ib.x - ia.x) * (id.y - ic.y) - (ib.y - ia.y) * (id.x - ic.x);
        P x{1e9, 1e9};
        bool ok = lineInter(a, b, c, d, x);
        CHECK(ok == (den != 0));
        if (ok) {   // Cramer's rule for a + t u = c + s v
            double ux = b.x - a.x, uy = b.y - a.y, vx = d.x - c.x, vy = d.y - c.y;
            double wx = c.x - a.x, wy = c.y - a.y;
            double det = -ux * vy + vx * uy, t = (-wx * vy + vx * wy) / det;
            CHECK(fabs(x.x - (a.x + t * ux)) < 1e-9 && fabs(x.y - (a.y + t * uy)) < 1e-9);
            CHECK(fabs(cross(a, b, x)) < 1e-7 && fabs(cross(c, d, x)) < 1e-7);
        }
        IP ip = rndP(-6, 6);
        P p = toD(ip);
        CHECK(fabs(distSeg(p, a, b) - len(closestBrute(p, a, b, 0, 1) - p)) < 1e-7);
        CHECK(fabs(I::distSeg(ip, ia, ib) - distSeg(p, a, b)) < 1e-12);   // T = ll too
        if (ia == ib) continue;
        P cl = closestBrute(p, a, b, -1000, 1000);
        CHECK(fabs(distLine(p, a, b) - len(cl - p)) < 1e-7);
        CHECK(fabs(I::distLine(ip, ia, ib) - len(cl - p)) < 1e-7);
        P pr = project(p, a, b);
        CHECK(len(pr - cl) < 1e-6);
    }
    puts("  line intersection, projection, distance to line / segment OK");
}

// a random simple polygon with distinct integer vertices in [0, R]^2:
// random points sorted by angle around a random centre, kept if no two
// edges touch (except neighbours at their common vertex)
vector<IP> randSimplePolygon(int R) {
    while (true) {
        int k = rnd(3, 8);
        vector<IP> p;
        rep(i, 0, k) p.push_back(rndP(0, R));
        sort(all(p));
        if (unique(all(p)) != p.end()) continue;
        double cx = rnd(0, 10 * R) / 10.0 + 0.03, cy = rnd(0, 10 * R) / 10.0 + 0.07;
        auto ang = [&](IP u) { return atan2(u.y - cy, u.x - cx); };
        sort(all(p), [&](IP u, IP v) { return ang(u) < ang(v); });
        bool simple = true;
        rep(i, 0, k) rep(j, i + 1, k) {
            IP a = p[i], b = p[(i + 1) % k], c = p[j], d = p[(j + 1) % k];
            if (j == i + 1 || (i == 0 && j == k - 1)) {   // neighbours
                auto A = latticeOn(a, b);
                int common = 0;
                for (IP q : latticeOn(c, d)) common += find(all(A), q) != A.end();
                if (common > 1) simple = false;
            } else if (bruteSegs(a, b, c, d)) simple = false;
        }
        if (simple) return p;
    }
}
// winding number of the polygon around q (0 = outside), or 2 if q is on
// the border. Sums the angles under which q sees each edge.
int windingBrute(const vector<IP>& p, IP q) {
    int k = p.size();
    double total = 0;
    rep(i, 0, k) {
        IP a = p[i], b = p[(i + 1) % k];
        if (onLattice(q, a, b)) return 2;
        total += atan2((double)crossB(q, a, b), (double)dotB(a - q, b - q));
    }
    return (int)lround(total / (2 * M_PI));
}

void testPolygon() {
    rep(iter, 0, 1500) {
        int R = rnd(2, 6);
        vector<IP> p = randSimplePolygon(R);
        if (rnd(0, 1)) reverse(all(p));   // both orientations
        if (rnd(0, 2) == 0) for (IP& v : p) v = v * 2;   // half-integer queries
        rotate(p.begin(), p.begin() + rnd(0, (int)p.size() - 1), p.end());
        vector<DP> pd;
        for (IP v : p) pd.push_back(toD(v));
        ll inside = 0, border = 0, wind = 0;
        rep(x, -1, 2 * R + 2) rep(y, -1, 2 * R + 2) {
            IP q{x, y};
            int w = windingBrute(p, q);
            CHECK(w == 2 || w == 0 || w == 1 || w == -1);
            int want = w == 2 ? 0 : (w == 0 ? -1 : 1);
            CHECK(I::inPolygon(p, q) == want);
            CHECK(D::inPolygon(pd, toD(q)) == want);
            if (w == 2) border++;
            else if (w != 0) inside++, wind = w;
        }
        ll a2 = I::area2(p);
        CHECK(llabs(a2) == 2 * inside + border - 2);   // Pick's theorem
        if (wind != 0) CHECK((a2 > 0) == (wind > 0));  // > 0 iff counter-clockwise
        CHECK(D::area2(pd) == a2);
        reverse(all(p));
        CHECK(I::area2(p) == -a2);
    }
    puts("  polygon area (Pick), point in polygon OK");
}

void testHull() {
    rep(iter, 0, 6000) {
        int n = rnd(1, 15), R = rnd(0, 1) ? 3 : 30;
        vector<IP> pts(n);
        for (IP& p : pts) p = rndP(-R, R);
        if (rnd(0, 7) == 0) {   // all on one line
            IP a = rndP(-3, 3), v = rndP(-2, 2);
            for (IP& p : pts) p = a + v * rnd(-3, 3);
        }
        vector<IP> h = I::hull(pts);
        vector<IP> d = pts;
        sort(all(d));
        d.erase(unique(all(d)), d.end());
        int m = h.size();
        vector<IP> hs = h;
        sort(all(hs));
        CHECK(unique(all(hs)) == hs.end());   // no vertex twice
        for (IP v : h) CHECK(binary_search(all(d), v));   // only input points
        bool line = true;
        for (IP q : d) if (crossB(d[0], d.back(), q) != 0) line = false;
        if (d.size() == 1) CHECK(m == 1);
        else if (line) CHECK(m == 2 && h[0] == d[0] && h[1] == d.back());
        else {
            CHECK(m >= 3);
            rep(i, 0, m) {
                IP a = h[i], b = h[(i + 1) % m], c = h[(i + 2) % m];
                CHECK(crossB(a, b, c) > 0);   // every vertex is a real corner
                for (IP q : pts) CHECK(crossB(a, b, q) >= 0);   // all inside or on
            }
            CHECK(I::area2(h) > 0);
        }
    }
    puts("  convex hull OK");
}

ll closestBruteForce(const vector<IP>& p) {
    ll best = LLONG_MAX;
    rep(i, 0, (int)p.size()) rep(j, i + 1, (int)p.size())
        best = min(best, dotB(p[i] - p[j], p[i] - p[j]));
    return best;
}

void testClosest() {
    rep(iter, 0, 3000) {
        int n = rnd(2, 40);
        ll R = vector<ll>{3, 1000, 1000000000}[rnd(0, 2)];
        vector<IP> p(n);
        for (IP& q : p) q = {rndll(-R, R), rndll(-R, R)};
        CHECK(I::closestPair(p) == closestBruteForce(p));
    }
    rep(iter, 0, 6) {   // bigger: random, on one vertical line, on a grid
        int n = 3000;
        vector<IP> p(n);
        rep(i, 0, n) {
            if (iter < 2) p[i] = {rndll(-1e9, 1e9), rndll(-1e9, 1e9)};
            else if (iter < 4) p[i] = {7, rndll(-1e9, 1e9)};
            else p[i] = {rnd(0, 60) * 1000LL, rnd(0, 60) * 1000LL};
        }
        CHECK(I::closestPair(p) == closestBruteForce(p));
    }
    puts("  closest pair OK");
}

void testAngles() {
    auto key = [](IP p) {   // direction angle in [0, 2 pi)
        double t = atan2(p.y, p.x);
        return t < 0 ? t + 2 * M_PI : t;
    };
    rep(iter, 0, 1000) {
        vector<IP> v;
        rep(i, 0, rnd(1, 25)) {
            IP p = rndP(-4, 4);
            if (!(p == IP{0, 0})) v.push_back(p);
        }
        for (IP a : v) for (IP b : v) {
            double c = dotB(a, b) / sqrt((double)dotB(a, a) * dotB(b, b));
            double want = acos(max(-1.0, min(1.0, c)));
            CHECK(fabs(I::angle(a, b) - want) < 1e-6);
            CHECK(fabs(D::angle(toD(a), toD(b)) - want) < 1e-6);
            bool same = crossB({0, 0}, a, b) == 0 && dotB(a, b) > 0;
            bool less = !same && key(a) < key(b);
            CHECK(I::byAngle(a, b) == less);
            CHECK(D::byAngle(toD(a), toD(b)) == less);
        }
        sort(all(v), I::byAngle);
        rep(i, 0, (int)v.size() - 1) CHECK(key(v[i]) <= key(v[i + 1]) + 1e-12);
    }
    rep(iter, 0, 5000) {   // rotation against complex multiplication
        DP p{rnd(-1000, 1000) / 7.0, rnd(-1000, 1000) / 7.0};
        double t = rnd(-10000, 10000) / 1000.0;
        complex<double> w = complex<double>(p.x, p.y) * polar(1.0, t);
        DP r = D::rot(p, t);
        CHECK(fabs(r.x - w.real()) < 1e-9 && fabs(r.y - w.imag()) < 1e-9);
    }
    DP r = D::rot(DP{2, 1}, M_PI / 2);   // the exact 90 degree turn of the comment
    CHECK(fabs(r.x - -1) < 1e-12 && fabs(r.y - 2) < 1e-12);
    puts("  angle, sort by angle, rotation OK");
}

void testCircles() {
    using namespace D;
    rep(iter, 0, 30000) {   // integer input, so the case is decided exactly
        IP ia = rndP(-6, 6), ib = rndP(-6, 6), ic = rndP(-6, 6);
        if (ia == ib) continue;
        ll r = rnd(1, 6);
        P a = toD(ia), b = toD(ib), c = toD(ic);
        // squared distance from c to the line = cr^2 / |b - a|^2
        ll cr = crossB(ia, ib, ic), l2 = dotB(ib - ia, ib - ia);
        size_t want = cr * cr > r * r * l2 ? 0 : (cr * cr == r * r * l2 ? 1 : 2);
        auto pts = lineCircle(a, b, c, r);
        CHECK(pts.size() == want);
        for (P q : pts) {
            CHECK(fabs(len(q - c) - r) < 1e-9);
            CHECK(fabs(cross(a, b, q)) < 1e-9);
        }
        if (want == 2) CHECK(len(pts[0] - pts[1]) > 1e-6);
    }
    rep(iter, 0, 30000) {
        IP ia = rndP(-6, 6), ib = rndP(-6, 6);
        ll r1 = rnd(1, 7), r2 = rnd(1, 7);
        ll d2 = dotB(ia - ib, ia - ib), hi = (r1 + r2) * (r1 + r2), lo = (r1 - r2) * (r1 - r2);
        size_t want = d2 == 0 || d2 > hi || d2 < lo ? 0 : (d2 == hi || d2 == lo ? 1 : 2);
        auto pts = circleCircle(toD(ia), r1, toD(ib), r2);
        CHECK(pts.size() == want);
        for (P q : pts) {
            CHECK(fabs(len(q - toD(ia)) - r1) < 1e-9);
            CHECK(fabs(len(q - toD(ib)) - r2) < 1e-9);
        }
        if (want == 2) CHECK(len(pts[0] - pts[1]) > 1e-6);
    }
    puts("  line-circle, circle-circle OK");
}

int main() {
    testPoint();
    testSegments();
    testLines();
    testPolygon();
    testHull();
    testClosest();
    testAngles();
    testCircles();
}
