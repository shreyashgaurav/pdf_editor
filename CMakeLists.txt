cmake_minimum_required(VERSION 3.16)
project(PDFEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Pdf PdfWidgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

qt_add_executable(PDFEditor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(PDFEditor PRIVATE
    Qt6::Widgets
    Qt6::Pdf
    Qt6::PdfWidgets
)

install(TARGETS PDFEditor)
qt_finalize_executable(PDFEditor)













# cmake_minimum_required(VERSION 3.21)
# project(PDFEditor VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt6 REQUIRED COMPONENTS Widgets Pdf PdfWidgets)

# set(PROJECT_SOURCES
#     main.cpp
#     mainwindow.cpp
#     mainwindow.h
#     mainwindow.ui
# )

# qt_add_executable(PDFEditor
#     MANUAL_FINALIZATION
#     ${PROJECT_SOURCES}
# )

# target_link_libraries(PDFEditor PRIVATE
#     Qt6::Widgets
#     Qt6::Pdf
#     Qt6::PdfWidgets
# )

# # Optional: wire qpdf if you add it via vcpkg later
# option(WITH_QPDF "Enable qpdf-based editing" OFF)
# if (WITH_QPDF)
#     find_package(qpdf CONFIG REQUIRED)        # vcpkg provides this target
#     target_link_libraries(PDFEditor PRIVATE qpdf)
#     target_compile_definitions(PDFEditor PRIVATE HAVE_QPDF=1)
# endif()

# install(TARGETS PDFEditor)
# qt_finalize_executable(PDFEditor)








# cmake_minimum_required(VERSION 3.16)

# project(pdf_Editor VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# set(PROJECT_SOURCES
#         main.cpp
#         mainwindow.cpp
#         mainwindow.h
#         mainwindow.ui
# )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(pdf_Editor
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET pdf_Editor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
# else()
#     if(ANDROID)
#         add_library(pdf_Editor SHARED
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(pdf_Editor
#             ${PROJECT_SOURCES}
#         )
#     endif()
# endif()

# target_link_libraries(pdf_Editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.pdf_Editor)
# endif()
# set_target_properties(pdf_Editor PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# include(GNUInstallDirs)
# install(TARGETS pdf_Editor
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(pdf_Editor)
# endif()
