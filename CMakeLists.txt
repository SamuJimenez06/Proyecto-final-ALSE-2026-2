cmake_minimum_required(VERSION 3.10)
project(ProyectoDomotica)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Sql Charts REQUIRED)
find_library(SQLITE3_LIBRARY NAMES sqlite3 REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")
file(GLOB FORMS "src/*.ui")

add_executable(ProyectoFinal ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(ProyectoFinal PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Charts ${SQLITE3_LIBRARY})
