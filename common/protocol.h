#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct __attribute__((packed)){
    uint8_t sequence_counter;
    uint8_t main_aps;
    uint8_t sub_aps;
    uint8_t checksum;
    uint8_t reserved[4];
}CanPedalPacket;

#define CAN_ID_PEDAL_DATA 0x100  // Node A가 쏘는 페달 데이터 ID
#define CAN_ID_FAULT_ALERT 0x7F0 // Node B가 고장 확정 시 Node C로 쏘는 비상 신호 ID

#endif