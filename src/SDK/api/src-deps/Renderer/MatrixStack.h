#pragma once

#include <stack>
#include "Matrix.h"

// size: 64
class MatrixStack {
public:
    struct MatrixStackRef {
        MatrixStack *stack;
        Matrix      *mat;
    };
    std::stack<Matrix> stack;    // off+0
    MatrixStackRef     stackRef; // off+40
    bool               _isDirty; // off+56
};
static_assert(sizeof(MatrixStack) == 64);
