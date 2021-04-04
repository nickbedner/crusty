cmake_minimum_required(VERSION 3.12)
project(Crusty LANGUAGES C VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

set(ignoreMe "${CMAKE_CXX_COMPILER}")

file(GLOB_RECURSE crusty_SRC
        include/*.h
        src/*.c)

add_executable(Crusty ${crusty_SRC})

if (WIN32)
        #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xclang -fopenmp -qopenmp-link=static")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xclang -fopenmp")
endif (WIN32)

if(APPLE)
        add_subdirectory(lib/MoltenVK buildmoltenvk)
        find_package(OpenMP)
        if (OPENMP_FOUND)
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        endif()
endif()

if(UNIX AND NOT APPLE)
        # for Linux, BSD, Solaris, Minix
        find_package(OpenMP)
        if (OPENMP_FOUND)
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-visibility -Wno-pragma-pack -Wno-deprecated-declarations -mavx")

if(CMAKE_BUILD_TYPE MATCHES Debug)
        message("Debug build.")
        set(CRUSTY_BUILD_LIBRARIES
                Crusty
                debug/crusty_core)
else()
endif()

if(WIN32)
        set(CRUSTY_BUILD_LIBRARIES ${CRUSTY_BUILD_LIBRARIES} wsock32 ws2_32 userenv)
endif()

if (WIN32 AND CMAKE_C_COMPILER_ID MATCHES "Clang")
        STRING(REPLACE " " "\ " PATH_NO_SPACE "$ENV{ProgramFiles}")
        set(CRUSTY_BUILD_LIBRARIES ${CRUSTY_BUILD_LIBRARIES} "${PATH_NO_SPACE}/LLVM/lib/libomp.lib" "${PATH_NO_SPACE}/LLVM/lib/libiomp5md.lib")
        add_custom_command (TARGET Crusty POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PATH_NO_SPACE}/LLVM/bin/libomp.dll ${CMAKE_CURRENT_BINARY_DIR}/libomp.dll)
endif ()

target_link_libraries(${CRUSTY_BUILD_LIBRARIES})

set(includeList
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenMP_CXX_LIBRARIES})

target_include_directories(Crusty PUBLIC ${includeList})

add_custom_target(crusty_rust COMMAND cargo build)

add_dependencies(Crusty crusty_rust)

#if(WIN32)
#        target_sources(Crusty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/icon.rc)
#endif()
