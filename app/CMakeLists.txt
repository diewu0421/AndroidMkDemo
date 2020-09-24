# 指定cmake最小的版本
cmake_minimum_required(VERSION 3.8)

add_library(native-lib SHARED src/main/cmake/native-lib.c)

# 怎么导入
#message("android_abi11111111111111111111111111111111111111111111  ${CMAKE_ANDROID_ARCH_ABI}")
#add_library(Test SHARED IMPORTED)
#set_target_properties(Test PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libTest.so)

# 这里不能使用 CMAKE_CXX_FLAGS 因为这里是c文件 （原因未知）
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}")

target_link_libraries(native-lib Test)
