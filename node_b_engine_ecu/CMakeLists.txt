cmake_minimum_required(VERSION 3.10)

find_package(Threads REQUIRED)

set(NODE_B_SOURCES
    main.c
    engine_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/protocol.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/portable/ThirdParty/GCC/Posix/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/portable/MemMang/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
)

add_executable(node_b_engine_ecu ${NODE_B_SOURCES})

# 💡 [CORE LINK FIX] Link the real-time library (rt) and pthread for virtual shared memory
target_link_libraries(node_b_engine_ecu PRIVATE Threads::Threads rt)