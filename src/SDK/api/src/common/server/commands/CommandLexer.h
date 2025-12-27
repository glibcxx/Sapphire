#pragma once

// size: 24
class CommandLexer {
public:
    enum class TokenType : int {};

    // size: 16
    struct Token {
        const char *text;   // off+0
        uint32_t    length; // off+8
        TokenType   type;   // off+12
    };

    const std::string &mInput; // off+0
    Token              mToken; // off+8
};