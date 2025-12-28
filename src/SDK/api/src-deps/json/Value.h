#pragma once

#include <map>
#include <utility>
#include <vector>
#include <string>

namespace Json {

    class ValueIterator;
    class ValueConstIterator;

    enum ValueType {
        nullValue = 0, ///< 'null' value
        intValue,      ///< signed integer value
        uintValue,     ///< unsigned integer value
        realValue,     ///< double value
        stringValue,   ///< UTF-8 string value
        booleanValue,  ///< bool value
        arrayValue,    ///< array value (ordered list)
        objectValue    ///< object value (collection of name/value pairs).
    };

    // size: 16
    class Value {
    public:
        using Members = std::vector<std::string>;
        using iterator = Json::ValueIterator;
        using const_iterator = Json::ValueConstIterator;
        using UInt = uint32_t;
        using Int = int32_t;
        using UInt64 = uint64_t;
        using Int64 = int64_t;
        using LargestInt = int64_t;
        using LargestUInt = uint64_t;
        using ArrayIndex = uint32_t;

        // size: 8
        class CZString {
        public:
            const char *cstr_; // off+0

            CZString(const char *cstr) :
                cstr_(duplicateStringValue(cstr)) {}

            CZString(const CZString &other) :
                cstr_(duplicateStringValue(other.cstr_)) {}

            ~CZString() {
                if (cstr_)
                    free(const_cast<char *>(cstr_));
            }

            bool operator<(const CZString &other) const {
                if (cstr_) return strcmp(cstr_, other.cstr_) < 0;
                return false;
            }

            bool operator==(const CZString &other) const {
                return !cstr_ && other.cstr_ || strcmp(cstr_, other.cstr_) == 0;
            }

            static char *duplicateStringValue(const char *value, unsigned int length = -1) {
                if (length == -1)
                    length = (unsigned int)strlen(value);
                char *newString = static_cast<char *>(malloc(length + 1));
                memcpy(newString, value, length);
                newString[length] = 0;
                return newString;
            }
        };

        using ObjectValues = std::map<CZString, Json::Value>;
        using ArrayValues = std::vector<Json::Value *>;

        union ValueHolder {
            Int64         int_;
            UInt64        uint_;
            double        real_;
            bool          bool_;
            CZString     *string_;
            ArrayValues  *array_;
            ObjectValues *map_;
        };

        ValueHolder value_; // off+0
        ValueType   type_;  // off+8

        SDK_API Value(Json::ValueType type = nullValue);
#pragma SDK_LINKER_SYM_ALIAS("??0Value@Json@@QEAA@W4ValueType@1@@Z", "?ctor@Value@Json@@QEAAPEAV12@W4ValueType@2@@Z")

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\x4C\x8B\x43\x00\x49\x8B\x10\x4C\x8D\x8C\x24")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x66\x0F\x6F\x0D")
        SDK_API Value *ctor(Json::ValueType type = nullValue);

        SDK_API Value(const Value &other);
#pragma SDK_LINKER_SYM_ALIAS("??0Value@Json@@QEAA@AEBV01@@Z", "?ctor@Value@Json@@QEAAPEAV12@AEBV12@@Z")

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x0F\xB6\x4F\x00\x0F\xB6\x44\x24\x00\x88\x47\x00\x48\x8B\x44\x24\x00\x88\x4C\x24\x00\x48\x8B\x4F\x00\x48\x89\x4C\x24\x00\x48\x8D\x4C\x24\x00\x48\x89\x47\x00\xE8\x00\x00\x00\x00\x48\x8B\x5C\x24")
        SDK_API Value *ctor(const Value &other);

        Value(Value &&other) noexcept :
            value_(other.value_), type_(std::exchange(other.type_, nullValue)) {}

        ~Value() {
            switch (type_) {
            case nullValue:
            case intValue:
            case uintValue:
            case realValue:
            case booleanValue:
                break;
            case stringValue:
                if (value_.string_)
                    delete value_.string_;
                break;
            case arrayValue:
                delete value_.array_;
            case objectValue:
                delete value_.map_;
                break;
            default:
                break;
            }
        }

        static const Value null;
    };
    static_assert(sizeof(Value) == 16);

    inline const Value Value::null{};

} // namespace Json
