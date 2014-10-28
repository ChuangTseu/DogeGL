project(DogeGL)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

IF (MSVC)

ELSE(MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # Warnings
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fms-extensions") # features
ENDIF(MSVC)


find_package(SDL2 REQUIRED) # SDL2

find_package(OpenGL REQUIRED) # OpenGL

find_package(GLEW REQUIRED) # GLEW

find_package(assimp REQUIRED) # assimp

IF(WIN32)
    add_definitions(-DWIN32)
ENDIF(WIN32)


file(GLOB SHADER_FILES bin_environment/*.frag bin_environment/*.vert bin_environment/*.glsl
                       bin_environment/*.tesc bin_environment/*.tese)

install(DIRECTORY bin_environment/ DESTINATION shaders
          FILES_MATCHING PATTERN "*.frag")

message("SHADER_FILES ${SHADER_FILES}")

add_executable(${PROJECT_NAME} ${SHADER_FILES} ${SRC_LIST})

include_directories(. ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})

add_subdirectory(MathsTools)
add_subdirectory(Graphics)

target_link_libraries(${PROJECT_NAME} MATHSTOOLS GRAPHICS)

message(${GLEW_LIBRARY})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARY})

