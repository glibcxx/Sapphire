#pragma once

template <typename T>
class buffer_span_mut {
public:
    T *mBegin; // off+0
    T *mEnd;   // off+8
};

template <typename T>
class buffer_span {
public:
    const T *mBegin; // off+0
    const T *mEnd;   // off+8
};