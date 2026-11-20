from load import *
from PIL import Image, ImageDraw
import sys
H, N = grid()
x0, y0, x1, y1 = [float(v) * 100 for v in sys.argv[1:5]]
i0, j0 = ij(x0, y0); i1, j1 = ij(x1, y1)
sub = H[i0:i1, j0:j1]
lo, hi = np.nanmin(sub), np.nanmax(sub)
t = (sub - lo) / (hi - lo)
img = np.zeros(sub.shape + (3,), np.uint8)
# banded colours every 5 m so steps show
band = ((sub // 500) % 2).astype(bool)
img[..., 0] = (t * 220 + 30).astype(np.uint8); img[..., 1] = (t * 220 + 30).astype(np.uint8); img[..., 2] = np.where(band, 200, 60)
# steep up-steps > 0.45 m between neighbours in red
st = np.zeros(sub.shape, bool)
for di, dj in ((1,0),(0,1),(-1,0),(0,-1)):
    st |= np.abs(np.roll(np.roll(sub, di, 0), dj, 1) - sub) > 190
img[st] = [220, 40, 40]
S = 4
im = Image.fromarray(img[::-1]).resize((sub.shape[1]*S, sub.shape[0]*S), Image.NEAREST)
d = ImageDraw.Draw(im)
cps, boosts, arrows = course()
def P(x, y): i, j = ij(x, y); return (j - j0) * S, (sub.shape[0] - 1 - (i - i0)) * S
for a in arrows:
    x, y = P(a[1], a[2]); d.ellipse([x-3,y-3,x+3,y+3], fill=(0,255,255)); d.text((x+4,y), a[0].split('_')[-1] + " z%d" % (a[3]/100), fill=(0,255,255))
for k, c in enumerate(cps):
    x, y = P(c[1], c[2]); d.ellipse([x-8,y-8,x+8,y+8], outline=(255,0,0), width=3); d.text((x+9,y-6), "CP%d z%d" % (k, c[3]/100), fill=(255,0,0))
for b in boosts:
    x, y = P(b[1], b[2]); d.rectangle([x-5,y-5,x+5,y+5], outline=(255,255,0), width=2); d.text((x+6,y-12), b[0].split('_')[-1] + " z%d" % (b[3]/100), fill=(255,255,0))
im.save(sys.argv[5])
print("height range", lo/100, hi/100)
