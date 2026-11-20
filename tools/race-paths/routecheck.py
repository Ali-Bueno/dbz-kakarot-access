# Walk a leg every metre over the surveys and flag climbs the robot cannot walk (capsule r 1 m,
# MaxStepHeight 0.45 m, WalkableFloorAngle 45 deg, live `rmove` 2026-11-20). Where a low survey
# (traces started just above the racer) exists, the height nearer the route's own height wins:
# that drops bone tops and keeps arch decks.
import sys, json, math, glob, os
from load import *
H, N = grid()
LOW = {}
for f in glob.glob(D + "survey_low*.txt"):
    for line in open(f):
        x, y, z = [float(v) for v in line.split()[:3]]
        LOW[(int(round(x / 200)), int(round(y / 200)))] = z
def h(x, y, zref):
    top = H[ij(x, y)]
    low = LOW.get((int(round(x / 200)), int(round(y / 200))))
    if low is None: return top
    return low if abs(low - zref) < abs(top - zref) else top
name = sys.argv[1]
pts = dict(json.load(open("legs.json")))[name]
prev = None
for k in range(len(pts) - 1):
    a, b = pts[k], pts[k + 1]
    L = math.hypot(b[0] - a[0], b[1] - a[1]); n = max(1, int(L / 100))
    for s in range(n + 1):
        t = s / n
        x, y, zr = a[0] + t * (b[0] - a[0]), a[1] + t * (b[1] - a[1]), a[2] + t * (b[2] - a[2])
        z = h(x, y, zr)
        if prev is not None:
            rise = z - prev[2]; run = math.hypot(x - prev[0], y - prev[1])
            jump = len(a) > 3 and s == n   # stepping off a jump point
            if run > 1 and rise > max(45, run) and not jump and not (len(b) > 3):
                print("seg %d->%d at %d,%d: rise %.1f m over %.1f m (z %d)" % (k, k + 1, x / 100, y / 100, rise / 100, run / 100, z / 100))
            if abs(z - zr) > 600:
                print("seg %d->%d at %d,%d: ground %d m, route %d m" % (k, k + 1, x / 100, y / 100, z / 100, zr / 100))
        prev = (x, y, z)
