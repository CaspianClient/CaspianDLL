#include "../../../Memory/Memory.hpp"
#include "HitResult.hpp"
#include "../Network/LoopbackPacketSender.hpp"
#include <unordered_map>

class PlayerListEntry {
    char padding[0x2D2];
public:
    BUILD_ACCESS(this, std::string, name, 0x18);
};

using playerList = std::unordered_map<UUID, PlayerListEntry>;

class Level {
public:
	BUILD_ACCESS(this, HitResult*, hitResult, 0xB38);
	BUILD_ACCESS(this, LoopbackPacketSender*, loopbackPacketSender, 0x0B30);
    playerList& getPlayerList() {
        return direct_access<playerList>(this, 0x1BC8);
    }
};