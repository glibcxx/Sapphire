#pragma once

// size: 296 (1.21.2), 456 (1.21.50), 280 (1.21.60)
class PackDownloadManager {
public:
#if MC_VERSION == v1_21_2
    __int64 mUnk0[13][2];
#elif MC_VERSION == v1_21_50
    // size: 168 (1.21.50)
    class Unk0 {
    public:
        struct Unk8 {
            __int64     mUnk0;   // off+0
            __int64     mUnk8;   // off+8
            int         mUnk16;  // off+16
            std::string mUnk24;  // off+24
            std::string mUnk56;  // off+56
            std::string mUnk88;  // off+88
            short       mUnk120; // off+120
        };

        short                mUnk0;   // off+0
        bool                 mUnk1;   // off+1
        Unk8                 mUnk8;   // off+8
        bool                 mUnk136; // off+136
        std::vector<__int64> mUnk144; // off+144
    };

    Unk0 mUnk0[2]; // off+0
#elif MC_VERSION == v1_21_60
    __int64 mUnk0[10][2];
#endif
    std::vector<__int64> mUnk336; // off+336
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    __int64 mUnk360[4]; // off+360
#endif
    std::unordered_map<std::string, __int64> mUnk392; // off+392
};