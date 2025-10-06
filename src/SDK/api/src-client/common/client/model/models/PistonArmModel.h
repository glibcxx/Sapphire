#pragma once

#include "Model.h"
#include "SDK/api/src-client/common/client/model/geom/ModelPart.h"

// size: 2936 (1.21.2), 2720 (1.21.50), 2712 (1.21.60)
class PistonArmModel : public Model {
public:
    ;                                  // off+(1.21.50)
    mce::MaterialPtr mDefaultMaterial; // off+280
    ModelPart        mArmBase;         // off+296
    ModelPart        mArm;             // off+1104
    ModelPart        mFront;           // off+1912

    // vtb+0
    virtual ~PistonArmModel();
};