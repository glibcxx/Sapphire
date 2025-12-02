#include "SigDatabase.h"

#include <iostream>
#include <exception>

namespace sapphire::codegen {

    namespace fshelper {

        template <typename T, std::enable_if_t<std::is_scalar_v<T>, char> = 0>
        auto read(std::ifstream &fs) {
            T result;
            fs.read(reinterpret_cast<char *>(&result), sizeof(T));
            return result;
        }

        template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
        void write(std::ofstream &fs, T s) {
            fs.write(reinterpret_cast<char *>(&s), sizeof(T));
        }

        template <typename>
        auto read(std::ifstream &fs, size_t length);

        template <>
        auto read<std::string>(std::ifstream &fs, size_t length) {
            std::string result;
            result.resize(length);
            fs.read(result.data(), length);
            return result;
        }

        void write(std::ofstream &fs, const std::string &s) {
            fs.write(s.data(), s.size());
        }

        template <typename T, std::enable_if_t<std::is_same_v<T, SigDatabase::SigOp>, char> = 0>
        auto read(std::ifstream &fs) {
            SigDatabase::SigOp result;
            result.opType = fshelper::read<SigDatabase::SigOpType>(fs);
            switch (result.opType) {
            case SigDatabase::SigOpType::None:
            case SigDatabase::SigOpType::Deref:
            case SigDatabase::SigOpType::Call:
            case SigDatabase::SigOpType::Mov:
            case SigDatabase::SigOpType::Lea:
                break;
            case SigDatabase::SigOpType::Disp:
                result.data = fshelper::read<ptrdiff_t>(fs);
                break;
            default:
                throw std::runtime_error{"Invalid sig operation type"};
            }
            return result;
        }

        void write(std::ofstream &fs, SigDatabase::SigOp s) {
            fs.write(reinterpret_cast<char *>(&s.opType), sizeof(s.opType));
            switch (s.opType) {
            case SigDatabase::SigOpType::Disp:
                fs.write(reinterpret_cast<char *>(&s.data), sizeof(s.data));
            default:
                break;
            }
        }

    } // namespace fshelper

    bool SigDatabase::load(std::ifstream &fs) {
        try {
            auto magicNum = fshelper::read<uint32_t>(fs);
            if (magicNum != SigDatabase::MAGIC_NUMBER)
                return false;
            mFormatVersion = fshelper::read<FormatVersion>(fs);
            mSupportVersion = fshelper::read<uint64_t>(fs);
            auto sigCount = fshelper::read<size_t>(fs);
            if (!sigCount) return false;
            mSigEntries.reserve(sigCount);
            for (size_t i = 0; i < sigCount; ++i) {
                SigEntry sigEntry;
                sigEntry.mSymbol = fshelper::read<std::string>(fs, fshelper::read<size_t>(fs));
                sigEntry.mSig = fshelper::read<std::string>(fs, fshelper::read<size_t>(fs));
                size_t sigOpCount = fshelper::read<size_t>(fs);
                if (sigOpCount) {
                    sigEntry.mOperations.reserve(sigOpCount);
                    for (size_t j = 0; j < sigOpCount; ++j) {
                        sigEntry.mOperations.emplace_back(fshelper::read<SigOp>(fs));
                    }
                }
                mSigEntries.emplace_back(std::move(sigEntry));
            }
            return true;
        } catch (std::exception &e) {
            std::cerr << "[Error] error while loadding sig file, msg: " << e.what() << '\n';
        } catch (...) {
            std::cerr << "[Error] unknown error while loadding sig file\n";
        }
        return false;
    }

    bool SigDatabase::save(std::ofstream &fs) const {
        try {
            fshelper::write(fs, SigDatabase::MAGIC_NUMBER);
            fshelper::write(fs, mFormatVersion);
            fshelper::write(fs, mSupportVersion);
            fshelper::write(fs, mSigEntries.size());
            for (auto &&it : mSigEntries) {
                fshelper::write(fs, it.mSymbol.size());
                fshelper::write(fs, it.mSymbol);
                fshelper::write(fs, it.mSig.size());
                fshelper::write(fs, it.mSig);
                fshelper::write(fs, it.mOperations.size());
                for (auto &&op : it.mOperations) {
                    fshelper::write(fs, op);
                }
            }
            return true;
        } catch (std::exception &e) {
            std::cerr << "[Error] error while saving sig file, msg: " << e.what() << '\n';
        } catch (...) {
            std::cerr << "[Error] unknown error while saving sig file\n";
        }
        return false;
    }

    std::string formatSig(const std::string &sig) {
        if (sig.empty()) return {};
        std::string result;
        result.reserve(sig.size() * 3 - 1);
        constexpr char hex[] = "0123456789ABCDEF";
        for (unsigned char c : sig) {
            if (!result.empty()) result += " ";
            if (c == 0x00)
                result += "??";
            else {
                result += hex[(c >> 4) & 0xF];
                result += hex[c & 0xF];
            }
        }
        return result;
    }

    void SigDatabase::dump() const {
        std::cout << "mFormatVersion=" << (int32_t)mFormatVersion << '\n';
        std::cout << "mSupportVersion=" << mSupportVersion << '\n';
        std::cout << "SigEntryCount=" << mSigEntries.size() << '\n';
        for (auto &&it : mSigEntries) {
            std::cout << "  mSymbol=" << it.mSymbol << '\n';
            std::cout << "  mSig=" << formatSig(it.mSig) << '\n';
            std::cout << "  mOperations=\n";
            for (auto &&op : it.mOperations) {
                std::cout << "    opType=" << (int32_t)op.opType << ", data=" << op.data << '\n';
            }
            std::cout << "---\n";
        }
    }

} // namespace sapphire::codegen