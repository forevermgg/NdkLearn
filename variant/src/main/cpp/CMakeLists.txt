# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("ndk")
include_directories(variant)
include_directories(include) # spdlog路径
# find_package(mpark_variant 1.3.0 REQUIRED)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
add_library(variant
        SHARED
        variant/android_util.h
        variant/android_util.cc
        variant/variant.h
        variant/variant.cc
        )

add_library( # Sets the name of the library.
        ndk

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        json.hpp
        log_utils.h
        io_realm_internal_NativeObjectReference.cpp
        io_realm_internal_core_NativeRealmAny.cpp
        native-lib.cpp)

install(FILES json.hpp
        DESTINATION json
        COMPONENT devel)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ndk
        variant
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})