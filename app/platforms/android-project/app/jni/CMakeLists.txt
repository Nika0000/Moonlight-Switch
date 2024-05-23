cmake_minimum_required(VERSION 3.6)

set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../..)
set(CMAKE_TOOLCHAIN_FILE ${PROJECT_PATH}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE PATH "vcpkg toolchain file" FORCE)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE $ENV{ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake CACHE PATH "android toolchain file")

# Find vcpkg
find_program(VCPKG
        NAMES vcpkg
        PATHS ${PROJECT_PATH}/extern/vcpkg/
)

if (NOT VCPKG)
    message(FATAL_ERROR "vcpkg not found")
endif ()

if(DEFINED ANDROID_ABI)
    message("Building for ABI: ${ANDROID_ABI}")
else()
    message(FATAL_ERROR "ANDROID_ABI is not defined.")
endif()

if(ANDROID_ABI STREQUAL "armeabi-v7a")
    set(VCPKG_TARGET_TRIPLET arm-android)
elseif (ANDROID_ABI STREQUAL "arm64-v8a")
    set(VCPKG_TARGET_TRIPLET arm64-android)
else ()
    message(FATAL_ERROR "Unsupported Android ABI: ${ANDROID_ABI}")
endif ()


get_filename_component(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build/libs/" ABSOLUTE)

execute_process(
        COMMAND ${VCPKG} install --triplet ${VCPKG_TARGET_TRIPLET} --x-install-root ${VCPKG_INSTALLED_DIR}
        WORKING_DIRECTORY ${PROJECT_PATH}
        RESULT_VARIABLE VCPKG_RESULT
)


if(VCPKG_RESULT)
    message(FATAL_ERROR "vcpkg install failed with error code ${VCPKG_RESULT}")
endif()

project(Moonlight)

if(ANDROID_ABI STREQUAL "armeabi-v7a")
    # armeabi-v7a requires cpufeatures library
    include(AndroidNdkModules)
    android_ndk_import_module_cpufeatures()
endif ()

# SDL sources are in a subfolder named "SDL"
add_subdirectory(SDL)

# Compilation of companion libraries
#add_subdirectory(SDL_image)
#add_subdirectory(SDL_mixer)
#add_subdirectory(SDL_ttf)

# Your game and its CMakeLists.txt are in a subfolder named "src"
#add_subdirectory(src)


SET(PLATFORM_ANDROID ON CACHE BOOL "")

find_program(LIBROMFS_PREBUILT_GENERATOR
        NAMES libromfs-generator
        PATHS ${PROJECT_PATH}
)

if (NOT EXISTS "${LIBROMFS_PREBUILT_GENERATOR}")
    message(FATAL_ERROR "libromfs-generator has not been built, please refer to borealis/build_libromfs_generator.sh for more information")
endif()

add_subdirectory(moonlight)
