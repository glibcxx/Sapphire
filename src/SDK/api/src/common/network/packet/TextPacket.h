#pragma once

#include "../Packet.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"

enum class TextPacketType : byte {
    Raw = 0,
    Chat = 1,
    Translate = 2,
    Popup = 3,
    JukeboxPopup = 4,
    Tip = 5,
    SystemMessage = 6,
    Whisper = 7,
    Announcement = 8,
    TextObjectWhisper = 9,
    TextObject = 10,
    TextObjectAnnouncement = 11,
};

// size: 256
class TextPacket : public Packet {
public:
    TextPacketType             mType;            // off+48
    std::string                mAuthor;          // off+56
    std::string                mMessage;         // off+88
    std::optional<std::string> mFilteredMessage; // off+120
    std::vector<std::string>   params;           // off+160
    bool                       mLocalize;        // off+184
    std::string                mXuid;            // off+192
    std::string                mPlatformId;      // off+224

    // vtb+0
    virtual ~TextPacket() = default;

    // vtb+2
    virtual std::string getName() const override;

    // vtb+4
    virtual void write(BinaryStream &stream) const override;

    // vtb+8
    virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) override;

    SDK_API static TextPacket createRaw(const std::string &raw);
};
static_assert(sizeof(TextPacket) == 256);