#pragma once

#include "../Mob.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src/common/world/level/ChunkPos.h"
#include "SDK/api/src/common/world/level/GameType.h"
#include "SDK/api/src/common/world/level/Tick.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"

class ChunkSource;
class ChunkViewSource;
class FrameUpdateContextBase;
class BlockActor;
class ChalkboardBlockActor;
class INpcDialogueData;
class TextObjectRoot;
class ResolvedTextObject;
class IMinecraftEventing;
class IContainerManager;
class Container;
class InventoryTransaction;
class ComplexInventoryTransaction;
class Packet;
class PlayerEventCoordinator;
class LayeredAbilities;
namespace Editor {
    class IEditorPlayer;
}

enum class BedSleepingResult : int {
    OK = 0,
    NOT_POSSIBLE_HERE = 1,
    NOT_POSSIBLE_NOW = 2,
    TOO_FAR_AWAY = 3,
    OTHER_PROBLEM = 4,
    NOT_SAFE = 5,
    BED_OBSTRUCTED = 6,
};

class Player : public Mob {
public:
    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x4C\x8B\xF1\x48\x89\x01\x48\x8D\x54\x24")
    SDK_API static const uintptr_t __vftable[];

    virtual void prepareRegion(ChunkSource &mainChunkSource);

    virtual void destroyRegion();

    virtual void suspendRegion();

    virtual void _fireDimensionChanged();

    virtual void changeDimensionWithCredits(DimensionType dimension);

    virtual void tickWorld(const Tick &currentTick);

    virtual void frameUpdate(FrameUpdateContextBase &frameUpdateContextBase) = 0;

    virtual const std::vector<ChunkPos> &getTickingOffsets() const;

    virtual void moveView();

    virtual void moveSpawnView(const Vec3 &spawnPosition, DimensionType dimensionType);

    virtual void checkMovementStats(const Vec3 &d);

    virtual HashedString getCurrentStructureFeature() const;

    virtual bool isAutoJumpEnabled();

    virtual void respawn();

    virtual void resetRot();

    virtual bool isInTrialMode();

    virtual void openPortfolio();

    virtual void openBook(int bookSlot, bool editable, int page, BlockActor *lectern);

    virtual void openTrading(const ActorUniqueID &uniqueID, bool useNewScreen);

    virtual void openChalkboard(ChalkboardBlockActor &showLockToggle, bool useNewScreen);

    virtual void openNpcInteractScreen(std::shared_ptr<INpcDialogueData> npc);

    virtual void openInventory();

    virtual void displayChatMessage(
        const std::string &author,
        const std::string &message,
#if MC_VERSION >= v1_21_50
        const std::optional<std::string> filteredMessage
#endif
    );

    virtual void displayClientMessage(const std::string &message, const std::optional<std::string> filteredMessage);

    virtual void displayTextObjectMessage(
        const TextObjectRoot &textObject, const std::string &fromXuid, const std::string &fromPlatformId
    );

    virtual void displayTextObjectWhisperMessage(
        const std::string &message, const std::string &xuid, const std::string &platformId
    );

    virtual void displayTextObjectWhisperMessage(
        const ResolvedTextObject &resolvedTextObject, const std::string &xuid, const std::string &platformId
    );

    virtual void displayWhisperMessage(
        const std::string &author,
        const std::string &message,
#if MC_VERSION >= v1_21_50
        const std::optional<std::string> filteredMessage,
#endif
        const std::string &xuid,
        const std::string &platformId
    );

    virtual BedSleepingResult startSleepInBed(const BlockPos &bedBlockPos);

    virtual void stopSleepInBed(bool forcefulWakeUp, bool updateLevelList);

    virtual bool canStartSleepInBed();

    virtual void openSign(const BlockPos &pos, bool);

    virtual void playEmote(const std::string &pieceId, bool);

    virtual bool isLoading() const;

    virtual bool isPlayerInitialized() const;

    virtual void stopLoading();

    virtual void setPlayerGameType(GameType gameType);

    virtual void initHUDContainerManager();

    virtual void _crit(Actor &actor);

    virtual IMinecraftEventing *getEventing() const;

    virtual std::string getUserId();

    virtual void addExperience(int xp);

    virtual void addLevels(int levels);

    virtual void setContainerData(IContainerManager &menu, int id, int value) = 0;

    virtual void slotChanged(
        IContainerManager &menu,
        Container         &container,
        int                slot,
        const ItemStack   &oldItem,
        const ItemStack   &newItem,
        bool               isResultSlot
    ) = 0;

    virtual void refreshContainer(IContainerManager &menu) = 0;

    virtual void deleteContainerManager();

    virtual bool isActorRelevant(const Actor &);

    virtual bool isTeacher() const = 0;

    virtual void onSuspension();

    virtual void onLinkedSlotsChanged();

    virtual void sendInventoryTransaction(const InventoryTransaction &transaction) const = 0;

    virtual void sendComplexInventoryTransaction(std::unique_ptr<ComplexInventoryTransaction> transaction) const = 0;

    virtual void sendNetworkPacket(Packet &packet) const;

    virtual PlayerEventCoordinator &getPlayerEventCoordinator() = 0;

    virtual bool isSimulated();

    virtual std::string getXuid() const;

    virtual const PlayerMovementSettings &getMovementSettings() const;

    virtual void requestMissingSubChunk(const SubChunkPos &pos);

    virtual uint8_t getMaxChunkBuildRadius() const;

    virtual void onMovePlayerPacketNormal(const Vec3 &pos, const Vec2 &rot, float yHeadRot);

    virtual std::shared_ptr<ChunkViewSource> _createChunkSource(ChunkSource &mainChunkSource);

    virtual void setAbilities(const LayeredAbilities &abilities);

    virtual Bedrock::NonOwnerPointer<Editor::IEditorPlayer> getEditorPlayer() const = 0;

    virtual void destroyEditorPlayer() = 0;
};