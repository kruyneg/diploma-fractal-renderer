#include "render/cuda/cuda_renderer.h"

#include <stdexcept>

#include "render/cuda/utils.h"
#include "render/fractals.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <cuda_gl_interop.h>

namespace {

__global__ void MandelbrotKernel(cudaSurfaceObject_t surf, int w, int h) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x >= w || y >= h) return;

  float fx = static_cast<float>(x) / static_cast<float>(w) * 3.5f - 2.5f;
  float fy = static_cast<float>(y) / static_cast<float>(h) * 2.0f - 1.0f;

  const int kMaxIter = 128;
  const int iteration = MandelbrotIterations({fx, fy}, kMaxIter);
  Color color = ColorFromIter(iteration, kMaxIter);

  uchar4 c = {color.r, color.g, color.b, color.a};
  surf2Dwrite(c, surf, x * sizeof(Color), y, cudaBoundaryModeTrap);
}

}  // namespace

CUDARenderer::CUDARenderer() = default;

CUDARenderer::~CUDARenderer() = default;

void CUDARenderer::Init(uint32_t target_tex_id) {
  if (initialized_) {
    throw std::runtime_error("CUDARenderer: double init");
  }

#ifdef _WIN32
  if (!wglGetCurrentContext()) {
    throw std::runtime_error("CUDARenderer: no current OpenGL context");
  }
#endif

  gl_tex_id_ = target_tex_id;
  initialized_ = true;
}

void CUDARenderer::Resize(uint32_t width, uint32_t height) {
  width_ = width;
  height_ = height;
}

void CUDARenderer::Render() {
  if (!initialized_ || width_ == 0 || height_ == 0) return;

  cudaGraphicsResource_t temp_resource = nullptr;
  CUDA_CHECK(
      cudaGraphicsGLRegisterImage(&temp_resource, gl_tex_id_, GL_TEXTURE_2D,
                                  cudaGraphicsRegisterFlagsWriteDiscard));

  CUDA_CHECK(cudaGraphicsMapResources(1, &temp_resource, 0));

  cudaArray_t array = nullptr;
  CUDA_CHECK(
      cudaGraphicsSubResourceGetMappedArray(&array, temp_resource, 0, 0));

  cudaResourceDesc res_desc = {};
  res_desc.resType = cudaResourceTypeArray;
  res_desc.res.array.array = array;

  cudaSurfaceObject_t surf = 0;
  CUDA_CHECK(cudaCreateSurfaceObject(&surf, &res_desc));

  dim3 block(16, 16);
  dim3 grid((width_ + block.x - 1) / block.x,
            (height_ + block.y - 1) / block.y);

  MandelbrotKernel<<<grid, block>>>(surf, width_, height_);

  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());

  CUDA_CHECK(cudaDestroySurfaceObject(surf));
  CUDA_CHECK(cudaGraphicsUnmapResources(1, &temp_resource, 0));
  CUDA_CHECK(cudaGraphicsUnregisterResource(temp_resource));

  glFlush();
}