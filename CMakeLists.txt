#######################################################
### SymPP                                           ###
#######################################################
# Project information
cmake_minimum_required(VERSION 3.14)
project(sympp VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

# CMake dependencies for installer
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Include CPM to find or download packages
# Note that CPM will try to find the packages locally before downloading them
# This avoids ODR problems
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)
include(cmake/CPM.cmake)

# Check if this a master project or a subdirectory of another project
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(MASTER_PROJECT ON)
else ()
    set(MASTER_PROJECT OFF)
endif ()
message("MASTER_PROJECT=${MASTER_PROJECT}")
set(SYMPP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
### Options                                         ###
#######################################################
# What to build
option(BUILD_PYTHON_BINDING "Compile the Python binding" ${MASTER_PROJECT})
option(BUILD_EXAMPLES "Compile the examples" ${MASTER_PROJECT})
option(BUILD_TESTS "Compile the tests" ${MASTER_PROJECT})
option(BUILD_INSTALLER "Build an installation package" ${MASTER_PROJECT})
option(BUILD_PACKAGE "Build an installation package" ${MASTER_PROJECT})

# How to build
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(DEBUG_MODE ON)
endif ()
option(BUILD_WITH_PEDANTIC_WARNINGS "Use pedantic warnings. This is useful for developers because many of these warnings will be in continuous integration anyway." ${DEBUG_MODE})

#######################################################
### Libraries                                       ###
#######################################################
# Main library
add_subdirectory(source)

#######################################################
### Examples and tests                              ###
#######################################################
# Examples
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

# Tests: unit tests and benchmarks
if (BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif ()

#######################################################
### Installer                                       ###
#######################################################
# Create and install SymPPConfig.cmake
if (BUILD_INSTALLER)
    # https://cliutils.gitlab.io/modern-cmake/chapters/install/installing.html
    # Set variable where the cmake config is
    set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/SymPP)

    # Create SymPPConfigVersion.cmake and install it
    write_basic_package_version_file(
            SymPPConfigVersion.cmake
            VERSION ${PACKAGE_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Create SymPPConfig.cmake from SymPPConfig.cmake.in
    # SymPPConfig.cmake will include the SymPPTargets.cmake file
    # We could have just renamed SymPPTargets.cmake to SymPPConfig.cmake
    # But SymPPConfig.cmake allows us to include extra dependencies
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/SymPPConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/SymPPConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SymPP)

    # Install the file SymPPConfig.cmake
    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/SymPPConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/SymPPConfigVersion.cmake
            COMPONENT "CPP_Library"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SymPP
            )
endif ()

#######################################################
### Packages                                        ###
#######################################################
if (BUILD_INSTALLER AND BUILD_PACKAGE)
    # Set the cpack variables
    # https://cliutils.gitlab.io/modern-cmake/chapters/install/packaging.html

    # The most common cpack variables
    set(CPACK_PACKAGE_NAME "SymPP")
    set(CPACK_PACKAGE_VENDOR "https://github.com/alandefreitas")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ Symbolic Library")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    # Set CPACK_SOURCE_IGNORE_FILES with files source packages shouldn't install
    # We get these from .gitignore to avoid redundancy
    FILE(READ .gitignore GITIGNORE_CONTENTS)
    STRING(REGEX REPLACE ";" "\\\\;" GITIGNORE_CONTENTS "${GITIGNORE_CONTENTS}")
    STRING(REGEX REPLACE "\n" ";" GITIGNORE_CONTENTS "${GITIGNORE_CONTENTS}")
    set(CPACK_SOURCE_IGNORE_FILES ${GITIGNORE_CONTENTS})

    # Always include CPack at last
    include(CPack)
endif ()