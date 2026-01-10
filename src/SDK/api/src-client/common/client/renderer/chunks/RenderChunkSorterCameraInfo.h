#pragma once

// size: 28
class RenderChunkSorterSharedInfo {
public:
    std::atomic<float> x;                    // off+0
    std::atomic<float> y;                    // off+4
    std::atomic<float> z;                    // off+8
    std::atomic<float> dx;                   // off+12
    std::atomic<float> dy;                   // off+16
    std::atomic<float> dz;                   // off+20
    std::atomic<bool>  cameraIsOrthographic; // off+24
};