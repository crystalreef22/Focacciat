cmake_minimum_required(VERSION 3.16)

project(Focacciat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.5)

add_subdirectory(extensionhost)

qt_add_executable(Focacciat
    main.cpp
)


target_include_directories(Focacciat PRIVATE blockers)
qt_add_qml_module(Focacciat
    URI Focacciat
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        SOURCES
        QML_FILES
        QML_FILES TodoList.qml
        SOURCES
        SOURCES
        SOURCES todomodel.h todomodel.cpp
        SOURCES
        SOURCES
        SOURCES formatutils.h formatutils.cpp
        SOURCES todoitem.h todoitem.cpp
        RESOURCES media/alm_focus.mp3
        QML_FILES components/TimeInput.qml
        SOURCES components/timeinput.h components/timeinput.cpp
        SOURCES
        SOURCES blockers/extensionintegration.h blockers/extensionintegration.cpp
        RESOURCES extensionmanifest.json
        SOURCES blockers/blocklist.h blockers/blocklist.cpp
        SOURCES blockers/blocklistlistmodel.h blockers/blocklistlistmodel.cpp
        QML_FILES blockers/BlocklistPopup.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Focacciat PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Focacciat
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(Focacciat PRIVATE Qt6::Quick)
target_link_libraries(Focacciat PRIVATE Qt6::Core)
target_link_libraries(Focacciat PRIVATE Qt6::Network)
target_link_libraries(Focacciat PRIVATE Qt6::Core)

# include(GNUInstallDirs) # nothing outside of bundle
install(TARGETS Focacciat extensionhost
    BUNDLE DESTINATION .
    # LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The extension host should be inserted into the app

# if(APPLE)
#     add_custom_command(TARGET Focacciat POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy extensionhost/extensionhost Focacciat.app/Contents/MacOS
#     )
# endif()

# Now generate the deployment script

qt_generate_deploy_qml_app_script(
    TARGET Focacciat
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
