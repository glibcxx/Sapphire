#pragma once

#include "CommandFlag.h"
#include "CommandLexer.h"
#include "CommandPermissionLevel.h"
#include "CommandVersion.h"
#include "SDK/api/src-deps/Core/Utility/TypeId.h"
#if MC_VERSION >= v1_21_60
#    include "SDK/api/src-deps/Core/Utility/brstd/copyable_function.h"
#endif

class Actor;
class CommandOrigin;
class Packet;
class CommandParameterData;
class Command;
class CommandRunStats;

enum class CommandParameterDataType : int {
    Basic = 0,
    Enum = 1,
    SoftEnum = 2,
    Postfix = 3,
    ChainedSubcommand = 4,
};

enum class CommandStatus : int {
    Invalid = 0,
    Local = 1,
    Remote = 2,
};

enum class SemanticConstraint : uint8_t {
    None = 0,
    RequiresCheatsEnabled = 1,
    RequiresElevatedPermissions = 2,
    RequiresHostPermissions = 4,
    RequiresAllowAliases = 8,
    VALUE_MASK = 15,
};

enum class CommandParameterOption : uint8_t {
    None = 0,
    EnumAutocompleteExpansion = 1,
    HasSemanticConstraint = 2,
    EnumAsChainedCommand = 4,
};

// size: 912 (1.21.2), 920 (1.21.50), 928 (1.21.60)
class CommandRegistry {
public:
    using ValueTypeID = uint64_t;
    using EnumValueID = size_t;
    using EnumID = uint32_t;
    using SoftEnumID = uint32_t;
    using ChainedSubcommandValueID = uint64_t;
    using ChainedSubcommandID = uint32_t;
    using SemanticConstraintID = uint8_t;
    using ConstrainedValueID = uint32_t;
    using ConstrainedValueLookupKey = std::pair<ValueTypeID, ConstrainedValueID>;

    template <typename ValueType>
    struct DefaultIdConverter {
        constexpr ValueType   operator()(ValueTypeID id) const { return convert<ValueType>(id); }
        constexpr ValueTypeID operator()(const ValueType &value) const { return convert<ValueTypeID>(value); }

        template <typename To, typename From>
            requires requires(From f) {
                static_cast<To>(f);
            }
        constexpr static To convert(const From &u) { return static_cast<To>(u); }
    };

#if MC_VERSION <= v1_21_50
    using CommandOverrideFunctor = std::function<void(CommandFlag &, const std::string &)>;
#else
    using CommandOverrideFunctor = std::function<void(const std::string &, CommandFlag &, CommandPermissionLevel &)>;
#endif
    using ScoreboardScoreAccessor = std::function<int(bool &, const std::string &, const Actor &)>;
    using HardTerminal = CommandLexer::TokenType;

    enum class HardNonTerminal : int {};

    // size: 4
    class Symbol {
    public:
        int mValue = -1; // off+0
        constexpr Symbol() = default;
        constexpr Symbol(size_t value) : mValue(value) {}
        constexpr Symbol(const Symbol &other) : mValue(other.mValue) {}
        constexpr Symbol &operator=(const Symbol &other) {
            mValue = other.mValue;
            return *this;
        }
        constexpr auto operator<=>(const Symbol &) const = default;

        constexpr int value() const { return mValue; }

        constexpr size_t toIndex() const {
            return this->mValue & 0xF80FFFFF;
        }

        static const int EnumBit = 2097152;
        static const int OptionalBit = 4194304;
        static const int FactorizationBit = 8388608;
        static const int PostfixBit = 16777216;
        static const int EnumValueBit = 33554432;
        static const int SoftEnumBit = 67108864;
    };

    struct SymbolHasher {};
    struct SymbolPairHasher {
    public:
        std::size_t operator()(const std::pair<Symbol, Symbol> &) const;
    };

