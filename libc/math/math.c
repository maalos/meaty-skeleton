#define NAN 0.0/0.0

/* Helper function: factorial */

int factorial(int n) {
    if (n < 0) {
        return -1;  // Factorial is not defined for negative numbers
    }

    int result = 1;
    while (n > 1) {
        result *= n;
        n--;
    }

    return result;
}

/* Power function */

double pow(double base, int exponent) {
    double result = 1;

    /* Multiply the base by itself exponent times */
    while (exponent > 0) {
        result *= base;
        exponent--;
    }

    return result;
}

/* Trigonometric functions */

double sin(double x) {
    double result = 0;
    int i, sign;

    /* Compute sin(x) using Taylor series expansion */
    for (i = 0, sign = 1; i < 10; i++, sign *= -1) {
        result += sign * pow(x, 2 * i + 1) / factorial(2 * i + 1);
    }

    return result;
}

double cos(double x) {
    double result = 0;
    int i, sign;

    /* Compute cos(x) using Taylor series expansion */
    for (i = 0, sign = 1; i < 10; i++, sign *= -1) {
        result += sign * pow(x, 2 * i) / factorial(2 * i);
    }

    return result;
}

double tan(double x) {
    return sin(x) / cos(x);
}

/* Square root function */

double sqrt(double x) {
    double lower = 0, upper = x, mid;

    if (x < 0) {
        return NAN;
    }

    /* Use binary search to approximate the square root */
    while (upper - lower > 1e-10) {
        mid = (lower + upper) / 2;
        if (mid * mid > x) {
            upper = mid;
        } else {
            lower = mid;
        }
    }

    return lower;
}

/* Absolute value function */

double abs(double x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

/* Minimum and maximum functions */

double min(double x, double y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

double max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

