# 指定cmake最小的版本
cmake_minimum_required(VERSION 3.4.1)

#add_library(native-lib SHARED src/main/cmake/native-lib.c)
add_library(testffmpeg SHARED src/main/cmake/testffmpeg.cpp)

# 怎么导入
#message("android_abi11111111111111111111111111111111111111111111  ${ANDROID_ABI}")
#add_library(Test SHARED IMPORTED)
#set_target_properties(Test PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libTest.so)
include_directories(src/main/cmake/include)

# 使用别的地方的so文件 就这么定义 如果是C++ 则使用CMAKE_CXX_FLAGS
# 这里不能使用 CMAKE_CXX_FLAGS 因为这里是c文件 （原因未知）
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}")
#set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}")


# 查找log 将路径复制给mylog  然后在target_link_libraries这里要这么写${mylog}, 不能直接写成mylog，否则报错
#find_library(mylog log)

target_link_libraries(
        testffmpeg
#        native-lib
        avcodec avfilter avformat avutil swresample swscale
        log)
