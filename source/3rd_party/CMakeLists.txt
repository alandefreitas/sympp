#######################################################
### Dependencies                                    ###
#######################################################
# TinyCC: Adapted from https://github.com/Alex2804/libtcc-cmake
SET(LIBTCC_TEST OFF CACHE BOOL "")
SET(LIBTCC_ENABLE_EXTENSION ON CACHE BOOL "")
add_subdirectory(libtcc EXCLUDE_FROM_ALL)

if (APPLE)
    # sem_init() is deprecated on Mac OS
    target_compile_options(libtcc PRIVATE -Wno-deprecated-declarations)
    # tccpp.c:1424:33: warning: adding 'int' to a string does not append to the string
    target_compile_options(libtcc PRIVATE -Wno-string-plus-int)
endif ()


if (MASTER_PROJECT AND NOT BUILD_SHARED_LIBS)
    get_target_property(RESULT libtcc INTERFACE_INCLUDE_DIRECTORIES)
    message("get_target_property(RESULT libtcc INTERFACE_INCLUDE_DIRECTORIES):")
    foreach (DIR ${RESULT})
        message("    - ${DIR}")
    endforeach ()
    install(TARGETS libtcc
            EXPORT SymPPTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/SymPP)
endif ()

# Threads
if (NOT WIN32)
    if (NOT APPLE)
        find_package(Threads REQUIRED)
    endif ()
endif ()
