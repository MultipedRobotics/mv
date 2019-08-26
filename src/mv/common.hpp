#pragma once

namespace yarp

#include <vector>
#include <stdint.h>

typedef uint8_t byte;
typedef std::vector<byte> packet;
typedef std::vector<byte> parameters;

typedef struct {
    byte id;
    double angle;
    double speed;
} ServoMove_t;

typedef struct {
    byte id;
} ServoInfo_t;

typedef vector<ServoInfo_t> PingResponse;
typedef vector<ServoMove_t> SyncMoveAngles;
