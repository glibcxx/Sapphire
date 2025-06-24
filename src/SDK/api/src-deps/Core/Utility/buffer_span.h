#pragma once

template <typename T>
class buffer_span_mut
{
public:
    T *mBegin; // off+0
    T *mEnd;   // off+8
};