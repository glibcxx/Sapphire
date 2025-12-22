#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <compare>
#include <regex>

namespace sapphire {

    class SemanticVersion {
    public:
        int         mMajor = 0;
        int         mMinor = 0;
        int         mPatch = 0;
        std::string mPreRelease;
        std::string mBuildMetadata;

    public:
        constexpr SemanticVersion(
            int major = 0, int minor = 0, int patch = 0, std::string pre_release = "", std::string build_metadata = ""
        ) : mMajor(major),
            mMinor(minor),
            mPatch(patch),
            mPreRelease(std::move(pre_release)),
            mBuildMetadata(std::move(build_metadata)) {}

        static std::optional<SemanticVersion> fromString(std::string_view versionString) {
            // Regex from https://semver.org/#is-there-a-suggested-regular-expression-regex-to-check-a-semver-string
            static const std::regex semverRegex(
                R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"
            );

            std::string s{versionString};
            std::smatch match;
            if (std::regex_match(s, match, semverRegex)) {
                SemanticVersion version;
                version.mMajor = std::stoi(match[1].str());
                version.mMinor = std::stoi(match[2].str());
                version.mPatch = std::stoi(match[3].str());
                if (match[4].matched)
                    version.mPreRelease = match[4].str();
                if (match[5].matched)
                    version.mBuildMetadata = match[5].str();
                return version;
            }

            return std::nullopt;
        }

        std::string toString() const {
            std::stringstream ss;
            ss << mMajor << "." << mMinor << "." << mPatch;
            if (!mPreRelease.empty())
                ss << "-" << mPreRelease;

            if (!mBuildMetadata.empty())
                ss << "+" << mBuildMetadata;

            return ss.str();
        }

        constexpr auto operator<=>(const SemanticVersion &other) const {
            if (mMajor != other.mMajor)
                return mMajor <=> other.mMajor;
            if (mMinor != other.mMinor)
                return mMinor <=> other.mMinor;
            if (mPatch != other.mPatch)
                return mPatch <=> other.mPatch;
            if (mPreRelease.empty() && !other.mPreRelease.empty())
                return std::strong_ordering::greater;
            if (!mPreRelease.empty() && other.mPreRelease.empty())
                return std::strong_ordering::less;
            if (!mPreRelease.empty() && !other.mPreRelease.empty()) {
                if (mPreRelease < other.mPreRelease) return std::strong_ordering::less;
                if (mPreRelease > other.mPreRelease) return std::strong_ordering::greater;
            }
            return std::strong_ordering::equal;
        };

        constexpr bool operator==(const SemanticVersion &other) const {
            return mMajor == other.mMajor
                && mMinor == other.mMinor
                && mPatch == other.mPatch
                && mPreRelease == other.mPreRelease;
        }
    };

} // namespace sapphire