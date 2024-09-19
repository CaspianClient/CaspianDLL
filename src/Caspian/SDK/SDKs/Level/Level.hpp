#include "../../../Memory/Memory.hpp"
#include "HitResult.hpp"
#include "../Network/LoopbackPacketSender.hpp"

class Level {
public:
	BUILD_ACCESS(this, HitResult*, hitResult, 0xB38);
	BUILD_ACCESS(this, LoopbackPacketSender*, loopbackPacketSender, 0x0B30);
};