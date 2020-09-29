
if (__libtcc_extension_include_directories)
    return()
endif ()
set(__libtcc_extension_include_directories INCLUDED)

function(libtcc_extension_public_include_directories FLAG)
    list(APPEND ${FLAG} $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extension/include>)
    list(APPEND ${FLAG} $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extension/include>)
    set(${FLAG} ${${FLAG}} PARENT_SCOPE)
endfunction()
