#ifndef MATH_H
#define MATH_H

int min(int a, int b);
int max(int a, int b);
int clamp(int value, int min_value, int max_value);
int sign(int a);
bool in_range(int value, int bound_a, int bound_b);

#endif