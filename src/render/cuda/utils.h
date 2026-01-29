#pragma once

#include <iostream>

#define CUDA_CHECK(call)                                                 \
  do {                                                                   \
    cudaError_t err = call;                                              \
    if (err != cudaSuccess) {                                            \
      std::cerr << "CUDA error " << __FILE__ << ":" << __LINE__ << " : " \
                << cudaGetErrorString(err) << std::endl;                 \
      std::exit(EXIT_FAILURE);                                           \
    }                                                                    \
  } while (0)

bool IsCUDASupported();
