file(GLOB SRCS Kernels/ref/*.c)
add_library(openvx_kernels ${SRCS} "Kernels/ref/ref_FindContours.c")
target_include_directories(openvx_kernels PUBLIC Common .)
