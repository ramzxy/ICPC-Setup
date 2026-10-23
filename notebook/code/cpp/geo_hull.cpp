// convex hull, counter-clockwise, without points in the middle of
// an edge. Fewer than 3 points if all points are on one line
vector<P> hull(vector<P> pts) {
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());
    if (pts.size() < 3) return pts;
    vector<P> h;
    rep(pass, 0, 2) {   // lower half, then upper half
        int start = h.size();
        for (P p : pts) {
            // drop the last point while it does not turn left
            while ((int)h.size() >= start + 2 &&
                   sgn(cross(h[h.size() - 2], h.back(), p)) <= 0)
                h.pop_back();
            h.push_back(p);
        }
        h.pop_back();   // it starts the other half
        reverse(all(pts));
    }
    return h;
}
