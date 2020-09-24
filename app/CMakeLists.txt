# 指定cmake最小的版本
cmake_minimum_required(VERSION 3.8)

add_library(native-lib SHARED src/main/cmake/native-lib.c)

add_library(Test STATIC IMPORTED)
# 怎么导入
message("android_abi11111111111111111111111111111111111111111111  ${CMAKE_ANDROID_ARCH_ABI}")
set_target_properties(Test PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cmake/libTest.a)

target_link_libraries(native-lib Test)
