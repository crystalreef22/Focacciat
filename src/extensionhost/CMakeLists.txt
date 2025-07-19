cmake_minimum_required(VERSION 3.16)

project(extensionhost VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(extensionhost
    main.cpp
)

set_target_properties(extensionhost PROPERTIES
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS extensionhost
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

