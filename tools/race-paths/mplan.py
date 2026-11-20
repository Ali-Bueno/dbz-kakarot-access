# Layered route planner over the multi-layer survey (`msurvey`, dumps/msurvey_race03.txt).
# A cell can hold several surfaces (ground under an arch, the arch deck, a bone over the ground),
# so a node is (cell, surface). Robot rules measured live 2026-11-20 (`rmove`): capsule r 1 m,
# 2.6 m tall, floors up to 45 degrees; a jump lifts ~5 m and carries a few metres; boosters 4-20 m
# over the ground launch it when it jumps standing under them.
#   python mplan.py            -> mlegs.json (+ prints each leg)
import math, json, heapq, sys
import numpy as np
from load import D, X0, Y0, STEP, NX, NY, course, ij

L = 3
Z = np.full((L, NY, NX), np.nan); NZ = np.zeros((L, NY, NX)); HD = np.full((L, NY, NX), -1.0)
for line in open(D + "msurvey_race03.txt"):
    x, y, z, nz, layer, head = line.split()
    i = (int(float(y)) - Y0) // STEP; j = (int(float(x)) - X0) // STEP; l = int(layer) - 1
    if 0 <= i < NY and 0 <= j < NX and l < L:
        Z[l, i, j] = float(z); NZ[l, i, j] = float(nz); HD[l, i, j] = float(head)

FLOOR_NZ = 0.35          # a single trace normal on rough rock is noise; slope is judged between cells
BODY = 700               # clearance (cm): the capsule is 2.6 m but robot + rider catch on ceilings ~5 m up (live)
DROP = 3000              # walk off a ledge onto ground at most this far below (cm)
JUMP_UP = 1200           # a jump lifts 15 m (live `rtrack`, standing and moving); margin kept
JUMP_MIN, JUMP_MAX = 2, 8    # take-off to the first landing cell, in cells (4-16 m)
RUNWAY = 10              # a jump carries 20-36 m at speed: the next RUNWAY cells must be safe too
JUMP_COST = 30.0         # in cells: walk unless a jump saves a lot
EDGE_PEN = [8.0, 4.0, 2.0, 1.0]   # cost multiplier by distance (cells) to an edge, then 0

# Water (same hulls as plan.py) per surface, and the edge distances: slow, so cached next to the
# survey (rebuilt when the survey is newer).
import os
CACHE = D + "mplan_cache.npz"
CACHED = os.path.exists(CACHE) and os.path.getmtime(CACHE) > os.path.getmtime(D + "msurvey_race03.txt")
if CACHED:
    _c = np.load(CACHE); WET = _c["wet"]; EDGE_C = _c["edge"]
import plan as _p
water = _p.water
def wet(x, y, z):
    for nm, poly, ztop in water:
        if z < ztop + 300 and _p.dist_poly(x, y, poly) <= 600: return True
    return False
if not CACHED:
  WET = np.zeros((L, NY, NX), bool)
  for nm, poly, ztop in water:
      xs = [p[0] for p in poly]; ys = [p[1] for p in poly]
      i0, j0 = ij(min(xs) - 700, min(ys) - 700); i1, j1 = ij(max(xs) + 700, max(ys) + 700)
      for i in range(max(0, i0), min(NY, i1 + 1)):
          for j in range(max(0, j0), min(NX, j1 + 1)):
              for l in range(L):
                  z = Z[l, i, j]
                  if not np.isnan(z) and z < ztop + 300 and _p.dist_poly(X0 + j*STEP, Y0 + i*STEP, poly) <= 600:
                      WET[l, i, j] = True

OK = (~np.isnan(Z)) & (NZ >= FLOOR_NZ) & ((HD < 0) | (HD >= BODY)) & ~WET

def ceiling(l, i, j):
    h = HD[l, i, j]
    return 1e9 if h < 0 else Z[l, i, j] + h

def land(z, a, b, up):
    """The surface the robot ends on entering column (a, b) at height z, climbing at most `up`:
    the highest walkable one not above z + up whose ceiling clears the body. None if blocked."""
    best = None
    for l in range(L):
        zz = Z[l, a, b]
        if np.isnan(zz) or zz > z + up: continue
        if ceiling(l, a, b) < max(z, zz) + BODY: continue
        if best is None or zz > Z[best, a, b]: best = l
    if best is None or not OK[best, a, b]: return None
    if z - Z[best, a, b] > DROP: return None
    return best

NB = [(1,0,1.0),(-1,0,1.0),(0,1,1.0),(0,-1,1.0),(1,1,1.414),(1,-1,1.414),(-1,1,1.414),(-1,-1,1.414)]

