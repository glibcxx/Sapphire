#pragma once

#include "CommandPropertyBag.h"

enum class CommandOutputType : int {
    None = 0,
    LastOutput = 1,
    Silent = 2,
    AllOutput = 3,
    DataSet = 4,
};

// size: 40
class CommandOutputParameter {
public:
    enum NoCountType {
        NoCount = 0,
    };

    std::string mString; // off+0
    int         mCount;  // off+32

    const std::string &getText() const { return this->mString; }

    int count() const { return this->mCount; }
};

enum class CommandOutputMessageType : int {
    Success = 0,
    Error = 1,
};

// size: 64
class CommandOutputMessage {
public:
    CommandOutputMessageType mType;      // off+0
    std::string              mMessageId; // off+8
    std::vector<std::string> mParams;    // off+40
};

// size: 48
class CommandOutput {
public:
    CommandOutputType                   mType;          // off+0
    std::unique_ptr<CommandPropertyBag> mBag;           // off+8
    std::vector<CommandOutputMessage>   mMessages;      // off+16
    int                                 mSuccessCount;  // off+40
    bool                                mHasPlayerText; // off+44

    CommandOutput(CommandOutputType type) :
        mType(type) {
        if (this->mType == CommandOutputType::DataSet) {
            this->mBag = std::make_unique<CommandPropertyBag>();
        }
    }

    CommandOutput(const CommandOutput &rhs) :
        mType(rhs.mType),
        mBag(rhs.mBag ? rhs.mBag->clone() : nullptr),
        mMessages(rhs.mMessages),
        mSuccessCount(rhs.mSuccessCount) {}

    void error(const std::string &msgId, const std::vector<CommandOutputParameter> &params = {}) {
        if (this->mType != CommandOutputType::None)
            this->addMessage(msgId, params, CommandOutputMessageType::Error);
    }

    void forceOutput(const std::string &msgId, const std::vector<CommandOutputParameter> &params = {}) {
        if (this->mType != CommandOutputType::None)
            this->addMessage(msgId, params, CommandOutputMessageType::Success);
    }

    void success(const std::string &msgId, const std::vector<CommandOutputParameter> &params = {}) {
        int successCount = 0;
        for (auto &&param : params) {
            successCount += param.count();
        }
        if (successCount != -1)
            this->mSuccessCount += std::max(successCount, 1);
        if (this->mType != CommandOutputType::None && this->mType != CommandOutputType::Silent)
            this->addMessage(msgId, params, CommandOutputMessageType::Success);
    }

    void success() {
        ++this->mSuccessCount;
    }

    int getSuccessCount() const {
        return this->mSuccessCount;
    }

    bool wantsData() const {
        return this->mType == CommandOutputType::DataSet;
    }

    bool empty() const {
        return this->mMessages.empty() && !this->wantsData();
    }

    void load(
        CommandOutputType                     type,
        int                                   successCount,
        std::vector<CommandOutputMessage>   &&messages,
        std::unique_ptr<CommandPropertyBag> &&data
    ) {
        this->mType = type;
        this->mSuccessCount = successCount;
        this->mMessages.swap(messages);
        this->mBag = std::move(data);
    }

    const CommandPropertyBag &getData() const {
        return *this->mBag;
    }

    CommandOutputType getType() const {
        return this->mType;
    }

    const std::vector<CommandOutputMessage> &getMessages() const {
        return this->mMessages;
    }

    void addMessage(
        const std::string                         &msgId,
        const std::vector<CommandOutputParameter> &params,
        CommandOutputMessageType                   type
    ) {
        if (this->mType == CommandOutputType::LastOutput)
            this->mMessages.clear();
        this->mMessages.emplace_back(type, msgId, getMessageParams(params));
    }

    std::vector<std::string> getMessageParams(const std::vector<CommandOutputParameter> &params) {
        std::vector<std::string> strings;
        strings.reserve(params.size());
        for (auto &&param : params) {
            if (param.count() != -1)
                strings.push_back(param.getText());
        }
        return strings;
    }
};