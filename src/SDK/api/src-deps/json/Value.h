#pragma once

#include <map>
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

    // size; 16
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

        class CZString {
        public:
            enum DuplicationPolicy {
                noDuplication = 0,
                duplicate,
                duplicateOnCopy
            };

            const char *cstr_;
            ArrayIndex  index_;

            bool operator<(const CZString &other) const {
                if (cstr_) return strcmp(cstr_, other.cstr_) < 0;
                return index_ < other.index_;
            }

            bool operator==(const CZString &other) const {
                if (cstr_) return strcmp(cstr_, other.cstr_) == 0;
                return index_ == other.index_;
            }
        };

        using ObjectValues = std::map<CZString, Json::Value>;
        using ArrayValues = std::vector<Json::Value *>;

        union ValueHolder {
            Int64         int_;
            UInt64        uint_;
            double        real_;
            bool          bool_;
            char         *string_;
            ArrayValues  *array_;
            ObjectValues *map_;
        };

        ValueHolder value_; // off+0
        ValueType   type_;  // off+8

        static const Value null;
    };
    static_assert(sizeof(Value) == 16);

    inline const Value Value::null{.type_ = ValueType::nullValue};

} // namespace Json