# Distance to an edge: a node whose neighbours are not all reachable on the level (|dz| <= 1.5 m).
print("edge distances..." if not CACHED else "cache loaded", flush=True)
EDGE = EDGE_C if CACHED else np.full((L, NY, NX), 9, np.int8)
from collections import deque
q = deque()
for l in (range(L) if not CACHED else ()):
    ii, jj = np.nonzero(OK[l])
    for i, j in zip(ii, jj):
        z = Z[l, i, j]; flat = 0
        for di, dj, c in NB:
            a, b = i + di, j + dj
            if 0 <= a < NY and 0 <= b < NX:
                m = land(z, a, b, 200 * c)
                if m is not None and abs(Z[m, a, b] - z) <= 200 * c: flat += 1
        if flat < 8: EDGE[l, i, j] = 0; q.append((l, i, j))
while q:
    l, i, j = q.popleft(); d = EDGE[l, i, j]
    if d >= len(EDGE_PEN): continue
    z = Z[l, i, j]
    for di, dj, _ in NB:
        a, b = i + di, j + dj
        if 0 <= a < NY and 0 <= b < NX:
            m = land(z, a, b, 200)
            if m is not None and abs(Z[m, a, b] - z) <= 200 and EDGE[m, a, b] > d + 1: EDGE[m, a, b] = d + 1; q.append((m, a, b))
if not CACHED: np.savez(CACHE, wet=WET, edge=EDGE)
def pen(l, i, j):
    d = EDGE[l, i, j]
    return EDGE_PEN[d] if d < len(EDGE_PEN) else 0.0

LAUNCH = {}
def add_launches(boosts):
    for nm, bx, by, bz in boosts:
        bi, bj = ij(bx, by)
        if not (0 <= bi < NY and 0 <= bj < NX): continue
        src = None
        for l in range(L):
            zz = Z[l, bi, bj]
            if OK[l, bi, bj] and 400 <= bz - zz <= 2000: src = (l, bi, bj)
        if not src: continue
        g = Z[src]
        lands = []
        for a in range(bi - 15, bi + 16):
            for b in range(bj - 15, bj + 16):
                if not (0 <= a < NY and 0 <= b < NX): continue
                d = math.hypot(a - bi, b - bj)
                if d > 15: continue
                for l in range(L):
                    zz = Z[l, a, b]
                    if OK[l, a, b] and g + 1500 < zz <= bz + 5500 and EDGE[l, a, b] >= 2:
                        lands.append(((l, a, b), 40.0 + d))
        if lands: LAUNCH[src] = lands

def land_any(z, a, b):
    """Highest surface (walkable or not) at or below z whose ceiling clears the body."""
    best = None
    for l in range(L):
        zz = Z[l, a, b]
        if np.isnan(zz) or zz > z: continue
        if ceiling(l, a, b) < z + BODY: continue
        if best is None or zz > Z[best, a, b]: best = l
    return best

PEAK_T, JUMP_H = 0.9, 1500.0             # live `rtrack`: +15 m, peak 0.9 s after take-off
GRAV = 2 * JUMP_H / PEAK_T ** 2
SPEEDS = (1200.0, 800.0)                 # air speed (cm/s): calibrated on the live crack jump, ~20 m; both must land

def fly(u, di, dj, v):
    """Simulate a jump from node u heading (di, dj) at speed v: the landing node, or None."""
    l, i, j = u; z0 = Z[u]
    nrm = math.hypot(di, dj); ux, uy = dj / nrm, di / nrm
    x0, y0 = X0 + j * STEP, Y0 + i * STEP
    t, dt, prev_z = 0.0, 0.04, z0
    while t < 3.0:
        t += dt
        x, y = x0 + ux * v * t, y0 + uy * v * t
        zt = z0 + JUMP_H - 0.5 * GRAV * (t - PEAK_T) ** 2 - (JUMP_H - 0.5 * GRAV * PEAK_T ** 2)
        a, b = ij(x, y)
        if not (0 <= a < NY and 0 <= b < NX): return None
        if (a, b) == (i, j): prev_z = zt; continue
        m = land_any(max(zt, prev_z) + 1, a, b)
        if m is None: return None                       # flew into a wall
        if Z[m, a, b] >= zt:                             # the feet reach it: landed
            return (m, a, b) if OK[m, a, b] else None
        prev_z = zt
    return None

def jumps(u, di, dj, c, why=None):
    """A jump from u in direction (di, dj), only where walking on is impossible (a crack, a step,
    a wall): the simulated arc must land on walkable ground off the edge at both speeds."""
    l, i, j = u; z = Z[u]
    a1, b1 = i + di, j + dj
    if not (0 <= a1 < NY and 0 <= b1 < NX): return []
    m1 = land(z, a1, b1, 200 * c)
    if m1 is not None and abs(Z[m1, a1, b1] - z) < 300:
        if why is not None: why.append("walkable ahead")
        return []
    lands = []
    for v in SPEEDS:
        n = fly(u, di, dj, v)
        if n is None or EDGE[n] < 1:
            if why is not None: why.append("v%d: %s" % (v / 100, "no landing" if n is None else "lands on an edge at %d,%d z%d" % ((X0 + n[2]*STEP) / 100, (Y0 + n[1]*STEP) / 100, Z[n] / 100)))
            return []
        lands.append(n)
    n = lands[-1]                                  # the nearer landing (slower run)
    d = math.hypot(n[1] - i, n[2] - j)
    return [(n, d + JUMP_COST + 2 * pen(*n))]

