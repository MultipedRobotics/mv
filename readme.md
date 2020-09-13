# mv

![Cpp](https://github.com/MultipedRobotics/mv/workflows/Cpp/badge.svg)

**Under Heavey Development**

Yet another Dynamexl library for smart servos. This is call `mv` after the
unix move command.

**Sort of a mess right now** :smile:

## Cool

- [termcolor](https://github.com/ikalnytskyi/termcolor)

## Examples

```cpp
#include <iostream>
#include "ax12.hpp"

// typedef struct {
//     byte id;
//     double angle;
//     double speed;
// } ServoMove_t;
//
// typedef struct {
//     byte id;
// } ServoInfo_t;
//
// typedef vector<ServoInfo_t> PingResponse;
// typedef vector<ServoMove_t> SyncMoveAngles;

AX12 servo;

packet ping = servo.make_ping(AX12::BROADCAST_ADDR);
PingResponse ans = servo.send(ping);

for (auto const& s: ans) cout << s.id << endl;

SyncMoveAngles d = {{1, 25, 0}, {2,-79, 0}};

packet pkt = servo.make_sync_angle_packet(d)
servo.send(pkt);
```
