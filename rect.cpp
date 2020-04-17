#include "rect.h"
#include "vec.h"

bool point_in_rect(Veci point, Rect rect) {
	if(		point.x >= rect.x
		&&	point.x < rect.x + rect.w
		&&	point.y >= rect.y
		&& point.y < rect.y + rect.h) {
		return true;
	}

	return false;
}

Veci center(Rect rect) {
	int x = rect.x + rect.w/2;
	int y = rect.y + rect.h/2;
	return {x,y};
}

Rect center_rect(Rect rect, Veci point) {
	int x = point.x - rect.w/2;
	int y = point.y - rect.h/2;
	return Rect{x, y, rect.w, rect.h};
}