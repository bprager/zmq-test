cmake_minimum_required(VERSION 3.10)

project(MyZMQProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Windows configuration using vcpkg
if(WIN32)
    # Set the vcpkg root directory
    set(VCPKG_ROOT "C:/tools/vcpkg" CACHE PATH "Vcpkg root directory")

    # Use vcpkg toolchain file
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")

    # Find ZeroMQ and cppzmq
    find_package(ZeroMQ CONFIG REQUIRED)
    find_package(cppzmq CONFIG REQUIRED)

    # Include directories for ZeroMQ and cppzmq
    include_directories("${VCPKG_ROOT}/packages/zeromq_x64-windows/include")
    include_directories("${VCPKG_ROOT}/packages/cppzmq_x64-windows/include")

    # Create the receiver and sender executables
    add_executable(receiver src/receiver.cpp)
    add_executable(sender src/sender.cpp)

    # Link ZeroMQ and cppzmq
    target_link_libraries(receiver PRIVATE libzmq cppzmq)
    target_link_libraries(sender PRIVATE libzmq cppzmq)
# Linux configuration
elseif(UNIX)
    # Find ZeroMQ
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ZEROMQ REQUIRED libzmq)

    # Find cppzmq
    find_path(CPPZMQ_INCLUDE_DIR zmq.hpp PATHS /usr/include)
    find_library(CPPZMQ_LIBRARY NAMES zmq)

    # Include directory for cppzmq
    include_directories(${CPPZMQ_INCLUDE_DIR})

    # Create the receiver and sender executables
    add_executable(receiver src/receiver.cpp)
    add_executable(sender src/sender.cpp)

    # Link ZeroMQ and cppzmq
    target_link_libraries(receiver PRIVATE ${ZEROMQ_LIBRARIES} ${CPPZMQ_LIBRARY})
    target_link_libraries(sender PRIVATE ${ZEROMQ_LIBRARIES} ${CPPZMQ_LIBRARY})
endif()
