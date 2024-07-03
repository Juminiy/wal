#include "../include/math.h"

double f64_arr_avg(const double* arr, size_t sz)
{
    double ftot = 0.0;
    for(int i = 0; i < sz; i++)
        ftot += arr[i];
    return ftot / sz;
}
