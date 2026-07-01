#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "protocol.h"

CAN_Bus_TypeDef *pVirtualBus = NULL;

void vInitVirtualCANBus(void)
{
    int fd = shm_open("/vCAN_Bus_Matrix", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(CAN_Bus_TypeDef));
    pVirtualBus = (CAN_Bus_TypeDef *)mmap(0, sizeof(CAN_Bus_TypeDef), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

uint8_t u8CalculateAUTOSARCRC8(const uint8_t *pu8Data, uint8_t u8Length)
{
    uint8_t u8Crc = 0xFF;
    for (uint8_t i = 0; i < u8Length; i++) {
        u8Crc ^= pu8Data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if ((u8Crc & 0x80) != 0) u8Crc = (uint8_t)((u8Crc << 1) ^ 0x2F);
            else u8Crc <<= 1;
        }
    }
    return (u8Crc ^ 0xFF);
}