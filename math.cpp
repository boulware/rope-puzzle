#include "math.h"

int min(int a, int b) {
	if(a <= b) return(a);
	else return(b);
}

int max(int a, int b) {
	if(a >= b) return(a);
	else return(b);
}

int clamp(int value, int min_value, int max_value) {
	return min(max(min_value, value), max_value);
}

int sign(int a) {
	if(a > 0) return 1;
	if(a == 0) return 0;
	if(a < 0) return -1;

	return 0;
}

bool in_range(int value, int bound_a, int bound_b) {
	int low_bound = min(bound_b, bound_b);
	int high_bound = max(bound_a, bound_b);
	if(value >= low_bound and value <= high_bound) return true;
	else return false;
}