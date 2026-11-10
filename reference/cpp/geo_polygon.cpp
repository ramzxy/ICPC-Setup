// twice the signed area of a polygon (vertices in order). > 0 if
// counter-clockwise. For integer points this is an exact integer
T area2(const vector<P>& p) {
    int n = p.size();
    T s = 0;
    rep(i, 0, n) s += cross(p[i], p[(i + 1) % n]);
    return s;
}
// is q in the polygon (any simple one, convex or not)? O(n)
// 1 = inside, 0 = on the border, -1 = outside
int inPolygon(const vector<P>& p, P q) {
    int n = p.size();
    bool in = false;
    rep(i, 0, n) {
        P a = p[i], b = p[(i + 1) % n];
        if (onSeg(q, a, b)) return 0;
        if (a.y > b.y) swap(a, b);   // now a is the lower end
        // does this edge cross the ray from q to the right?
        if (a.y <= q.y && q.y < b.y && sgn(cross(a, b, q)) > 0)
            in = !in;
    }
    return in ? 1 : -1;
}
