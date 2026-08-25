#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace config
{
    class ConfigData
    {
    public:
        using KeyValueMap = std::unordered_map<std::string, std::string>;
        using SectionMap = std::unordered_map<std::string, KeyValueMap>;

        ConfigData() = default;

        void set(
            std::string_view section,
            std::string_view key,
            std::string_view value);

        [[nodiscard]]
        bool hasSection(std::string_view section) const noexcept;

        [[nodiscard]]
        bool hasValue(
            std::string_view section,
            std::string_view key) const noexcept;

        [[nodiscard]]
        const std::string& getValue(
            std::string_view section,
            std::string_view key) const;

        [[nodiscard]]
        const SectionMap& sections() const noexcept;

        [[nodiscard]]
        const KeyValueMap& section(
            std::string_view section) const;

    private:
        SectionMap m_sections;
    };
}