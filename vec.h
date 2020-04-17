#ifndef VEC_H
#define VEC_H

struct Veci {
	int x, y;
};

Veci operator+(Veci a, Veci b);
bool operator==(Veci a, Veci b);
bool operator!=(Veci a, Veci b);

#endif