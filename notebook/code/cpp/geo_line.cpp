// T = double. Point where the infinite lines a-b and c-d cross;
// false if they are parallel. Two segments: segsIntersect first
bool lineInter(P a, P b, P c, P d, P& out) {
    T den = cross(b - a, d - c);
    if (sgn(den) == 0) return false;
    out = a + (b - a) * (cross(c - a, d - c) / den);
    return true;
}
// T = double. Point on the line a-b closest to p
P project(P p, P a, P b) {
    return a + (b - a) * (dot(p - a, b - a) / norm(b - a));
}
// distance from p to the infinite line a-b (a != b)
double distLine(P p, P a, P b) {
    return abs(cross(a, b, p)) / len(b - a);
}
// distance from p to the segment [a, b]
double distSeg(P p, P a, P b) {
    if (a == b || dot(p - a, b - a) <= 0) return len(p - a);
    if (dot(p - b, a - b) <= 0) return len(p - b);
    return distLine(p, a, b);
}
// segment to segment: 0 if segsIntersect, else the minimum of the
// 4 distances from an endpoint to the other segment
