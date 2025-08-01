cmake_minimum_required(VERSION 3.16)

project(kicad-installer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(unofficial-minizip CONFIG REQUIRED)

set(
    TS_FILES
    i18n/kicad-installer_zh_CN.ts
)

set(
    RES_FILES
    resources/kicad_installer.qrc
    resources/icons/kicad_installer.rc
)

set(PROJECT_SOURCES
    main.cpp
    ${TS_FILES}
    ${RES_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kicad-installer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        app/pages/pageconfigportable.h app/pages/pageconfigportable.cpp app/pages/pageconfigportable.ui
        app/pages/pagedownloadprogress.h app/pages/pagedownloadprogress.cpp app/pages/pagedownloadprogress.ui
        app/logo/logowidget.h app/logo/logowidget.cpp app/logo/logowidget.ui
        app/pages/pageopendownloaddir.h app/pages/pageopendownloaddir.cpp app/pages/pageopendownloaddir.ui
        app/pages/pageselectsavepath.h app/pages/pageselectsavepath.cpp app/pages/pageselectsavepath.ui
        app/pages/pageconfcontainer.h app/pages/pageconfcontainer.cpp app/pages/pageconfcontainer.ui
        app/pages/pageselectinstallmethod.h app/pages/pageselectinstallmethod.cpp app/pages/pageselectinstallmethod.ui
    )

    # Define target properties for Android with Qt 6 as:
    # set_property(TARGET kicad-installer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    # ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(kicad-installer SHARED
            ${PROJECT_SOURCES}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(kicad-installer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_subdirectory(libs)
add_subdirectory(app)

target_link_libraries(kicad-installer
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    QWKWidgets
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    cpr::cpr
    unofficial::minizip::minizip
)

target_link_directories(
    kicad-installer
    PRIVATE SYSTEM
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
    "${CMAKE_CURRENT_SOURCE_DIR}/app"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.kicad-installer)
endif()

set_target_properties(kicad-installer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS kicad-installer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(kicad-installer)
endif()
