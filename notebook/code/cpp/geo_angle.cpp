// angle between vectors a and b, in [0, pi] (radians)
double angle(P a, P b) {
    return atan2(abs(cross(a, b)), dot(a, b));
}
// sort vectors by direction, counter-clockwise, starting at the
// +x axis: sort(all(v), byAngle). Exact with T = ll. No (0, 0)!
bool lowHalf(P a) { return a.y < 0 || (a.y == 0 && a.x < 0); }
bool byAngle(P a, P b) {
    if (lowHalf(a) != lowHalf(b)) return lowHalf(b);
    return sgn(cross(a, b)) > 0;
}
