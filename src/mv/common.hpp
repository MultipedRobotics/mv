#pragma once


#include <vector>
#include <stdint.h>

typedef uint8_t byte;
typedef std::vector<byte> packet;
typedef std::vector<packet> packets;
typedef std::vector<byte> parameters;

typedef struct {
    byte id;
    double angle;
    double speed;
} ServoMove_t;

typedef struct {
    byte id;
} ServoInfo_t;

typedef std::vector<ServoInfo_t> PingResponse;
typedef std::vector<ServoMove_t> SyncMoveAngles;
