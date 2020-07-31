#include <Messages/NotifyFactionsChanges.h>
#include <Serialization.hpp>
#include <cassert>

void NotifyFactionsChanges::SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept
{
    assert(Changes.size() < 0x100);

    aWriter.WriteBits(Changes.size() & 0xFF, 8);

    for (auto& change : Changes)
    {
        Serialization::WriteVarInt(aWriter, change.first);
        change.second.Serialize(aWriter);
    }
}

void NotifyFactionsChanges::DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept
{
    ServerMessage::DeserializeRaw(aReader);

    uint64_t count = 0;
    aReader.ReadBits(count, 8);

    for (auto i = 0u; i < count; ++i)
    {
        auto& change = Changes[Serialization::ReadVarInt(aReader) & 0xFFFFFFFF];
        change.Deserialize(aReader);
    }
}
