cmake_minimum_required(VERSION 3.24)

project(recursive_mutex)

set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use clean C++ without compiler-specific extensions

find_package(Boost 1.90.0 CONFIG REQUIRED COMPONENTS program_options json)

add_executable(${PROJECT_NAME}
               src/main.cpp)

set_target_properties(${PROJECT_NAME}
                      PROPERTIES
                        CXX_STANDARD            20
                        EXPORT_COMPILE_COMMANDS On)

target_link_libraries(${PROJECT_NAME}
                      Boost::boost
                      Boost::program_options
                      Boost::json)