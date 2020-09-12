#pragma once


#include <array>
#include <vector>
#include <stdint.h>

// typedef uint8_t byte;
typedef std::vector<uint8_t> packet;
typedef std::vector<packet> packets;
typedef std::vector<uint8_t> parameters;

typedef struct {
    uint8_t id;
    uint16_t count;
} ServoMove_t;

typedef struct {
    uint8_t id;
    uint16_t count;
    uint16_t speed;
} ServoMoveSpeed_t;

// typedef struct {
//     uint8_t id;
// } ServoInfo_t;

// typedef std::vector<ServoInfo_t> PingResponse;
// typedef std::vector<ServoMove_t> SyncMoveAngles;
// typedef std::vector<ServoMove_t> SyncMoveAngles;
