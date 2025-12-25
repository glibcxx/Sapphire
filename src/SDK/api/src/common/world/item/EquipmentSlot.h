#pragma once

namespace SharedTypes::Legacy {

    enum class EquipmentSlot : int {
        None = -1,
        Begin = 0,
        HandSlot = 0,
        Mainhand = 0,
        Offhand = 1,
        ArmorSlot = 2,
        Head = 2,
        Torso = 3,
        Legs = 4,
        Feet = 5,
        Body = 6,
        ContainerSlot = 7,
        Hotbar = 7,
        Inventory = 8,
        EnderChest = 9,
        Saddle = 10,
        EntityArmor = 11,
        Chest = 12,
        Equippable = 13,
        Count = 14,
    };

}

enum class HandSlot : int {
    Mainhand = 0,
    Offhand = 1,
    _count = 2,
};

enum class ArmorSlot : int {
    Head = 0,
    Torso = 1,
    Legs = 2,
    Feet = 3,
    Body = 4,
    _humanoidCount = 4,
    _count = 5,
};
