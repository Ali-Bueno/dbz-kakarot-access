from load import *
import heapq, math, json, sys
H, N = grid()
UP = 120.0            # max climb per 2 m step without jumping (cm)
JUMP_UP = 300.0       # a ledge up to this high is climbed with a jump (cm); live: a jump lifts ~5 m
JUMP_COST = 25.0      # extra cost of a jump, in cells
CLEAR = 3             # cells of wall clearance penalised
# --- water: ATWaterVolume hulls ---
vols = {}; cur = None
for line in open(D + "volumes_ATWaterVolume.txt"):
    p = line.split()
    if p[0] == "vol": cur = p[1]; vols[cur] = {"loc": tuple(map(float, p[3:6])), "yaw": float(p[8]), "v": []}
    else: vols[cur]["v"].append((int(p[1]),) + tuple(map(float, p[2:5])))
def hull(pts):
    pts = sorted(set(pts))
    def cr(o, a, b): return (a[0]-o[0])*(b[1]-o[1]) - (a[1]-o[1])*(b[0]-o[0])
    lo, up = [], []
    for p in pts:
        while len(lo) >= 2 and cr(lo[-2], lo[-1], p) <= 0: lo.pop()
        lo.append(p)
    for p in reversed(pts):
        while len(up) >= 2 and cr(up[-2], up[-1], p) <= 0: up.pop()
        up.append(p)
    return lo[:-1] + up[:-1]
water = []
for nm, v in vols.items():
    lx, ly, lz = v["loc"]; yw = math.radians(v["yaw"])
    for e in sorted(set(q[0] for q in v["v"])):
        ev = [q[1:] for q in v["v"] if q[0] == e]
        w = [(lx + x*math.cos(yw) - y*math.sin(yw), ly + x*math.sin(yw) + y*math.cos(yw)) for x, y, z in ev]
        ztop = lz + max(z for _, _, z in ev)
        water.append((nm + "#" + str(e), hull(w), ztop))
def dist_poly(x, y, poly):   # signed-ish: 0 inside, else distance to polygon
    inside = True
    for k in range(len(poly)):
        a, b = poly[k], poly[(k+1) % len(poly)]
        if (b[0]-a[0])*(y-a[1]) - (b[1]-a[1])*(x-a[0]) < 0: inside = False
    if inside: return 0.0
    best = 1e18
    for k in range(len(poly)):
        a, b = poly[k], poly[(k+1) % len(poly)]
        dx, dy = b[0]-a[0], b[1]-a[1]; L = dx*dx + dy*dy
        t = max(0, min(1, ((x-a[0])*dx + (y-a[1])*dy) / L)) if L else 0
        best = min(best, math.hypot(x - a[0] - t*dx, y - a[1] - t*dy))
    return best
WET = np.zeros(H.shape, bool)
for nm, poly, ztop in water:
    xs = [p[0] for p in poly]; ys = [p[1] for p in poly]
    if max(xs) < X0 or min(xs) > X0 + NX*STEP or max(ys) < Y0 or min(ys) > Y0 + NY*STEP: continue
    for i in range(NY):
        y = Y0 + i*STEP
        if y < min(ys) - 1500 or y > max(ys) + 1500: continue
        for j in range(NX):
            x = X0 + j*STEP
            if x < min(xs) - 1500 or x > max(xs) + 1500: continue
            if H[i, j] < ztop + 300 and dist_poly(x, y, poly) <= 600: WET[i, j] = True
# --- walls: a cell next to a too-steep up-step ---
WALL = np.zeros(H.shape, bool)
for di, dj in ((1,0),(-1,0),(0,1),(0,-1)):
    sh = np.roll(np.roll(H, di, 0), dj, 1)
    WALL |= (sh - H) > JUMP_UP   # a wall: higher than a jump
near = np.zeros(H.shape, np.int8)
for di in range(-CLEAR, CLEAR+1):
    for dj in range(-CLEAR, CLEAR+1):
        near |= np.roll(np.roll(WALL, di, 0), dj, 1).astype(np.int8)
# Hard clearance: the robot's legs span several metres; nothing within 4 m of a wall's foot.
TIGHT = np.zeros(H.shape, bool)
for di in range(-2, 3):
    for dj in range(-2, 3):
        if di * di + dj * dj <= 4:
            TIGHT |= np.roll(np.roll(WALL, di, 0), dj, 1)