    using NonTerminal = Symbol;
    using Terminal = Symbol;
    using SymbolVector = std::vector<Symbol>;
    using ParseSet = entt::dense_set<Symbol, SymbolHasher>;
#if MC_VERSION <= v1_21_2
    using ParseMap = std::map<Symbol, SymbolVector>;
#else
    using ParseMap = std::map<Symbol, ParseSet>;
#endif

    // size: 72 (1.21.2/1.21.50), 128 (1.21.60)
    struct Overload {
#if MC_VERSION <= v1_21_50
        using AllocFunction = std::unique_ptr<Command> (*)();
#else
        using AllocFunction = brstd::copyable_function<std::unique_ptr<Command>()>;
#endif

        ;                                                     // off+(1.21.2/1.21.50)
        CommandVersion                    version;            // off+0
        AllocFunction                     alloc;              // off+8
        std::vector<CommandParameterData> params;             // off+16
        int                               versionOffset = -1; // off+40
        bool                              isChaining;         // off+44
        std::vector<Symbol>               paramsSymbol;       // off+48
    };

    // size: 152
    struct Signature {
        std::string            name;                     // off+0
        std::string            description;              // off+32
        std::vector<Overload>  overloads;                // off+64
        std::vector<uint32_t>  chainedSubcommandIndexes; // off+88
        CommandPermissionLevel permissionLevel;          // off+112
        Symbol                 commandSymbol;            // off+116
        Symbol                 commandAliasEnum;         // off+120
        CommandFlag            flags;                    // off+124
        int                    firstRule;                // off+128
        int                    firstFactorization;       // off+132
        int                    firstOptional;            // off+136
        bool                   runnable;                 // off+140
#if MC_VERSION == v1_21_2
        bool hasChainedSubcommands;       // off+141
        bool finalizedChainedSubcommands; // off+142
#endif
        size_t ruleCounter; // off+144
    };

    // size: 32
    struct LexicalToken {
        const char            *mText;           // off+0
        uint32_t               mLength;         // off+8
        Terminal               mType;           // off+12
        Terminal               mIdentifierInfo; // off+16
        const CommandRegistry &mRegistry;       // off+24
    };

    // size: 40
    struct ParseToken {
        std::unique_ptr<ParseToken> child;  // off+0
        std::unique_ptr<ParseToken> next;   // off+8
        ParseToken                 *parent; // off+16
        const char                 *text;   // off+24
        uint32_t                    length; // off+32
        Symbol                      type;   // off+36
    };

    using ProcessFunction = std::function<ParseToken *(ParseToken &, Symbol)>;

    // size: 104
    struct ParseRule {
        NonTerminal     nonTerminal; // off+0
        ProcessFunction process;     // off+8
        SymbolVector    derivation;  // off+72
        CommandVersion  versions;    // off+96
    };

    using RuleIndex = int;

    // size: 12
    struct OptionalParameterChain {
        int       parameterCount;     // off+0
        RuleIndex followingRuleIndex; // off+4
        Symbol    paramSymbol;        // off+8
    };

    // size: 4
    struct Factorization {
        Terminal commandSymbol; // off+0
    };

    using ParseFunction = bool (CommandRegistry::*)(
        void *, const ParseToken &, const CommandOrigin &, int, std::string &, std::vector<std::string> &
    ) const;

    // size: 72
    struct Enum {
        std::string                                      name;   // off+0
        Bedrock::typeid_t<CommandRegistry>               type;   // off+32
        ParseFunction                                    parse;  // off+40
        std::vector<std::pair<EnumValueID, ValueTypeID>> values; // off+48
    };

    // size: 32
    struct ConstrainedValue {
        Symbol                            mValue;       // off+0
        Symbol                            mEnum;        // off+4
        std::vector<SemanticConstraintID> mConstraints; // off+8
    };

    // size: 72
    struct ChainedSubcommand {
        std::string                                                           name;
        Bedrock::typeid_t<CommandRegistry>                                    type;
        ParseFunction                                                         parse;
        std::vector<std::pair<ChainedSubcommandValueID, ChainedSubcommandID>> values;
    };

