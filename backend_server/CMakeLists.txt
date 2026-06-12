cmake_minimum_required(VERSION 3.16)

project(backend_server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматический поиск Qt в типичных путях установки Windows
if(WIN32 AND NOT DEFINED CMAKE_PREFIX_PATH)
    foreach(_ver 6.9.0 6.8.3 6.8.2 6.8.1 6.8.0 6.7.3 6.7.2 6.7.1 6.7.0)
        foreach(_kit mingw_64 msvc2022_64 msvc2019_64)
            list(APPEND CMAKE_PREFIX_PATH "C:/Qt/${_ver}/${_kit}")
        endforeach()
    endforeach()
    # MSYS2
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/mingw64" "C:/msys2/mingw64")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

add_executable(main_server
    main_serv.cpp
    functions_to_server.h functions_to_server.cpp
    mytcpserver.h mytcpserver.cpp
    databasemanager.h databasemanager.cpp
    algorithms.h algorithms.cpp
)

target_link_libraries(main_server
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

include(GNUInstallDirs)
install(TARGETS main_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
