#include "render/cuda/cuda_renderer.h"

#include <stdexcept>

#include "render/common/fractals.h"
#include "render/common/utils.h"
#include "render/cuda/utils.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <cuda_gl_interop.h>

namespace {

__global__ void Render2DKernel(cudaSurfaceObject_t surf, int w, int h,
                               render::RenderSettings settings) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x >= w || y >= h) return;

  const auto pos = render::PixelToPosition(x, y, w, h, settings.camera);

  int iteration;
  if (settings.fractal.type == render::FractalType::kMandelbrot) {
    iteration = render::MandelbrotIterations(pos.x, pos.y,
                                             settings.fractal.max_iterations);
  } else if (settings.fractal.type == render::FractalType::kJulia) {
    iteration = render::JuliaIterations(
        pos.x, pos.y, settings.fractal.max_iterations,
        settings.fractal.julia.c_re, settings.fractal.julia.c_im);
  }
  Color color =
      render::ColorFromIter(iteration, settings.fractal.max_iterations,
                            Color{255, 0, 255, 255}, Color{100, 255, 100, 255});

  uchar4 c = {color.r, color.g, color.b, color.a};
  surf2Dwrite(c, surf, x * sizeof(Color), y, cudaBoundaryModeTrap);
}

__global__ void RayMarchingKernel(cudaSurfaceObject_t surf, int w, int h,
                                  render::RenderSettings settings) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x >= w || y >= h) {
    return;
  }

  Ray ray = MakeRay(x, y, w, h, settings.camera);

  Color color = {0, 0, 0, 255};

  double t = 0.0;
  const int max_steps = 100;
  const double min_dist = 0.001;
  const float max_dist = 13.0;
  for (int i = 0; i < max_steps; ++i) {
    const auto pos = ray.position + ray.direction * t;
    const auto distance = render::CalculateSignedDistance(pos, settings);
    if (distance < min_dist) {
      auto norm = render::GetNormal(pos, settings);
      norm = norm * 0.5 + Vector3d{0.5, 0.5, 0.5};
      norm = norm * 255;
      color.r = norm.x;
      color.g = norm.y;
      color.b = norm.z;

      break;
    }
    if (distance > max_dist) {
      break;
    }
    t += distance;
  }

  uchar4 c = {color.r, color.g, color.b, color.a};
  surf2Dwrite(c, surf, x * sizeof(Color), y, cudaBoundaryModeTrap);
}

}  // namespace

namespace render {

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

  if (Is2DFractal(settings_provider_->GetSettings().fractal.type)) {
    Render2DKernel<<<grid, block>>>(surf, width_, height_,
                                    settings_provider_->GetSettings());
  } else {
    RayMarchingKernel<<<grid, block>>>(surf, width_, height_,
                                       settings_provider_->GetSettings());
  }
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());

  CUDA_CHECK(cudaDestroySurfaceObject(surf));
  CUDA_CHECK(cudaGraphicsUnmapResources(1, &temp_resource, 0));
  CUDA_CHECK(cudaGraphicsUnregisterResource(temp_resource));

  glFlush();
}

void CUDARenderer::SetSettingsProvider(SettingsProvider* settings) {
  settings_provider_ = settings;
}

}  // namespace render
