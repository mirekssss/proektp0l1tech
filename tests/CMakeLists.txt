cmake_minimum_required(VERSION 3.16)

project(test_algorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Автоматический поиск Qt в типичных путях установки Windows
if(WIN32 AND NOT DEFINED CMAKE_PREFIX_PATH)
    foreach(_ver 6.9.0 6.8.3 6.8.2 6.8.1 6.8.0 6.7.3 6.7.2 6.7.1 6.7.0)
        foreach(_kit mingw_64 msvc2022_64 msvc2019_64)
            list(APPEND CMAKE_PREFIX_PATH "C:/Qt/${_ver}/${_kit}")
        endforeach()
    endforeach()
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/mingw64" "C:/msys2/mingw64")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

add_executable(test_algorithms
    test_algorithms.cpp
    ../backend_server/algorithms.h
    ../backend_server/algorithms.cpp
)

target_include_directories(test_algorithms PRIVATE ../backend_server)

target_link_libraries(test_algorithms
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
)

enable_testing()
add_test(NAME AlgorithmsTest COMMAND test_algorithms)
