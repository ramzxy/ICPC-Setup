// is p on the segment [a, b]? (the endpoints count)
bool onSeg(P p, P a, P b) {
    return sgn(cross(a, b, p)) == 0 && sgn(dot(a - p, b - p)) <= 0;
}
// do segments [a, b] and [c, d] have a common point? Touching and
// overlapping count. Exact with T = ll
bool segsIntersect(P a, P b, P c, P d) {
    int s1 = sgn(cross(a, b, c)), s2 = sgn(cross(a, b, d));
    int s3 = sgn(cross(c, d, a)), s4 = sgn(cross(c, d, b));
    if (s1 * s2 < 0 && s3 * s4 < 0) return true;  // a proper X
    return onSeg(c, a, b) || onSeg(d, a, b) ||
           onSeg(a, c, d) || onSeg(b, c, d);
}
