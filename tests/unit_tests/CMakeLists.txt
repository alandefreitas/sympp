#######################################################
### Catch2 - Unit tests                             ###
#######################################################
# Find or get Catch2
# Note that CPM will try to "find_package(Catch2)" before downloading it
# See the option CPM_USE_LOCAL_PACKAGES in ROOT/CMakeLists.txt
# This is important: see One Definition Rule (ODR)
CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 2.12.1
        OPTIONS
        "CATCH_USE_VALGRIND OFF"
        "CATCH_BUILD_TESTING OFF"
        "CATCH_BUILD_EXAMPLES OFF"
        "CATCH_BUILD_EXTRA_TESTS OFF"
        "CATCH_ENABLE_COVERAGE OFF"
        "CATCH_ENABLE_WERROR ON"
        "CATCH_INSTALL_DOCS OFF"
        "CATCH_INSTALL_HELPERS OFF"
)
include(${Catch2_SOURCE_DIR}/contrib/Catch.cmake)

#######################################################
### Test all node types                             ###
#######################################################
add_executable(test_node_types node_types.cpp)
target_link_libraries(test_node_types PRIVATE sympp Catch2)
catch_discover_tests(test_node_types)

#######################################################
### Test sym object                                 ###
#######################################################
# add_executable(test_sym sym.cpp)
# target_link_libraries(test_sym PRIVATE sympp Catch2)
# catch_discover_tests(test_sym)

#######################################################
### Test symbolic functions                         ###
#######################################################
# add_executable(test_sym_functions sym_functions.cpp)
# target_link_libraries(test_sym_functions PRIVATE sympp Catch2)
# catch_discover_tests(test_sym_functions)

#######################################################
### Test compiler                                   ###
#######################################################
# add_executable(test_sym_functions sym_functions.cpp)
# target_link_libraries(test_sym_functions PRIVATE sympp Catch2)
# catch_discover_tests(test_sym_functions)
