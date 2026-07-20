cmake_minimum_required(VERSION 3.28)

project(cake LANGUAGES CXX)

set (CANDY_VERSION "6.1.9")
set (CAKE_RELEASE "1")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CAKE_VERSION "${CANDY_VERSION}-${CAKE_RELEASE}") 
configure_file("setup.iss.in" "setup.iss" @ONLY)
add_compile_definitions(CAKE_VERSION="${CAKE_VERSION}")

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Network)
find_package(Poco REQUIRED COMPONENTS Foundation JSON Net NetSSL)

add_executable(cake
    define.h main.cc
    mainwindow.h mainwindow.cc
    candyitem.h candyitem.cc
    detailarea.h detailarea.cc
    startoption.h startoption.cc
    feedback.h feedback.cc

    update.h update.cc
    about.h about.cc
    images.qrc admin.rc
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Network Poco::Foundation Poco::JSON Poco::Net Poco::NetSSL)

set_target_properties(cake PROPERTIES
    QT_TARGET_DESCRIPTION "Cake"
    WIN32_EXECUTABLE TRUE
)

include(FetchContent)
FetchContent_Declare(
    candy
    GIT_REPOSITORY https://github.com/lanthora/candy.git
    GIT_TAG        "v${CANDY_VERSION}"
    EXCLUDE_FROM_ALL
)
set(CANDY_DEVEL "1" CACHE BOOL "" FORCE)
set(CANDY_NOEXE "1" CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(candy)
include_directories(${candy_SOURCE_DIR}/candy/include/)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE candy-library)

include(GNUInstallDirs)
install(TARGETS cake)
