find_package(OpenCV REQUIRED)

file(GLOB SRCS Kernels/*.cpp)
add_executable(demo demo.cpp ${SRCS} "Kernels/demo_FindContours.cpp")
target_link_libraries(demo openvx_kernels ${OpenCV_LIBS})