    // size: 56
    struct SoftEnum {
        std::string              mName;   // off+0
        std::vector<std::string> mValues; // off+32
    };

    // size: 96 (1.21.2), 100 (1.21.50, 1.21.60)
    struct ParamSymbols {
        Terminal x;             // off+0
        Terminal y;             // off+4
        Terminal z;             // off+8
        Terminal dx;            // off+12
        Terminal dy;            // off+16
        Terminal dz;            // off+20
        Terminal r;             // off+24
        Terminal rm;            // off+28
        Terminal rx;            // off+32
        Terminal rxm;           // off+36
        Terminal ry;            // off+40
        Terminal rym;           // off+44
        Terminal l;             // off+48
        Terminal lm;            // off+52
        Terminal c;             // off+56
        Terminal m;             // off+60
        Terminal name;          // off+64
        Terminal type;          // off+68
        Terminal family;        // off+72
        Terminal score;         // off+76
        Terminal tag;           // off+80
        Terminal hasitem;       // off+84
        Terminal haspermission; // off+88
        Terminal hasproperty;   // off+92
#if MC_VERSION >= v1_21_50
        Terminal codebuilder; // off+96
#endif
    };

    // size: 128
    struct RegistryState {
        uint32_t                    signatureCount;              // off+0
        uint32_t                    enumValueCount;              // off+4
        uint32_t                    postfixCount;                // off+8
        uint32_t                    enumCount;                   // off+12
        uint32_t                    enumLookupCount;             // off+16
        uint32_t                    typeLookupCount;             // off+20
        uint32_t                    factorizationCount;          // off+24
        uint32_t                    optionalCount;               // off+28
        uint32_t                    ruleCount;                   // off+32
        uint32_t                    softEnumCount;               // off+36
        uint32_t                    constraintCount;             // off+40
        uint32_t                    chainedSubcommandCount;      // off+44
        uint32_t                    chainedSubcommandValueCount; // off+48
        std::vector<uint32_t>       enumValuesCount;             // off+56
        const std::vector<uint32_t> rainedValueCount;            // off+80
        std::vector<uint32_t>       softEnumValuesCount;         // off+104
    };

    using PredictTable = entt::dense_map<std::pair<Symbol, Symbol>, int, SymbolPairHasher>;

    // size: 112
    struct ParseTable {
        ParseMap                 first;   // off+0
        ParseMap                 follow;  // off+16
        PredictTable             predict; // off+32
        std::chrono::nanoseconds buildDuration;
    };

    // size: 112
    struct SemanticInfo {
        bool                mIsValid;                      // off+0
        std::vector<Symbol> mConstrainedParams;            // off+8
        std::string         mSoftEnumText;                 // off+32
        std::string         mSoftEnumEscapeCharExceptions; // off+64
        std::set<Symbol>    mAlreadyCompletedSymbols;      // off+96
    };

    using ParamVisitCallback = std::function<bool(
        const CommandParameterData &, const CommandOrigin &, const ParseToken *
    )>;

    using CommandVisitCallback = std::function<bool(const ParseToken *, const CommandOrigin &)>;

    std::function<void(const Packet &)> mNetworkUpdateCallback; // off+0
    ScoreboardScoreAccessor             mGetScoreForObjective;  // off+64
    bool                                mIsEduMode;             // off+128
    std::vector<ParseRule>              mRules;                 // off+136

    using ParseTableMap = std::map<uint32_t, ParseTable>;

