cmake_minimum_required(VERSION 3.5)
project(resto_simulation_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE LOGGER_SOURCE "src/com/fiuba/resto/logger/*.cpp")
file(GLOB_RECURSE PROCESS_SOURCE "src/com/fiuba/resto/process/*.cpp")
file(GLOB_RECURSE PROCESS_ACTION_SOURCE "src/com/fiuba/resto/process/action/*.cpp")
file(GLOB_RECURSE TYPES_SOURCE "src/com/fiuba/resto/types/*.cpp")
file(GLOB_RECURSE UTILS_SOURCE "src/com/fiuba/resto/utils/*.cpp")
file(GLOB_RECURSE UTILS_SIGNALS_SOURCE "src/com/fiuba/resto/utils/signals/*.cpp")
file(GLOB_RECURSE PARSER_SOURCE "src/com/fiuba/resto/parser/*.cpp")
file(GLOB_RECURSE JSON_SOURCE "src/com/fiuba/resto/parser/*.hpp")

add_library(resto ${LOGGER_SOURCE} ${PROCESS_SOURCE} ${PROCESS_ACTION_SOURCE} ${TYPES_SOURCE} ${UTILS_SOURCE} ${UTILS_SIGNALS_SOURCE} ${PARSER_SOURCE} ${JSON_SOURCE})

add_executable(resto_simulation_c ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/main/RestoMain.cpp)

target_link_libraries(resto_simulation_c resto)

target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/logger)
target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/process)
target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/process/action)
target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/types)
target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/utils)
target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/utils/signals)
target_include_directories(resto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/com/fiuba/resto/parser)
