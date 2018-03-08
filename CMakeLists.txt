cmake_minimum_required(VERSION 3.9)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(QCameraBug)

# required packages
find_package(Qt5 COMPONENTS Core Quick Multimedia REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "cannyfilter.cpp" "main.qrc")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Quick
    Qt5::Multimedia
    ${OpenCV_LIBS}
)