    ParseTableMap                                           mParseTables;                  // off+160
    std::vector<OptionalParameterChain>                     mOptionals;                    // off+176
    std::vector<std::string>                                mEnumValues;                   // off+200
    std::vector<Enum>                                       mEnums;                        // off+224
    std::vector<std::string>                                mChainedSubcommandValues;      // off+248
    std::vector<ChainedSubcommand>                          mChainedSubcommands;           // off+272
    std::vector<Factorization>                              mFactorizations;               // off+296
    std::vector<std::string>                                mPostfixes;                    // off+320
    std::map<std::string, EnumID>                           mEnumLookup;                   // off+344
    std::map<std::string, EnumValueID>                      mEnumValueLookup;              // off+360
    std::map<std::string, uint32_t>                         mChainedSubcommandLookup;      // off+376
    std::map<std::string, uint64_t>                         mChainedSubcommandValueLookup; // off+392
    std::vector<Symbol>                                     mCommandSymbols;               // off+408
    std::map<std::string, Signature>                        mSignatures;                   // off+432
    std::map<Bedrock::typeid_t<CommandRegistry>, int>       mTypeLookup;                   // off+448
    std::map<std::string, std::string>                      mAliases;                      // off+464
    std::vector<SemanticConstraint>                         mSemanticConstraints;          // off+480
    std::map<SemanticConstraint, SemanticConstraintID>      mSemanticConstraintLookup;     // off+504
    std::vector<ConstrainedValue>                           mConstrainedValues;            // off+520
    std::map<ConstrainedValueLookupKey, ConstrainedValueID> mConstrainedValueLookup;       // off+544
    std::vector<SoftEnum>                                   mSoftEnums;                    // off+560
    std::map<std::string, uint32_t>                         mSoftEnumLookup;               // off+584
    std::vector<RegistryState>                              mStateStack;                   // off+600
    ParamSymbols                                            mArgs;                         // off+624
#if MC_VERSION == v1_21_2
    std::unordered_map<uint8_t, uint8_t> mSkipOnEpsAutocompleteSymbols;
    std::unordered_map<uint8_t, uint8_t> mAllowEmptySymbols;
#else
    std::unordered_set<int> mSkipOnEpsAutocompleteSymbols; // off+728
    std::unordered_set<int> mAllowEmptySymbols;            // off+792
#endif
    CommandOverrideFunctor mCommandOverrideFunctor; // off+856
#if MC_VERSION >= v1_21_60
    std::unique_ptr<CommandRunStats> mCommandRunStats; // off+920
#endif

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x8B\x54\x24\x00\x48\x83\xFA\x00\x72\x00\x48\xFF\xC2\x48\x8B\x4C\x24\x00\x48\x8B\xC1\x48\x81\xFA\x00\x00\x00\x00\x72\x00\x48\x83\xC2\x00\x48\x8B\x49\x00\x48\x2B\xC1\x48\x83\xC0\x00\x48\x83\xF8\x00\x76\x00\xFF\x15\x00\x00\x00\x00\xCC\xE8\x00\x00\x00\x00\x66\x0F\x6F\x05\x00\x00\x00\x00\xF3\x0F\x7F\x44\x24\x00\xC6\x44\x24\x00\x00\x48\x85\xFF\x75\x00\x33\xC0\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\x48\x8D\x4F\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8B\x57\x00\x48\x3B\x57\x00\x74\x00\x48\x89\x1A\x48\x89\x42\x00\x33\xC0\x48\x89\x42\x00\x48\x89\x42\x00\x48\x89\x42\x00\xC7\x42\x00\x00\x00\x00\x00\x88\x42\x00\x48\x89\x42\x00\x48\x89\x42\x00\x48\x89\x42\x00\x48\x83\x47\x00\x00\xEB\x00\x4C\x8D\x4C\x24\x00\x4C\x8D\x44\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\x5F\x00\x4C\x8D\x43\x00\x48\x8B\xD7\x48\x8B\xCE\xE8\x00\x00\x00\x00\x48\x8D\x43\x00\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\xCC\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x4C\x89\x44\x24\x00\x57\x48\x83\xEC\x00\x49\x8B\xD8\x48\x8B\xF1\x0F\x57\xC0\x0F\x11\x44\x24\x00\x0F\x57\xC9\xF3\x0F\x7F\x4C\x24\x00\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x8D\x54\x24\x00\x48\x8B\xCE\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x8B\x54\x24\x00\x48\x83\xFA\x00\x72\x00\x48\xFF\xC2\x48\x8B\x4C\x24\x00\x48\x8B\xC1\x48\x81\xFA\x00\x00\x00\x00\x72\x00\x48\x83\xC2\x00\x48\x8B\x49\x00\x48\x2B\xC1\x48\x83\xC0\x00\x48\x83\xF8\x00\x76\x00\xFF\x15\x00\x00\x00\x00\xCC\xE8\x00\x00\x00\x00\x66\x0F\x6F\x05\x00\x00\x00\x00\xF3\x0F\x7F\x44\x24\x00\xC6\x44\x24\x00\x00\x48\x85\xFF\x75\x00\x33\xC0\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\x48\x8D\x4F\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8B\x57\x00\x48\x3B\x57\x00\x74\x00\x48\x89\x1A\x48\x89\x42\x00\x33\xC0\x48\x89\x42\x00\x48\x89\x42\x00\x48\x89\x42\x00\xC7\x42\x00\x00\x00\x00\x00\x88\x42\x00\x48\x89\x42\x00\x48\x89\x42\x00\x48\x89\x42\x00\x48\x83\x47\x00\x00\xEB\x00\x4C\x8D\x4C\x24\x00\x4C\x8D\x44\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\x5F\x00\x4C\x8D\x43\x00\x48\x8B\xD7\x48\x8B\xCE\xE8\x00\x00\x00\x00\x48\x8D\x43\x00\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x00\x5F\xC3\xCC\x48\x89\x5C\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xF6\x40\x00\x00\x75\x00\x45\x32\xE4")
    SDK_API Signature *findCommand(const std::string &name);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x54\x24\x00\x48\x83\xFA\x00\x72\x00\x48\xFF\xC2\x48\x8B\x4C\x24\x00\x48\x8B\xC1\x48\x81\xFA\x00\x00\x00\x00\x72\x00\x48\x83\xC2\x00\x48\x8B\x49\x00\x48\x2B\xC1\x48\x83\xC0\x00\x48\x83\xF8\x00\x76\x00\xFF\x15\x00\x00\x00\x00\xCC\xE8\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\x4C\x8B\x44\x24\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\x4C\x24\x00\x48\x33\xCC\xE8\x00\x00\x00\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x55\x00\x48\x83\xFA\x00\x76\x00\x48\xFF\xC2\x48\x8B\x4D\x00\x48\x8B\xC1\x48\x81\xFA\x00\x00\x00\x00\x72\x00\x48\x83\xC2\x00\x48\x8B\x49\x00\x48\x2B\xC1\x48\x83\xC0\x00\x48\x83\xF8\x00\x76\x00\xFF\x15\x00\x00\x00\x00\xCC\xE8\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x48\x8B\x5D\x00\x48\x89\x5D\x00\x0F\x57\xC0\x0F\x11\x45\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\xF2\x0F\x10\x05\x00\x00\x00\x00\xF2\x0F\x11\x45\x00\x0F\xB6\x05")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x54\x24\x00\x48\x83\xFA\x00\x76\x00\x48\xFF\xC2\x48\x8B\x4C\x24\x00\x48\x8B\xC1\x48\x81\xFA\x00\x00\x00\x00\x72\x00\x48\x83\xC2\x00\x48\x8B\x49\x00\x48\x2B\xC1\x48\x83\xC0\x00\x48\x83\xF8\x00\x76\x00\xFF\x15\x00\x00\x00\x00\xCC\xE8\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8D\x1D")
    SDK_API void registerCommand(
        const std::string     &name,
        const char            *description,
        CommandPermissionLevel requirement,
        CommandFlag            f1 = {},
        CommandFlag            f2 = {}
    );

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xF0\x48\x8B\xF2\x4C\x8B\xF9\x48\x89\x4D")
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xF0\x48\x8B\xF2\x4C\x8B\xE9\x48\x89\x4D")
    SPHR_DECL_API("1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF0\x48\x8B\xF2\x4C\x8B\xE9\x48\x89\x4D")
    SDK_API void registerAlias(std::string name, std::string alias);

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\xF2\x48\x89\x55\x00\x48\x8B\xD9\x48\x89\x4C\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\xDA\x48\x89\x55\x00\x4C\x8B\xE9")
    SDK_API void registerOverloadInternal(Signature &signature, Overload &overload);

    SPHR_DECL_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xF9\x4D\x8B\xF0\x4C\x8B\xE2")
    SPHR_DECL_API("1.21.50", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xF1\x4C\x89\x45\x00\x4C\x8B\xFA")
    SPHR_DECL_API("1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xF1\x4C\x89\x44\x24\x00\x4C\x8B\xFA")
    SDK_API Symbol addEnumValuesInternal(
        const std::string                                      &name,
        const std::vector<std::pair<std::string, ValueTypeID>> &strings,
        Bedrock::typeid_t<CommandRegistry>                      type,
        CommandRegistry::ParseFunction                          parse
    );

    SPHR_DECL_API("1.21.2", "\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xE9\x49\x8B\xF0")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x89\x4D\x00\x4D\x8B\xE0\x48\x8B\xF2")
    SDK_API Symbol addEnumValuesInternal(
        const std::string                                 &name,
        const std::vector<std::pair<size_t, ValueTypeID>> &values,
        Bedrock::typeid_t<CommandRegistry>                 type,
        ParseFunction                                      parse
    );

    ValueTypeID getEnumData(const ParseToken &token) const {
        size_t index = token.child->type.toIndex();
        auto  &values = this->mEnums[token.type.toIndex()].values;
        auto   it = std::lower_bound(values.begin(), values.end(), index, [](auto &&p1, auto &&p2) {
            return p1.first < p2;
        });
        return it->second;
    }

    template <typename T, typename Converter = DefaultIdConverter<T>>
        requires std::is_enum_v<T>
    int addEnumValues(const std::string &name, const std::vector<std::pair<std::string, T>> &values) {
        std::vector<std::pair<std::string, ValueTypeID>> convertedValues;
        Converter                                        convert{};
        convertedValues.reserve(values.size());
        for (auto &&value : values) {
            convertedValues.push_back({value.first, convert(value.second)});
        }
        return this->addEnumValuesInternal(
                       name, convertedValues, type_id<CommandRegistry, T>(), &CommandRegistry::parseEnum<T, Converter>
        )
            .value();
    }

    template <typename CmdClass>
    static std::unique_ptr<Command> allocateCommand() {
        return std::make_unique<CmdClass>();
    }

    template <typename Param>
    bool parse(
        void                     *storage,
        const ParseToken         &token,
        const CommandOrigin      &origin,
        int                       version,
        std::string              &error,
        std::vector<std::string> &errorParams
    ) const {
        return false;
    }

    template <typename Param, typename Converter = DefaultIdConverter<Param>>
    bool parseEnum(
        void                     *storage,
        const ParseToken         &token,
        const CommandOrigin      &origin,
        int                       version,
        std::string              &error,
        std::vector<std::string> &errorParams
    ) const {
        Param *output = (Param *)storage;
        *output = Converter{}(this->getEnumData(token));
        return true;
    }
};

