cmake_minimum_required(VERSION 3.16)

project(FocusAssist9 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.5)

add_subdirectory(extensionhost)

qt_add_executable(FocusAssist9
    main.cpp
)

qt_add_qml_module(FocusAssist9
    URI FocusAssist9
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
        SOURCES extensionintegration.h extensionintegration.cpp
        RESOURCES extensionmanifest.json
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(FocusAssist9 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FocusAssist9
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(FocusAssist9 PRIVATE Qt6::Quick)
target_link_libraries(FocusAssist9 PRIVATE Qt6::Core)
target_link_libraries(FocusAssist9 PRIVATE Qt6::Network)

# include(GNUInstallDirs) # nothing outside of bundle
install(TARGETS FocusAssist9 extensionhost
    BUNDLE DESTINATION .
    # LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The extension host should be inserted into the app

# if(APPLE)
#     add_custom_command(TARGET FocusAssist9 POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy extensionhost/extensionhost FocusAssist9.app/Contents/MacOS
#     )
# endif()

# Now generate the deployment script

qt_generate_deploy_qml_app_script(
    TARGET FocusAssist9
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
