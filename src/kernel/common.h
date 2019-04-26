#ifndef DGL_KERNEL_COMMON_H_
#define DGL_KERNEL_COMMON_H_

namespace dgl {
namespace kernel {

#ifdef __CUDACC__
#define DGLDEVICE __device__
#define DGLINLINE __forceinline__
#else
#define DGLDEVICE
#define DGLINLINE __inline__
#endif  // __CUDACC__

#ifdef DGL_USE_CUDA
#define DGL_XPU_SWITCH(val, Method, ...)  \
  if (val == kDLCPU) {                    \
    cpu::Method(__VA_ARGS__);             \
  } else if (val == kDLGPU) {             \
    cuda::Method(__VA_ARGS__);            \
  } else {                                \
    LOG(FATAL) << "Unsupported device type: " << val;  \
  }
#else  // DGL_USE_CUDA
#define DGL_XPU_SWITCH(val, Method, ...)  \
  if (val == kDLCPU) {                    \
    cpu::Method(__VA_ARGS__);             \
  } else {                                \
    LOG(FATAL) << "Unsupported device type: " << val;  \
  }
#endif  // DGL_USE_CUDA

#define GEN_DTYPE(GEN, ...)  \
  GEN(__VA_ARGS__, float)    \
  GEN(__VA_ARGS__, double)   \
  GEN(__VA_ARGS__, int32_t)  \
  GEN(__VA_ARGS__, int64_t)  \

#define DGL_DTYPE_SWITCH(val, DType, ...)                   \
  if (val.code == kDLInt && val.bits == 32) {               \
    typedef int32_t DType;                                  \
    {__VA_ARGS__}                                           \
  } else if (val.code == kDLInt && val.bits == 64) {        \
    typedef int64_t DType;                                  \
    {__VA_ARGS__}                                           \
  } else if (val.code == kDLFloat && val.bits == 32) {      \
    typedef float DType;                                    \
    {__VA_ARGS__}                                           \
  } else if (val.code == kDLFloat && val.bits == 64) {      \
    typedef double DType;                                   \
    {__VA_ARGS__}                                           \
  } else {                                                  \
    LOG(FATAL) << "Unsupported dtype: " << val.code << "_"  \
               << val.bits;                                 \
  }


}  // namespace kernel
}  // namespace dgl

#endif  // DGL_KERNEL_COMMON_H_