// size: 96
class CommandParameterData {
public:
    using ParseFunction = CommandRegistry::ParseFunction;

    Bedrock::typeid_t<CommandRegistry> mTypeIndex;               // off+0
    ParseFunction                      mParse;                   // off+8
    std::string                        mName;                    // off+16
    const char                        *mEnumNameOrPostfix;       // off+48
    CommandRegistry::Symbol            mEnumOrPostfixSymbol;     // off+56
    const char                        *mChainedSubcommand;       // off+64
    CommandRegistry::Symbol            mChainedSubcommandSymbol; // off+72
    CommandParameterDataType           mParamType;               // off+76
    int                                mOffset;                  // off+80
    int                                mSetOffset;               // off+84
    bool                               mIsOptional;              // off+88
    CommandParameterOption             mOptions;                 // off+89

    CommandParameterData(
        Bedrock::typeid_t<CommandRegistry>  typeIndex,
        CommandParameterData::ParseFunction parse,
        std::string                         name,
        CommandParameterDataType            paramType,
        const char                         *enumNameOrPostfix,
        const char                         *subChain,
        int                                 offset,
        bool                                optional,
        int                                 setOffset,
        CommandParameterOption              options
    ) :
        mTypeIndex(typeIndex),
        mParse(parse),
        mName(std::move(name)),
        mEnumNameOrPostfix(enumNameOrPostfix),
        mEnumOrPostfixSymbol(),
        mChainedSubcommand(subChain),
        mChainedSubcommandSymbol(),
        mParamType(paramType),
        mOffset(offset),
        mSetOffset(setOffset),
        mIsOptional(optional),
        mOptions(options) {
        assert(offset != -1 && "Data offset cannot be -1 (null of member ptr)");
    }
};

