cmake_minimum_required (VERSION 2.6)
project (SparseMatTEST)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")



#########################################################
# Include Files
#########################################################
add_executable(outputSparseMatTEST mainSparseMatTEST.cpp )

########################################################
# Linking & stuff
#########################################################

# create the program "Gunvarrel"
#target_link_libraries(Gunvarrel ${OPENGL_LIBRARIES} ${SDL_LIBRARY} )

