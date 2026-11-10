// 2D point / vector. T = ll: exact integer math (when the input is
// integer). T = double: decimals in the input, or you need division
using T = ll;
const double EPS = 1e-9;
int sgn(T v) { return (v > EPS) - (v < -EPS); }  // -1, 0 or 1
struct P {
    T x, y;
    P operator+(P o) const { return {x + o.x, y + o.y}; }
    P operator-(P o) const { return {x - o.x, y - o.y}; }
    P operator*(T k) const { return {x * k, y * k}; }
    P operator/(T k) const { return {x / k, y / k}; }
    bool operator<(P o) const { return tie(x, y) < tie(o.x, o.y); }
    bool operator==(P o) const { return x == o.x && y == o.y; }
};
istream& operator>>(istream& in, P& p) { return in >> p.x >> p.y; }
T dot(P a, P b) { return a.x * b.x + a.y * b.y; }
T cross(P a, P b) { return a.x * b.y - a.y * b.x; }
T norm(P a) { return dot(a, a); }   // squared length (exact)
double len(P a) { return sqrt((double)norm(a)); }
// > 0: a -> b -> c turns left (counter-clockwise), < 0: turns
// right, 0: all on one line. |cross(a, b, c)| = 2 * triangle area
T cross(P a, P b, P c) { return cross(b - a, c - a); }
