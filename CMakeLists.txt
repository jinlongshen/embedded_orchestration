cmake_minimum_required(VERSION 3.16)
project(embedded_orchestration VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(orchestrator
    src/main.cpp
    src/LlmClient.cpp
)

target_include_directories(orchestrator PRIVATE include)
target_link_libraries(orchestrator PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

