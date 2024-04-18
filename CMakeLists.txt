cmake_minimum_required(VERSION 3.5)

project(cake LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

add_executable(cake
    main.cc
    mainwindow.h mainwindow.cc
    candylist.h candylist.cc
    candyitem.h candyitem.cc
    detailarea.h detailarea.cc
    startoption.h startoption.cc
    feedback.h feedback.cc
    images.qrc admin.rc
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)

set_target_properties(cake PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(FetchContent)
FetchContent_Declare(
    candy
    GIT_REPOSITORY https://github.com/lanthora/candy.git
    GIT_TAG        3c21271c7889cca3bf19f5fd3b538149d855ecb3
)
set(CANDY_DEVEL "1" CACHE BOOL "" FORCE)
set(CANDY_NOEXE "1" CACHE BOOL "" FORCE)
FetchContent_GetProperties(candy)
if(NOT candy_POPULATED)
    FetchContent_Populate(candy)
    add_subdirectory(${candy_SOURCE_DIR} ${candy_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
include_directories(${candy_SOURCE_DIR}/src/cffi/)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE libcandy)

include(GNUInstallDirs)
install(TARGETS cake
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
