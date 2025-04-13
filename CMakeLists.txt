cmake_minimum_required(VERSION 3.30)
project(sysaccess C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(sysaccess SHARED
        src/cpu.c
        src/ram.c
        src/user.c
        src/system.c
        src/disk.c
        src/net.c
        src/snapshot.c
        src/snapshot_json.c
)

if (WIN32)
    target_link_libraries(sysaccess PRIVATE
            pdh
            iphlpapi
            ws2_32
            wininet
    )
endif()

add_executable(test_cpu
        tests/test.c
)

target_link_libraries(test_cpu sysaccess)
