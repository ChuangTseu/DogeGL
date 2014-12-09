SET(LIBRARY_VERSION "0.1.0")
SET(LIBRARY_NAME "QtGUI")

FILE(GLOB src *)

file(GLOB_RECURSE QT_UI_FILES *.ui)

# Qt
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

find_package(OpenGL REQUIRED) # OpenGL

include_directories(
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5OpenGL_INCLUDE_DIRS}

        ${CMAKE_CURRENT_BINARY_DIR}
)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

qt5_wrap_ui(UI_HEADER_FILES ${QT_UI_FILES})
message("QT_UI_FILES: ${QT_UI_FILES}")

set(QT_USE_OPENGL TRUE)

ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_LIBRARY(QtGUI STATIC ${src} ${QT_UI_FILES})

target_link_libraries(QtGUI Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL GRAPHICS ${OPENGL_LIBRARIES})

SET_TARGET_PROPERTIES(QtGUI PROPERTIES VERSION ${LIBRARY_VERSION})


