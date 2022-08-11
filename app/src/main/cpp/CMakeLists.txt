# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

# 最低的cmake版本
cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("jnitest")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        jnitest   # 库的名称

        # Sets the library as a shared library.
        SHARED    # SHARED:动态库、STATIC:静态库

        # Provides a relative path to your source file(s).
        native-lib.cpp) # 源文件，可以是多个

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
# 寻找系统中的log库，保存在log-lib变量中
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# jnitest这个库会去依赖log-lib这个库
target_link_libraries( # Specifies the target library.
        jnitest

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})