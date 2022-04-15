cmake_minimum_required(VERSION 3.5)

project(CMinusParser_library)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

############################################################
# Create a library
############################################################
file(GLOB SOURCES "src/static/*/*.cpp" "src/static/*.cpp")

# Generate the static library from the library sources
add_library(CMinusParser_library STATIC 
    ${SOURCES}
)

target_include_directories(CMinusParser_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)



############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(CMinusParser_binary
    src/main.cpp
)

# link the new hello_library target with the hello_binary target
target_link_libraries(CMinusParser_binary
    PRIVATE 
        CMinusParser_library
)
