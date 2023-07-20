# zmq-test
Brief test of ZMQ on Unix Domain sockets

## Overview

The test contains of 2 programs `sender` and `receiver`.

The `receiver` starts and waits for messages until a message "exit" appears.
The `sender` takes a command line argument, send it to the `receiver` and exists.

## Installation

### Windows

First, install vcpkg following instructions from the [vcpkg GitHub](https://github.com/microsoft/vcpkg) page.

vcpkg.exe install zeromq:x64-windows
vcpkg install cppzmq:x64-windows

cd build-windows
cmake ..
cmake --build .

### Linux 

sudo apt install cppzmq-dev

cd build-linux
cmake ..
cmake --build .


