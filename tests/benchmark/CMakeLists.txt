#######################################################
### Google benchmark                                ###
#######################################################
# Force some options for the google benchmark library
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
    set(LLVMAR_EXECUTABLE ON CACHE BOOL "Release mode" FORCE)
    set(LLVMNM_EXECUTABLE ON CACHE BOOL "Release mode" FORCE)
    set(LLVMRANLIB_EXECUTABLE ON CACHE BOOL "Release mode" FORCE)
    set(GCC_AR ON CACHE BOOL "Release mode" FORCE)
    set(GCC_RANLIB ON CACHE BOOL "Release mode" FORCE)
endif ()
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Enable installation of benchmark" FORCE)

# Find or get the google benchmark library
# Note that CPM will try to find(benchmark) before downloading it
CPMAddPackage(
        NAME benchmark
        GITHUB_REPOSITORY google/benchmark
        VERSION 1.5.1
        OPTIONS
        "BENCHMARK_ENABLE_TESTING Off"
)

if (benchmark_ADDED)
    # patch google benchmark target
    set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
    target_compile_definitions(benchmark PUBLIC -DNDEBUG)
endif ()

#######################################################
### Evaluation benchmark                            ###
#######################################################
# add_executable(numeric_evaluation numeric_evaluation.cpp)
# target_link_libraries(numeric_evaluation PUBLIC sympp benchmark)