NB = [(1,0,1.0),(-1,0,1.0),(0,1,1.0),(0,-1,1.0),(1,1,1.414),(1,-1,1.414),(-1,1,1.414),(-1,-1,1.414)]
JUMPED = {}
LAUNCH = {}            # cell -> list of (landing cell, cost): stop under a floating booster and jump
LAUNCH_R = 3000        # landing within this horizontal distance of the booster (cm)
LAUNCH_UP = 5500       # landing ground at most this far above the booster (cm)
def add_launches(boosts):
    for nm, bx, by, bz in boosts:
        bi, bj = ij(bx, by)
        if not (0 <= bi < NY and 0 <= bj < NX): continue
        lift = bz - H[bi, bj]
        if lift < 400 or lift > 2000: continue
        src = (bi, bj)
        r = int(LAUNCH_R / STEP)
        lands = []
        for a in range(bi - r, bi + r + 1):
            for b in range(bj - r, bj + r + 1):
                if 0 <= a < NY and 0 <= b < NX and not WET[a, b] and not WALL[a, b]:
                    d = math.hypot(a - bi, b - bj)
                    if d <= r and H[a, b] <= bz + LAUNCH_UP and H[a, b] > H[bi, bj] + 1500:
                        lands.append(((a, b), 40.0 + d))
        if lands: LAUNCH[src] = lands
def astar(src, dst):
    gi, gj = dst
    g = {src: 0.0}; prev = {}; pq = [(0.0, src)]
    while pq:
        f, u = heapq.heappop(pq)
        if u == dst: break
        i, j = u; gu = g[u]
        if f - math.hypot(i-gi, j-gj) > gu + 1e-6: continue
        for (a, b), lc in LAUNCH.get(u, ()):
            ng = gu + lc
            if ng < g.get((a, b), 1e18):
                g[(a, b)] = ng; prev[(a, b)] = u; JUMPED[(a, b)] = "launch"
                heapq.heappush(pq, (ng + math.hypot(a-gi, b-gj), (a, b)))
        for di, dj, c in NB:
            a, b = i+di, j+dj
            if not (0 <= a < NY and 0 <= b < NX) or WET[a, b]: continue
            if TIGHT[a, b] and H[a, b] <= H[i, j] + 50 and not TIGHT[i, j]: continue
            dz = H[a, b] - H[i, j]
            jump = dz > UP * c
            if jump and dz > JUMP_UP: continue
            cost = c * (1 + 2.0 * near[a, b]) + max(0, dz) / 100.0 * 0.5 + (2.0 if dz < -400 else 0)
            if jump: cost += JUMP_COST
            ng = gu + cost
            if ng < g.get((a, b), 1e18):
                g[(a, b)] = ng; prev[(a, b)] = u; JUMPED[(a, b)] = jump
                heapq.heappush(pq, (ng + math.hypot(a-gi, b-gj), (a, b)))
    if dst not in prev and dst != src: return None
    path = [dst]
    while path[-1] != src: path.append(prev[path[-1]])
    return path[::-1]
def rdp(pts, eps):
    if len(pts) < 3: return pts
    a, b = pts[0], pts[-1]; dx, dy = b[0]-a[0], b[1]-a[1]; L = math.hypot(dx, dy) or 1
    dmax, k = 0, 0
    for idx in range(1, len(pts)-1):
        p = pts[idx]; d = abs(dy*(p[0]-a[0]) - dx*(p[1]-a[1])) / L
        if d > dmax: dmax, k = d, idx
    if dmax > eps: return rdp(pts[:k+1], eps)[:-1] + rdp(pts[k:], eps)
    return [a, b]
def nearest_free(x, y):
    i, j = ij(x, y)
    for r in range(0, 20):
        for a in range(i-r, i+r+1):
            for b in range(j-r, j+r+1):
                if 0 <= a < NY and 0 <= b < NX and not WET[a, b] and not WALL[a, b] and not TIGHT[a, b]: return (a, b)
def cellxyz(c): return (X0 + c[1]*STEP, Y0 + c[0]*STEP, float(H[c]))
