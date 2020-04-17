#include "vec.h"

Veci operator+(Veci a, Veci b) {
	return Veci{a.x + b.x, a.y + b.y};
}

bool operator==(Veci a, Veci b) {
	if(a.x == b.x && a.y == b.y) return(true);
	else return(false);
}

bool operator!=(Veci a, Veci b) {
	if(a.x != b.x || a.y != b.y) return(true);
	else return(false);
}