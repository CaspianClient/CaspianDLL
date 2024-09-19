#include "../../../Memory/Memory.hpp"
#include "Packet.hpp"
#include "Packets/TextPacket.hpp"

class LoopbackPacketSender {
public:
	virtual void send(Packet *packet);
    virtual void sendToServer(Packet *packet);
};