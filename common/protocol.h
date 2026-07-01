#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct __attribute__((packed)){
    uint8_t sequence_counter;
    uint8_t main_aps;
    uint8_t sub_aps;
    uint8_t checksum;
} CanPedalPacket;

typedef struct __attribute__((packed)){
    uint8_t status_and_counter; 
    uint8_t rpm_high;           
    uint8_t rpm_low;            
    uint8_t checksum;           
} CanEnginePacket;

#define CAN_ID_PEDAL_DATA  0x100  
#define CAN_ID_ENGINE_DATA 0x200  

#define PEDAL_DATA_ID      0x5    
#define CLUSTER_DATA_ID    0x7    

typedef struct {
    uint32_t node_a_slot; 
    uint32_t node_b_slot; 
} CAN_Bus_TypeDef;

extern CAN_Bus_TypeDef *pVirtualBus;

void vInitVirtualCANBus(void);
uint8_t u8CalculateAUTOSARCRC8(const uint8_t *pu8Data, uint8_t u8Length);

#endif /* PROTOCOL_H */