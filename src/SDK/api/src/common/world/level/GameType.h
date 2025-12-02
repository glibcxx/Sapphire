#pragma once

enum class GameType : int {
    Undefined = -1,
    Survival = 0,
    Creative = 1,
    Adventure = 2,
    SurvivalViewer = 3,
    CreativeViewer = 4,
    Default = 5,
    Spectator = 6,
    WorldDefault = 0,
};