#pragma once

#include "SDK/api/src-deps/Core/Utility/UUID.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/actor/player/Player.h"
#include "SDK/api/src/common/world/level/GameType.h"
#include "SDK/api/src/common/entity/components/MoveInputComponent.h"

class IClientInstance;
class Level;
class NetworkIdentifier;
class Certificate;

class LocalPlayer : public Player {
public:
    SDK_API LocalPlayer(
        IClientInstance             &client,
        Level                       &level,
        const std::string           &displayName,
        GameType                     playerGameType,
        bool                         isHostingPlayer,
        const NetworkIdentifier     &owner,
        SubClientId                  subid,
        mce::UUID                    uuid,
        const std::string           &playFabId,
        const std::string           &deviceId,
        std::unique_ptr<Certificate> certificate,
        EntityContext               &entityContext
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                                                                                                                                                    \
    "??0LocalPlayer@@QEAA@AEAVIClientInstance@@AEAVLevel@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4GameType@@_NAEBVNetworkIdentifier@@EVUUID@mce@@22V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@4@AEAVEntityContext@@@Z",        \
    "?ctor@LocalPlayer@@QEAAPEAV1@AEAVIClientInstance@@AEAVLevel@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4GameType@@_NAEBVNetworkIdentifier@@EVUUID@mce@@22V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@5@AEAVEntityContext@@@Z" \
)

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x89\x33\x48\x8B\xC3\x48\x81\xC4\x00\x00\x00\x00\x41\x5F\x41\x5E\x41\x5D")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x3B\x48\x8B\xC3\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x81\xC4\x00\x00\x00\x00\x41\x5F\x41\x5E\x41\x5D\x41\x5C\x5F\x5E\x5D\xC3")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xD8\x33\xFF\xEB\x00\x33\xFF\x8B\xDF\x48\x89\x5D")
    SDK_API LocalPlayer *ctor(
        IClientInstance             &client,
        Level                       &level,
        const std::string           &displayName,
        GameType                     playerGameType,
        bool                         isHostingPlayer,
        const NetworkIdentifier     &owner,
        SubClientId                  subid,
        mce::UUID                    uuid,
        const std::string           &playFabId,
        const std::string           &deviceId,
        std::unique_ptr<Certificate> certificate,
        EntityContext               &entityContext
    );

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x85\xF6\x74\x00\x48\x8B\xCE\xE8\x00\x00\x00\x00\x0F\x28\xCE")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8B\x06\x0F\x28\xCE")
    SDK_API void localPlayerTurn(const Vec2 &deltaRot);

    MoveInputComponent *getMoveInputComponent() {
        return tryGetComponent<MoveInputComponent>();
    }

    const MoveInputComponent *getMoveInputComponent() const {
        return tryGetComponent<MoveInputComponent>();
    }
};