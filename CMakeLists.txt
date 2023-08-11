cmake_minimum_required(VERSION 3.21)
project(filing_system)

set(CMAKE_CXX_STANDARD 14)
include_directories(/opt/homebrew/Cellar/libmagic/5.45/include)

add_executable(filing_system main.cpp)

find_library(CORE_SERVICES_FRAMEWORK CoreServices)
find_library(LIBMAGIC magic REQUIRED)

target_link_libraries(filing_system
        ${CORE_SERVICES_FRAMEWORK}
        ${LIBMAGIC}
        )