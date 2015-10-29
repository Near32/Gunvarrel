cmake_minimum_required (VERSION 2.6)
project (Gunvarrel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

#########################################################
# FIND SDL
#########################################################
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})
link_directories(${SDL_LIBRARY_DIRS})
add_definitions(${SDL_DEFINITIONS})
if(NOT SDL_FOUND)
    message(ERROR " SDL not found!")
endif(NOT SDL_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################
# Include Files
#########################################################
add_executable(Gunvarrel src/TrackBallCamera.cpp src/common/objloader.cpp src/main.cpp)

########################################################
# Linking & stuff
#########################################################

# create the program "Gunvarrel"
target_link_libraries(Gunvarrel ${OPENGL_LIBRARIES} ${SDL_LIBRARY} )

