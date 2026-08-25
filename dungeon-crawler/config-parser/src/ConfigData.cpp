#include <config/ConfigData.h>

#include <stdexcept>

namespace config
{
    void ConfigData::set(
        std::string_view section,
        std::string_view key,
        std::string_view value)
    {
        m_sections[std::string(section)][std::string(key)] = std::string(value);
    }

    bool ConfigData::hasSection(std::string_view section) const noexcept
    {
        return m_sections.find(std::string(section)) != m_sections.end();
    }

    bool ConfigData::hasValue(
        std::string_view section,
        std::string_view key) const noexcept
    {
        const auto sectionIt = m_sections.find(std::string(section));

        if (sectionIt == m_sections.end())
        {
            return false;
        }

        return sectionIt->second.find(std::string(key)) !=
            sectionIt->second.end();
    }

    const std::string& ConfigData::getValue(
        std::string_view section,
        std::string_view key) const
    {
        const auto sectionIt = m_sections.find(std::string(section));

        if (sectionIt == m_sections.end())
        {
            throw std::out_of_range(
                "Configuration section not found: " + std::string(section));
        }

        const auto valueIt = sectionIt->second.find(std::string(key));

        if (valueIt == sectionIt->second.end())
        {
            throw std::out_of_range(
                "Configuration key not found: " +
                std::string(section) + "." +
                std::string(key));
        }

        return valueIt->second;
    }

    const ConfigData::SectionMap& ConfigData::sections() const noexcept
    {
        return m_sections;
    }

    const ConfigData::KeyValueMap& ConfigData::section(
        std::string_view section) const
    {
        const auto sectionIt = m_sections.find(std::string(section));

        if (sectionIt == m_sections.end())
        {
            throw std::out_of_range(
                "Configuration section not found: " + std::string(section));
        }

        return sectionIt->second;
    }
}