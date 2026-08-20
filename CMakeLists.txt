cmake_minimum_required(VERSION 3.25)

project(ChessEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(ChessEngine 
src/main.cpp
src/Board.cpp
src/Piece.cpp
src/PieceTextures.cpp
src/Renderer.cpp
src/ChessRules.cpp
)

target_include_directories(ChessEngine PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(ChessEngine PRIVATE 
SFML::Graphics 
SFML::Window 
SFML::System)