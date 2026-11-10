// T = double. Where the line a-b meets the circle (c, r): 0, 1 or
// 2 points
vector<P> lineCircle(P a, P b, P c, double r) {
    P d = b - a;
    P m = a + d * (dot(c - a, d) / norm(d));   // closest to c
    double h2 = r * r - norm(c - m);
    if (h2 < -EPS) return {};
    if (h2 < EPS) return {m};   // the line touches the circle
    P off = d * (sqrt(h2) / len(d));
    return {m - off, m + off};
}
// T = double. Where the circles (a, r1) and (b, r2) meet
vector<P> circleCircle(P a, double r1, P b, double r2) {
    P v = b - a;
    double d2 = norm(v);
    if (d2 < EPS) return {};   // same centre
    double x = (d2 + r1 * r1 - r2 * r2) / (2 * d2);  // part of v
    double h2 = r1 * r1 / d2 - x * x;
    if (h2 < -EPS) return {};
    P m = a + v * x, off = P{-v.y, v.x} * sqrt(max(0.0, h2));
    if (h2 < EPS) return {m};
    return {m - off, m + off};
}
