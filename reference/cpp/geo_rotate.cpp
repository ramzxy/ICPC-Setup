// T = double. Rotate p counter-clockwise by t radians around the
// origin. Around a centre c: rot(p - c, t) + c
P rot(P p, double t) {
    double c = cos(t), s = sin(t);
    return {p.x * c - p.y * s, p.x * s + p.y * c};
}
// turn 90 degrees left, exact: {-p.y, p.x}. Right: {p.y, -p.x}
