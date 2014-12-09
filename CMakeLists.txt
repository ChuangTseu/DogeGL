project(DogeGL)
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)

aux_source_directory(. SRC_LIST)

#if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
#endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

IF (MSVC)

ELSE(MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # Warnings
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fms-extensions -g") # features
ENDIF(MSVC)


find_package(OpenGL REQUIRED) # OpenGL

find_package(GLEW REQUIRED) # GLEW

find_package(assimp REQUIRED) # assimp

find_package(DevIL REQUIRED) # DevIL


IF(WIN32)
    add_definitions(-DWIN32)
ENDIF(WIN32)


file(GLOB SHADER_FILES bin_environment/*.frag bin_environment/*.vert bin_environment/*.glsl
                       bin_environment/*.tesc bin_environment/*.tese bin_environment/*.geom)

install(DIRECTORY bin_environment/ DESTINATION shaders
          FILES_MATCHING PATTERN "*.frag")

add_executable(${PROJECT_NAME} ${SHADER_FILES} ${SRC_LIST})


IF(NOT DOGEGL_WINDOW_MANAGER)
    set(DOGEGL_WINDOW_MANAGER QT)
ENDIF()

## TMP HACK FOR QT KEYBOARD ENUM ##
    find_package(Qt5Core)
    include_directories(${Qt5Core_INCLUDE_DIRS})
## ##


IF(DOGEGL_WINDOW_MANAGER STREQUAL "SDL2")
    find_package(SDL2 REQUIRED) # SDL2

    include_directories(${SDL2_INCLUDE_DIR})

    add_definitions(-DUSE_SDL2)

    add_subdirectory(Sdl2GUI)

    target_link_libraries(${PROJECT_NAME} Sdl2GUI)

ELSEIF(DOGEGL_WINDOW_MANAGER STREQUAL "QT")
    find_package(Qt5Core)
    include_directories(${Qt5Core_INCLUDE_DIRS})

    add_definitions(-DUSE_QT)

    add_subdirectory(QtGUI)

    target_link_libraries(${PROJECT_NAME} QtGUI)


ELSE()
    error("DOGEGL_WINDOW_MANAGER ${DOGEGL_WINDOW_MANAGER} not available")
ENDIF()



include_directories(. ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${DEVIL_INCLUDE_DIR})

add_subdirectory(MathsTools)
add_subdirectory(Graphics)

target_link_libraries(${PROJECT_NAME} MATHSTOOLS GRAPHICS)


target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARY} ${DEVIL_LIBRARY})

target_link_libraries(${PROJECT_NAME})

