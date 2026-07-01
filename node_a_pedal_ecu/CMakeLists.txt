cmake_minimum_required(VERSION 3.10)

# 💡 리눅스 멀티스레드 라이브러리 체크
find_package(Threads REQUIRED)

# (main.c)과 페달 핵심 로직(pedal_task.c) 묶기
set(NODE_A_SOURCES
    main.c
    pedal_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/protocol.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/portable/ThirdParty/GCC/Posix/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/portable/MemMang/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freertos/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
)

# 가속 페달 실행 파일 생성 지시
add_executable(node_a_pedal_ecu ${NODE_A_SOURCES})

# 💡 [최종 진압 핵심] 리눅스 공유 메모리(rt)와 멀티스레드 접착제를 강제로 연결해 조립을 끝냅니다!
target_link_libraries(node_a_pedal_ecu PRIVATE Threads::Threads rt)