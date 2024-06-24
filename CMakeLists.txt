cmake_minimum_required(VERSION 3.16)

project(anilist_project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# Find the required Qt modules
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick NetworkAuth Sql)

qt_standard_project_setup()

# Add the executable
qt_add_executable(appanilist_project
    main.cpp
    assets/gql.qrc
)

# Add the QML module
qt_add_qml_module(appanilist_project
    URI anilist_project
    VERSION 1.0
    QML_FILES
        Main.qml
        src/QML/Sidebar.qml
        src/QML/IconButton.qml
    SOURCES
        src/anime.cpp
        src/animelistmodel.cpp
        src/animedetailmodel.cpp
        src/networkmanager.cpp
        src/anilist.cpp
        src/oauthimplicit.cpp
        src/filewriter.cpp
        src/settings.cpp
        src/appsettingskey.cpp
        src/appresourcekey.cpp
        src/resources.cpp
        src/databasemanager.cpp

        assets/anime.h
        assets/animelistmodel.h
        assets/animedetailmodel.h
        assets/networkmanager.h
        assets/anilist.h
        assets/oauthimplicit.h
        assets/filewriter.h
        assets/settings.h
        assets/appsettingskey.h
        assets/appresourcekey.h
        assets/resources.h
        assets/databasemanager.h

    RESOURCES
        assets/gql.qrc
        QML_FILES src/QML/AnimeList.qml
        QML_FILES src/QML/AnimeLibrary.qml
        QML_FILES
        QML_FILES src/QML/AppSettings.qml
        RESOURCES src/QML/qmldir
        SOURCES assets/initdb.h
)

# Add the assets and src directories to the include directories
target_include_directories(appanilist_project
    PRIVATE assets
    PRIVATE src
)

set_source_files_properties(AppSettings.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

# Set target properties
set_target_properties(appanilist_project PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Link the required Qt libraries
target_link_libraries(appanilist_project
    PRIVATE Qt6::Quick
    PRIVATE Qt6::NetworkAuth
    PRIVATE Qt6::Sql
)

# Installation setup
include(GNUInstallDirs)
install(TARGETS appanilist_project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
