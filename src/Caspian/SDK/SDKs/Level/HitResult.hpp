#pragma once

#include <Vectors.hpp>

struct BlockPos {
    int x, y, z;
};

enum class HitResultType : int {
    Tile = 0x0,
    Entity = 0x1,
    EntityOutOfRange = 0x2,
    NoHit = 0x3,
};

enum class FacingID : char {
    Unknown = -1,
    Down = 0,
    Up = 1,
    North = 2,
    South = 3,
    West = 4,
    East = 5,
};


class HitResult {
public:
    Vec3 startPos;
    Vec3 rayDir;
    HitResultType type;
    FacingID facing;
    BlockPos blockPos;
    Vec3 pos;

    [[nodiscard]]float distance() {
        return startPos.GetDistance(pos);
    }
};
