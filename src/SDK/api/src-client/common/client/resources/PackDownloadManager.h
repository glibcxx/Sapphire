#pragma once

// size: 432 (1.21.50)
class PackDownloadManager {
public:
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

    Unk0                                     mUnk0[2];   // off+0
    std::vector<__int64>                     mUnk336;    // off+36
    __int64                                  mUnk360[4]; // off+360
    std::unordered_map<std::string, __int64> mUnk392;    // off+392
};