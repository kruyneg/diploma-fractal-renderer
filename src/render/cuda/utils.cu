#include "render/cuda/utils.h"

bool IsCUDASupported() {
  int deviceCount = 0;
  if (cudaGetDeviceCount(&deviceCount) != cudaSuccess) {
    return false;
  }
  return deviceCount > 0;
}