#pragma once

#include <nlohmann/json.hpp>

namespace sapphire::ipc {

    namespace status {
        constexpr std::string_view Handshake = "handshake";
        constexpr std::string_view Success = "success";
        constexpr std::string_view Error = "error";
        constexpr std::string_view Handoff = "handoff";
    } // namespace status

    class Message {
        nlohmann::json mMsg;

    public:
        Message() = default;

        Message(std::string_view status, const nlohmann::json &data = {
        }) :
            mMsg{{"status", status}, {"data", data}} {}

        static Message parse(const std::string &raw) {
            Message msg;
            msg.mMsg = nlohmann::json::parse(raw);
            return msg;
        }

        std::string dump() const {
            return mMsg.dump();
        }

        std::string_view getStatus() const {
            return mMsg["status"];
        }

        const nlohmann::json &getData() const {
            return mMsg["data"];
        }
    };

} // namespace sapphire::ipc