cmake_minimum_required(VERSION 3.5)

project(CMinusLexer_library)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

############################################################
# Create a library
############################################################
file(GLOB SOURCES "src/static/*.cpp")
# Generate the static library from the library sources
add_library(CMinusLexer_library STATIC 
    ${SOURCES}
)

target_include_directories(CMinusLexer_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)



############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(CMinusLexer_binary
    src/main.cpp
)

# link the new hello_library target with the hello_binary target
target_link_libraries(CMinusLexer_binary
    PRIVATE 
        CMinusLexer_library
)
