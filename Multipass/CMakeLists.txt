cmake_minimum_required(VERSION 3.10)
project(UbuntuImageFetcher)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(UbuntuImageFetcher main.cpp)
target_link_libraries(UbuntuImageFetcher ${CURL_LIBRARIES})