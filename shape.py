#! /usr/bin/python3

from math import sin, cos, pi

for v in [-1,1]:
    print(v)
    for u in range(60):
        print(str((1 + v/2 * cos(u/60 * pi))*cos(u/30 * pi)), end=', ')
        print(str((1 + v/2 * cos(u/60 * pi))*sin(u/30 * pi)), end=', ')
        print(str(v/2 * sin(u/60*pi)) + ", ")
    print("// End of first outer strip")


# Indices
for i in range(60):
    print(i, (60 + i) % 120, (i + 1) % 120)
    print((60 + i) % 120, (i + 1)%120, (60 + i + 1) % 120)
