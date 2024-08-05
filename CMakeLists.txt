cmake_minimum_required(VERSION 3.28)
project(FalloutHacking LANGUAGES CXX)

add_executable(FalloutHacking main.cpp character.cpp file_manager.cpp hacking_handler.cpp mouse_control.cpp symbol.cpp word.cpp)
target_link_libraries(FalloutHacking PRIVATE sfml-graphics)
target_link_libraries(FalloutHacking PRIVATE sfml-window)
target_link_libraries(FalloutHacking PRIVATE sfml-system)
target_compile_features(FalloutHacking PRIVATE cxx_std_20)