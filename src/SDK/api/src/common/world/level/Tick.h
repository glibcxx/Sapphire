#pragma once

// size: 8
struct Tick {
    Tick(uint64_t v) :
        tickID(v) {}

    Tick() :
        tickID(0) {}

    Tick(const Tick &t) :
        tickID(t.tickID) {}

    const Tick &operator=(const Tick &t) {
        this->tickID = t.tickID;
        return *this;
    }

    const Tick &operator=(const uint64_t &t) {
        this->tickID = t;
        return *this;
    }

    uint64_t getTimeStamp() const {
        return this->tickID;
    }

    bool operator!=(const Tick &other) const {
        return this->tickID != other.tickID;
    }

    bool operator<(const Tick &other) const {
        return this->tickID < other.tickID;
    }

    void operator++() {
        ++this->tickID;
    }

    Tick operator+(int o) const {
        return Tick(this->tickID + o);
    }

    bool operator>(const Tick &other) const {
        return this->tickID > other.tickID;
    }

    bool operator>=(const Tick &other) const {
        return this->tickID >= other.tickID;
    }

    bool operator<=(const Tick &other) const {
        return this->tickID <= other.tickID;
    }

    bool operator==(const Tick &other) const {
        return this->tickID == other.tickID;
    }

    uint64_t operator%(int mod) const {
        return this->tickID % mod;
    }

    uint64_t tickID; // off+0
};