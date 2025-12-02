#pragma once

#include <fstream>
#include <vector>

namespace sapphire::codegen {

    class SigDatabase {
    public:
        static constexpr uint32_t MAGIC_NUMBER = 0X3046FCDB; // crc32(".sig.db")

        enum class FormatVersion : int32_t {
            v1_0_0
        };

        enum class SigOpType : int32_t {
            None = 0,
            Disp = 1,
            Deref = 2,
            Call = 3,
            Mov = 4,
            Lea = 5,
            _invalid = -1,
        };

        struct SigOp {
            SigOpType opType;
            ptrdiff_t data;
        };

        struct SigEntry {
            enum class Type : int8_t {
                Function,
                Data,
                VirtualThunk,
                CtorThunk,
                DtorThunk,
                _invalid = -1,
            };
            Type        mType = Type::Function;
            std::string mSymbol;
            std::string mExtraSymbol; // for Thunk
            std::string mSig;

            std::vector<SigOp> mOperations;

            constexpr bool hasExtraSymbol() const {
                return mType == Type::VirtualThunk || mType == Type::CtorThunk || mType == Type::DtorThunk;
            }
        };

        SigDatabase(uint64_t supportVersion, FormatVersion fmtVer = FormatVersion::v1_0_0) :
            mFormatVersion(fmtVer), mSupportVersion(supportVersion) {}

        bool load(std::ifstream &fs);

        bool save(std::ofstream &fs) const;

        void dump() const;

        void addSigEntry(SigEntry &&sig) {
            mSigEntries.emplace_back(sig);
        }

        size_t        size() const { return mSigEntries.size(); }
        FormatVersion formatVersion() const { return mFormatVersion; }
        uint64_t      supportVersion() const { return mSupportVersion; }

        const std::vector<SigEntry> getSigEntries() const { return mSigEntries; }

    private:
        FormatVersion         mFormatVersion;
        uint64_t              mSupportVersion;
        std::vector<SigEntry> mSigEntries;
    };

} // namespace sapphire::codegen