def astar(src, dst):
    gi, gj = dst[1], dst[2]
    g = {src: 0.0}; prev = {}; how = {}
    pq = [(0.0, src)]
    while pq:
        f, u = heapq.heappop(pq)
        l, i, j = u
        if abs(i - gi) <= 1 and abs(j - gj) <= 1 and abs(Z[u] - Z[dst]) < 400:
            path = [u]
            while path[-1] in prev: path.append(prev[path[-1]])
            path.reverse()
            return path, how
        gu = g[u]
        if f - math.hypot(i - gi, j - gj) > gu + 1e-6: continue
        z = Z[u]
        def relax(v, c, kind):
            ng = gu + c
            if ng < g.get(v, 1e18):
                g[v] = ng; prev[v] = u; how[v] = kind
                heapq.heappush(pq, (ng + math.hypot(v[1] - gi, v[2] - gj), v))
        for di, dj, c in NB:
            a, b = i + di, j + dj
            if not (0 <= a < NY and 0 <= b < NX): continue
            m = land(z, a, b, 200 * c)          # 45 degrees: rise <= run
            if m is not None:
                relax((m, a, b), c * (1.0 + pen(m, a, b)), "walk")
        for di, dj, c in NB:                     # jumps where walking on is impossible
            for v, cost in jumps(u, di, dj, c):
                relax(v, cost, "jump")
        for v, lc in LAUNCH.get(u, ()):
            relax(v, lc, "launch")
    return None, None

def node_near(x, y, z, r=4):
    ci, cj = ij(x, y); best = None
    for a in range(ci - r, ci + r + 1):
        for b in range(cj - r, cj + r + 1):
            for l in range(L):
                if 0 <= a < NY and 0 <= b < NX and OK[l, a, b]:
                    s = abs(Z[l, a, b] - z) / 100 + math.hypot(a - ci, b - cj) + pen(l, a, b)
                    if best is None or s < best[0]: best = (s, (l, a, b))
    return best and best[1]

def xyz(v): return [X0 + v[2] * STEP, Y0 + v[1] * STEP, float(Z[v])]

def rdp(pts, eps):
    if len(pts) < 3: return pts
    (x0, y0), (x1, y1) = pts[0][:2], pts[-1][:2]
    dx, dy = x1 - x0, y1 - y0; Lh = math.hypot(dx, dy) or 1
    k, dm = 0, -1
    for n in range(1, len(pts) - 1):
        d = abs((pts[n][0] - x0) * dy - (pts[n][1] - y0) * dx) / Lh
        if d > dm: k, dm = n, d
    if dm <= eps: return [pts[0], pts[-1]]
    return rdp(pts[:k + 1], eps)[:-1] + rdp(pts[k:], eps)

def leg(name, a, b):
    s = node_near(*a); t = node_near(*b)
    path, how = astar(s, t)
    if not path: print(name, "NO PATH"); return None
    pts = [xyz(v) for v in path]
    cuts = [0]
    for k in range(1, len(path)):
        kind = how.get(path[k])
        if kind in ("jump", "launch"):
            pts[k - 1].append("j" if kind == "jump" else "launch")
            if k - 1 > cuts[-1]: cuts.append(k - 1)
            cuts.append(k)
    cuts.append(len(path) - 1)
    out = []
    for p, q2 in zip(cuts, cuts[1:]):
        part = rdp(pts[p:q2 + 1], 150)
        out += part if not out else part[1:]
    Lm = sum(math.hypot(out[k+1][0]-out[k][0], out[k+1][1]-out[k][1]) for k in range(len(out)-1)) / 100
    print(name, len(out), "points", round(Lm), "m,", sum(1 for w in out if "j" in w[3:]), "jumps,",
          sum(1 for w in out if "launch" in w[3:]), "launches", flush=True)
    return out

if __name__ == "__main__":
    cps, boosts, arrows = course()
    add_launches(boosts)
    print("launch pads:", len(LAUNCH), flush=True)
    which = sys.argv[1:] or ["1", "2", "3", "4", "5"]
    legs = []
    for k in range(1, 6):
        if str(k) not in which: continue
        a = cps[k - 1][1:]; b = cps[k][1:]
        out = leg("L%d" % k, a, b)
        if out: legs.append(("L%d" % k, out))
    json.dump(legs, open("mlegs.json", "w"))