// clang-format off
template <>
SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xC0")
SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x49\x8B\xC0\x48\x8B\xDA")
SDK_API bool CommandRegistry::parse<int>(void *storage, const CommandRegistry::ParseToken &token, const CommandOrigin &origin, int version, std::string &error, std::vector<std::string> &errorParams) const;
template <>
SPHR_DECL_API("1.21.2", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x85\x00\x00\x00\x00\x0F\x57\xC0\x0F\x11\x85\x00\x00\x00\x00\x0F\x57\xC9\x66\x0F\x7F\x8D\x00\x00\x00\x00\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x89\xAD")
SPHR_DECL_API("1.21.50", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x45\x00\x0F\x57\xC0\x0F\x11\x85\x00\x00\x00\x00\x0F\x57\xC9\x66\x0F\x7F\x8D\x00\x00\x00\x00\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\xB5")
SPHR_DECL_API("1.21.60", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x45\x00\x0F\x57\xC0\x0F\x11\x45\x00\x0F\x57\xC9\x66\x0F\x7F\x4D\x00\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\x00\xE8\x00\x00\x00\x00\x4C\x89\x7D\x00\xC7\x45\x00\x00\x00\x00\x00\x4C\x89\x7D\x00\xC7\x45\x00\x00\x00\x00\x00\x44\x89\x7D\x00\xC7\x45\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x66\xC7\x45\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x0F\xB7\x08\x66\x89\x4C\x24")
SDK_API bool CommandRegistry::parse<bool>(void *storage, const CommandRegistry::ParseToken &token, const CommandOrigin &origin, int version, std::string &error, std::vector<std::string> &errorParams) const;
template <>
SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x49\x8B\xE8\x48\x8B\xFA\x48\x8B\xB4\x24")
SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x00\x49\x8B\xF8")
SDK_API bool CommandRegistry::parse<float>(void *storage, const CommandRegistry::ParseToken &token, const CommandOrigin &origin, int version, std::string &error, std::vector<std::string> &errorParams) const;
template <>
SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xDA\x48\x85\xD2")
SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xDA\x33\xFF")
SDK_API bool CommandRegistry::parse<std::string>(void *storage, const CommandRegistry::ParseToken &token, const CommandOrigin &origin, int version, std::string &error, std::vector<std::string> &errorParams) const;
template <>
SPHR_DECL_API("1.21.2", "\x48\x83\xEC\x00\x48\x8B\x84\x24\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8B\x44\x24\x00\x48\x89\x44\x24\x00\x8B\x44\x24")
SPHR_DECL_API("1.21.50,1.21.60", "\x40\x55\x53\x56\x57\x41\x56\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF1\x48\x8B\xDA")
SDK_API bool CommandRegistry::parse<class CommandSelector<class Player>>(void *storage, const CommandRegistry::ParseToken &token, const CommandOrigin &origin, int version, std::string &error, std::vector<std::string> &errorParams) const;
// clang-format on
