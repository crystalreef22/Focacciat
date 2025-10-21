cmake_minimum_required(VERSION 3.21)

project(Focacciat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

find_package(Qt6 REQUIRED COMPONENTS Core Quick Network Widgets)

qt_standard_project_setup(REQUIRES 6.9) # Older versions MAY work, but you would
                                        # need to set necessary qt_policies

add_subdirectory(extensionhost)  # FIXME: extensionintegration

# FIXME
if(APPLE)
set(MACOSX_BUNDLE_ICON_FILE focusassisticon)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/focacciat-icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")
set(extensionhost_binary "${extensionhost_BINARY_DIR}/extensionhost")
set_source_files_properties(${extensionhost_binary} PROPERTIES
       MACOSX_PACKAGE_LOCATION "MacOS")
qt_add_executable(Focacciat MACOSX_BUNDLE main.cpp ${app_icon_macos} ${extensionhost_binary})
else()
qt_add_executable(Focacciat
    main.cpp
)
endif()


# Required for QML integration
target_include_directories(Focacciat
    PRIVATE
        blockers
        components
        settings
)

qt_add_qml_module(Focacciat
    URI Focacciat
    VERSION 1.0
    QML_FILES
        DebugTodoView.qml
        DebugView.qml
        Main.qml
        ProjectView.qml
        TimerCircle.qml
        TodoView.qml
        settings/BlocklistPopup.qml
        components/TimeInput.qml
    SOURCES
        blockers/blocklist.h blockers/blocklist.cpp
        blockers/blocklistlistmodel.h blockers/blocklistlistmodel.cpp
        blockers/extensionintegration.h blockers/extensionintegration.cpp
        components/abstracttimeinput.h components/abstracttimeinput.cpp
        formatutils.h formatutils.cpp
        globalstate.h globalstate.cpp
        maskedapplicationwindow.h maskedapplicationwindow.cpp
        projectitem.h projectitem.cpp
        projectmodel.h projectmodel.cpp
        projectprogressmodel.h projectprogressmodel.cpp
        todoitem.h todoitem.cpp
        todomodel.h todomodel.cpp
    RESOURCES
        extensionmanifest.json
        media/alm_focus.mp3
        QML_FILES components/FlatButton.qml
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
target_link_libraries(Focacciat PRIVATE Qt6::Core)
target_link_libraries(Focacciat PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Focacciat PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS Focacciat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


# Now generate the deployment script

qt_generate_deploy_qml_app_script(
    TARGET Focacciat
    OUTPUT_SCRIPT deploy_script
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})
