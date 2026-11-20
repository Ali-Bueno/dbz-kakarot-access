from plan import *
from PIL import Image, ImageDraw
cps, boosts, arrows = course()
add_launches([(b[0], b[1], b[2], b[3]) for b in boosts])
print("launch pads:", len(LAUNCH))
B = {b[0].split('_')[-1]: b for b in boosts}
legs = []
def leg(name, pts_xy, jump_at=None):
    """pts_xy: (x, y) stops planned between with A*, or FIXED points (x, y, z[, flag]) placed by
    hand from the height tables, for stretches the 2 m grid cannot model (a crack to jump)."""
    out = []
    for k in range(len(pts_xy) - 1):
        pa, pb = pts_xy[k], pts_xy[k + 1]
        if len(pa) > 2 and len(pb) > 2:        # fixed -> fixed: no planning
            if not out: out.append(list(pa))
            out.append(list(pb)); continue
        s = nearest_free(*pa[:2]); d = nearest_free(*pb[:2])
        p = astar(s, d)
        if p is None: print(name, "NO PATH", k); return None
        # Split at the jumps so simplification keeps each take-off point, flagged.
        xy = [list(cellxyz(c)) for c in p]
        cuts = [0]
        for q in range(1, len(p)):
            if JUMPED.get(p[q]) and q - 1 - cuts[-1] >= 0:
                if q - 1 > cuts[-1]: cuts.append(q - 1)
                xy[q - 1].append("launch" if JUMPED[p[q]] == "launch" else "j")
                if JUMPED[p[q]] == "launch": cuts.append(q)
        cuts.append(len(p) - 1)
        simp = []
        for a, b in zip(cuts, cuts[1:]):
            part = rdp(xy[a:b + 1], 300)
            simp += part if not simp else part[1:]
        # merge jump points within 10 m of the previous one
        lastj = None
        for w in simp:
            if len(w) > 3 and w[3] == "j":
                if lastj and math.hypot(w[0]-lastj[0], w[1]-lastj[1]) < 1000 and w[3] == "j": w.pop()
                else: lastj = w
        if out:
            simp = simp[1:]
            if len(pa) > 2: simp = [q for q in simp if math.hypot(q[0]-pa[0], q[1]-pa[1]) > 300 or len(q) > 3]
        out += [list(q) for q in simp]
        if jump_at is not None and k == jump_at: out[-1].append("jump")
    L = sum(math.hypot(out[i+1][0]-out[i][0], out[i+1][1]-out[i][1]) for i in range(len(out)-1))
    print(name, len(out), "waypoints", round(L/100), "m", sum(1 for w in out if "j" in w[3:]), "ledge jumps", sum(1 for w in out if "launch" in w[3:]), "launches")
    legs.append((name, out)); return out
c = [(p[1], p[2]) for p in cps]
b17 = B["17"]
leg("L1", [c[0], c[1]])
leg("L2", [c[1], c[2]])
# L3 entirely by hand, along the centreline of the high ground (height tables, verified by the
# autopilot 2026-11-20). Off the CP2 rock north, jump the 2-4 m crack at y 1612 onto the ridge
# (87 -> 90 m), climb it, jump the 2 m crack at y 1686 (104 -> 107 m), cross the 120 m plateau,
# then down the 14-22 m wide tongue to the CP3 plateau. A* hugged the edges and the robot fell.
# The race places dinosaur bones on this leg; a survey started just above the robot (survey_low*)
# shows where they block: west of the ridge bone, east of the plateau bone.
L3 = [(-1871, 1572, 87), (-1868, 1590, 82), (-1866, 1600, 84), (-1864, 1608, 87, "j"),
      (-1858, 1618, 90), (-1856, 1630, 92), (-1855, 1644, 94), (-1855, 1656, 100),
      (-1852, 1666, 105), (-1848, 1676, 106), (-1846, 1681, 104, "j"), (-1846, 1694, 112),
      (-1846, 1710, 123), (-1842, 1730, 127), (-1838, 1750, 125), (-1837, 1770, 124),
      (-1837, 1790, 121), (-1838, 1810, 120), (-1840, 1830, 121), (-1846, 1848, 115),
      (-1851, 1858, 105), (-1856, 1870, 105), (-1856, 1884, 104), (-1853, 1892, 107),
      (-1848, 1898, 108), (-1843, 1908, 106), (-1836, 1920, 108), (-1826, 1940, 108)]
leg("L3", [tuple(v * 100 if isinstance(v, int) else v for v in p) for p in L3])
leg("L4", [c[3], c[4]])
leg("L5", [c[4], c[5]])
json.dump(legs, open("legs.json", "w"))
# render
img = np.zeros((NY, NX, 3), np.uint8)
t = np.clip((H - 1000) / (13000 - 1000), 0, 1)
for k, f in enumerate((200, 200, 150)): img[..., k] = (t * f + 40).astype(np.uint8)
img[WALL] = [150, 60, 60]; img[WET] = [20, 60, 200]
S = 2
im = Image.fromarray(img[::-1]).resize((NX*S, NY*S), Image.NEAREST); d = ImageDraw.Draw(im)
def P(x, y): i, j = ij(x, y); return j*S, (NY-1-i)*S
cols = [(255,0,255),(0,255,0),(255,128,0),(0,200,255),(255,255,255)]
for (name, pts), col in zip(legs, cols):
    d.line([P(p[0], p[1]) for p in pts], fill=col, width=3)
    for p in pts:
        if len(p) > 3: x, y = P(p[0], p[1]); d.ellipse([x-6,y-6,x+6,y+6], outline=(255,0,0), width=3)
for k, cp in enumerate(cps):
    x, y = P(cp[1], cp[2]); d.ellipse([x-6,y-6,x+6,y+6], outline=(255,0,0), width=2); d.text((x+8,y-6), "CP%d"%k, fill=(255,0,0))
for b in boosts:
    x, y = P(b[1], b[2]); d.rectangle([x-4,y-4,x+4,y+4], outline=(255,255,0), width=2)
im.save("legs.png")
