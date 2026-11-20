import numpy as np, sys, os
# Survey and course dumps written by the dev channel (`survey`, `course`, `volgeom`), gitignored.
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "mod", "KakarotAccess", "Scripts", "dumps") + os.sep
X0, Y0, STEP = -250000, 130000, 200
NX, NY = 500, 600
def grid():
    H = np.full((NY, NX), np.nan); N = np.full((NY, NX), np.nan)
    for line in open(D + "survey_race03.txt"):
        x, y, z, nz, _ = line.split()
        i = (int(float(y)) - Y0) // STEP; j = (int(float(x)) - X0) // STEP
        if 0 <= i < NY and 0 <= j < NX: H[i, j] = float(z); N[i, j] = float(nz)
    return H, N
def course():
    cps, boosts, arrows = [], [], []
    for line in open(D + "dump_race_course.txt"):
        p = line.split()
        x, y, z = float(p[2]), float(p[3]), float(p[4])
        if p[0] == "BP_CheckPoint_C": cps.append((p[1], x, y, z))
        elif p[0] == "BP_BoostPoint_C": boosts.append((p[1], x, y, z))
        elif p[0] == "arrow": arrows.append((p[1], x, y, z, float(p[5])))
    cps.sort(key=lambda c: int(''.join(ch for ch in c[0].split('_')[1][10:] if ch.isdigit()) or 0))
    return cps, boosts, arrows
def ij(x, y): return int((y - Y0) / STEP), int((x - X0) / STEP)
import math
VC = (-181298.0, 86889.0); VYAW = math.radians(-50.6); VA, VB = 19846.0, 80113.0; VTOP = 4800.0
def in_volume(x, y):
    dx, dy = x - VC[0], y - VC[1]
    c, s = math.cos(-VYAW), math.sin(-VYAW)
    lx, ly = dx * c - dy * s, dx * s + dy * c
    return abs(lx) <= VA and abs(ly) <= VB
