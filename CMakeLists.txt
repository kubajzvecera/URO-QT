cmake_minimum_required(VERSION 3.30)
project(alkokalkulacka)

# 1. SPRÁVNÁ CESTA (Zkontroluj, zda verze 6.11.0 v cestě sedí!)
set(CMAKE_PREFIX_PATH "/home/tucnak/Qt/6.11.0/gcc_64")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 2. HLEDÁNÍ QT6
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(alkokalkulacka main.cpp
        hlavicka.h
        alkokalkulacka.cpp
        alkokalkulacka.cpp)

# 3. LINKOVÁNÍ QT6 (Opraveno z Qt5 na Qt6)
target_link_libraries(alkokalkulacka
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)