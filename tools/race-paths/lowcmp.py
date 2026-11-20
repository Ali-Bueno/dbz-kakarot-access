# Compare a low survey (traces started just above the racer) with the top-down one: prints the
# low heights, '*' where the top survey saw something higher (an arch or bone overhead).
import sys
from load import *
H, N = grid()
name = sys.argv[1]
rows = {}
for line in open(D + "survey_%s.txt" % name):
    x, y, z = [float(v) for v in line.split()[:3]]
    rows.setdefault(int(round(y)), {})[int(round(x))] = z
ys = sorted(rows, reverse=True)
xs = sorted({x for r in rows.values() for x in r})
k = int(sys.argv[2]) if len(sys.argv) > 2 else 2
print("      " + "".join("%5d" % ((x // 100) % 1000) for x in xs[::k]))
for y in ys[::k]:
    out = []
    for x in xs[::k]:
        z = rows[y].get(x)
        top = H[ij(x, y)]
        if z is None: out.append("    ?"); continue
        out.append("%4d%s" % (z / 100, "*" if top - z > 300 else " "))
    print("%5d " % (y / 100) + "".join(out))
