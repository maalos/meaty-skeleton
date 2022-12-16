#ifndef _MATH_H
#define _MATH_H 1

#ifdef __cplusplus
extern "C" {
#endif

#define NAN 0.0/0.0

/* Helper function: factorial */
int factorial(int n);

/* Power function */
double pow(double base, int exponent);

/* Trigonometric functions */
double sin(double x);
double cos(double x);
double tan(double x);

/* Square root function */
double sqrt(double x);

/* Absolute value function */
double abs(double x);

/* Minimum and maximum functions */
double min(double x, double y);
double max(double x, double y);

#ifdef __cplusplus
}
#endif

#endif
