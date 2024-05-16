cmake_minimum_required(VERSION 3.16)

project(anilist_project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS NetworkAuth)

qt_standard_project_setup()

qt_add_executable(appanilist_project
    main.cpp
)

qt_add_qml_module(appanilist_project
    URI anilist_project
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES networkmanager.h networkmanager.cpp
        SOURCES qsdata.h qsdata.cpp
        SOURCES anilist.h anilist.cpp
        SOURCES oauthimplicit.h oauthimplicit.cpp
        SOURCES filewriter.h filewriter.cpp
        SOURCES settings.h settings.cpp
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appanilist_project PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appanilist_project
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appanilist_project
    PRIVATE Qt6::Quick
    PRIVATE Qt6::NetworkAuth
)

include(GNUInstallDirs)
install(TARGETS appanilist_project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
