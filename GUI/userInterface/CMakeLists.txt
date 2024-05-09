cmake_minimum_required(VERSION 3.16)

project(userInterface VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 6.2 COMPONENTS Quick Multimedia Network REQUIRED)



qt_standard_project_setup()

qt_add_executable(appuserInterface
    main.cpp
    playercontroller.h playercontroller.cpp
    doorstate.h doorstate.cpp
    batteryindicator.h batteryindicator.cpp
    client.h                 client.cpp
    tempindicator.h          tempindicator.cpp
    #sensorclient.h sensorclient.cpp



)

qt_add_qml_module(appuserInterface
    URI userInterface
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES
        QML_FILES AudioInfoBox.qml
        QML_FILES ImageButton.qml

        SOURCES
        audioinfo.h audioinfo.cpp
        client.h client.cpp


        RESOURCES
        assets/images/song_1.jpg
        assets/images/song_2.jpg
        assets/images/song_3.jpg

        assets/icons/app_icon.ico
        assets/icons/next_icon.png
        assets/icons/previous_icon.png
        assets/icons/play_icon.png
        assets/icons/pause_icon.png

        assets/videos/video_1.avi

        assets/audio/symphony_no_5.mp3
        assets/audio/air_on_the_g_string.mp3
        assets/audio/eine_kleine_nachtmusik.mp3







)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuserInterface PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.userInterface
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuserInterface
    PRIVATE Qt6::Quick  Qt6::Multimedia Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS appuserInterface
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
