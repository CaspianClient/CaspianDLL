#include "../Packet.hpp"

enum class TextPacketType {
    RAW = 0,
    CHAT = 1,
    TRANSLATION = 2,
    POPUP = 3,
    JUKEBOX_POPUP = 4,
    TIP = 5,
    SYSTEM = 6,
    WHISPER = 7,
    ANNOUNCEMENT = 8,
    JSON_WHISPER = 9,
    JSON = 10,
    JSON_ANNOUNCEMENT = 11
};

class TextPacket : public ::Packet {
public:
    TextPacketType           mType;            // this+0x30
    std::string              mAuthor;          // this+0x38
    std::string              mMessage;         // this+0x58
    std::string              mFilteredMessage; // this+0x78
    bool                     mUnknown;         // this+0x98
    std::vector<std::string> mParams;          // this+0xA0
    bool                     mLocalize;        // this+0x88
    std::string              mXuid;            // this+0xC0
    std::string              mPlatformId;      // this+0xE0
};