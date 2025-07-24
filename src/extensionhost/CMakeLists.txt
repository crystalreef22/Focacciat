cmake_minimum_required(VERSION 3.16)

project(extensionhost VERSION 0.1 LANGUAGES CXX)
# project(extensionhost-test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES
    *.h
    *.cpp
)

if(WIN32)
    add_executable(extensionhost
        main_win.cpp
    )
else()
    add_executable(extensionhost
        main_unix.cpp
    )
    # add_executable(extensionhost-test testserver.cpp )
endif()

set_target_properties(extensionhost PROPERTIES
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE TRUE
)
# set_target_properties(extensionhost-test PROPERTIES MACOSX_BUNDLE FALSE WIN32_EXECUTABLE TRUE )

include(GNUInstallDirs)
install(TARGETS extensionhost
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

