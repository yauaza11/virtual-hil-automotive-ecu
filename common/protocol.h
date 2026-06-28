#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct __attribute__((packed)){
    uint8_t sequence_counter;
    uint8_t main_aps;
    uint8_t sub_aps;
    uint8_t checksum;
}CanPedalPacket;

// 💡 Node B -> Node C (엔진 RPM 및 상태 패킷)
typedef struct __attribute__((packed)){
    uint8_t status_and_counter; // [ 4비트 데이터ID | 4비트 시퀀스 ]
    uint8_t rpm_high;           // RPM 상위 8비트
    uint8_t rpm_low;            // RPM 하위 8비트
    uint8_t checksum;           // AUTOSAR CRC-8
} CanEnginePacket;

#define CAN_ID_PEDAL_DATA 0x100  // Node A가 쏘는 페달 데이터 ID
#define CAN_ID_ENGINE_DATA 0x200 
#define CAN_ID_FAULT_ALERT 0x7F0 // Node B가 고장 확정 시 Node C로 쏘는 비상 신호 ID

#define PEDAL_DATA_ID       0x5
#define CLUSTER_DATA_ID     0x7   // ⭕ 계기판용 ID 추가 완료!

#endif