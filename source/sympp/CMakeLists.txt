#######################################################
### Library                                         ###
#######################################################
add_library(sympp
        # Main library objects
        core/node_interface.h
        core/node_interface.cpp
        core/terminal_node_interface.h
        core/internal_node_interface.h
        core/sym_error.h
        core/sym_error.cpp
        core/sym.h
        core/sym.cpp

        # Nodes that represent a symbol
        node/terminal/boolean.h
        node/terminal/boolean.cpp
        node/terminal/integer.h
        node/terminal/integer.cpp
        node/terminal/number_interface.h
        node/terminal/number_interface.cpp
        node/terminal/rational.h
        node/terminal/rational.cpp
        node/terminal/real.h
        node/terminal/real.cpp
        node/terminal/variable.h
        node/terminal/variable.cpp
        node/terminal/constant.h
        node/terminal/constant.cpp

        node/operation/summation.h
        node/operation/summation.cpp
        node/operation/product.h
        node/operation/product.cpp

        node/function/function_interface.h
        node/function/function_interface.cpp
        node/function/abs.h
        node/function/abs.cpp
        node/function/cos.h
        node/function/cos.cpp
        node/function/cosh.h
        node/function/cosh.cpp
        node/function/log.h
        node/function/log.cpp
        node/function/pow.h
        node/function/pow.cpp
        node/function/sin.h
        node/function/sin.cpp
        node/function/sinh.h
        node/function/sinh.cpp

        node/statement/statement.h
        node/statement/statement.cpp

        # Free-standing functions to manipulate a sym
        functions/mathematics.cpp
        functions/mathematics.h
        functions/operators.cpp
        functions/operators.h
        functions/symbolic.cpp
        functions/symbolic.h

        # Header to include everything
        sympp.h
        )

target_include_directories(sympp
        PUBLIC $<BUILD_INTERFACE:${SYMPP_ROOT_DIR}/source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(sympp PUBLIC cxx_std_17)

if (NOT WIN32 AND NOT APPLE)
    target_link_libraries(sympp PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif ()

target_link_libraries(sympp PRIVATE libtcc)

if (MSVC)
    target_compile_options(sympp PUBLIC /utf-8)
    target_compile_options(sympp PUBLIC /EHsc)
endif ()


# Set warning levels to about the same level for MSVC, GCC, and Clang
if (BUILD_WITH_PEDANTIC_WARNINGS)
    if (MSVC)
        target_compile_options(sympp PUBLIC /W4 /WX)
    else ()
        target_compile_options(sympp PUBLIC -Wall -Wextra -pedantic -Werror)
        if (APPLE)
            # -Wself-assign-overloaded has a false positive that breaks pybind11
            target_compile_options(sympp PUBLIC -Wno-self-assign-overloaded)
        endif ()
    endif ()
endif ()


#######################################################
### Installer                                       ###
#######################################################
if (BUILD_INSTALLER)
    # Install sympp target
    install(TARGETS sympp
            EXPORT SymPPTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            )

    # Install headers
    install(DIRECTORY ${SYMPP_ROOT_DIR}/source/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
            )

    # Install cmake script that imports the targets
    install(EXPORT SymPPTargets
            FILE SymPPTargets.cmake
            NAMESPACE SymPPTargets::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SymPP
            )
endif ()

