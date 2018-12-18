#pragma once

#include <cuda.h>
#include <builtin_types.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cstring>
#include <ctime>

#include <math.h>

#define WARP_SIZE 32
extern "C"
__device__ int _pow_(int a, int b);
extern "C"
__global__ void _cuda_parallel_sum(unsigned char *in, int num_elements, int *sum, const float *information);
extern "C"
__global__ void _touch_test(const unsigned char * subregion, unsigned int *cell, int *table, const float *information);
extern "C"
__global__ void _vector_add(int *table, int *itable);
extern "C"
__global__ void _intersect_test(unsigned int *cell, int *table, int *itable, const float *information);



int cuda_parallel_sum(const unsigned char * a, const float *information);
int* cuda_parallel_inclusion(const unsigned char * subregion, unsigned int *cell, const float *information);