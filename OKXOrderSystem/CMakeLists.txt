cmake_minimum_required(VERSION 3.10)
project(OKXOrderSystem)

set(CMAKE_CXX_STANDARD 17)

set(VCPKG_TARGET_TRIPLET "x64-windows")
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS})

add_executable(okx_order_system main.cpp okx_client.cpp)
target_link_libraries(okx_order_system ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
