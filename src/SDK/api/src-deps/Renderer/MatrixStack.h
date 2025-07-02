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

    MatrixStackRef push() {
        this->_isDirty = true;
        Matrix &m = this->stack.emplace(Matrix{glm::mat4{1.0f}});
        return MatrixStackRef{this, &m};
    }

    MatrixStackRef push(const Matrix &matrix) {
        this->_isDirty = true;
        Matrix &m = this->stack.emplace(matrix);
        return MatrixStackRef{this, &m};
    }

    void pop() {
        this->_isDirty = true;
        this->stack.pop();
    }

    bool isDirty() const {
        return this->_isDirty;
    }
};
static_assert(sizeof(MatrixStack) == 64);
