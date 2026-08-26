#include "config/IniParser.h"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

namespace
{
    std::string trim(const std::string& value)
    {
        std::size_t first = 0;

        while (first < value.size() &&
            std::isspace(static_cast<unsigned char>(value[first])))
        {
            ++first;
        }

        if (first == value.size())
        {
            return {};
        }

        std::size_t last = value.size() - 1;

        while (last > first &&
            std::isspace(static_cast<unsigned char>(value[last])))
        {
            --last;
        }

        return value.substr(first, last - first + 1);
    }
}

namespace config
{
    ConfigData IniParser::parse(std::string_view filePath) const
    {
        std::ifstream file{ std::string(filePath) };

        if (!file.is_open())
        {
            throw std::runtime_error(
                "Unable to open configuration file: " +
                std::string(filePath));
        }

        ConfigData data;
        std::string currentSection;
        std::string line;
        std::size_t lineNumber = 0;

        while (std::getline(file, line))
        {
            ++lineNumber;

            const std::string trimmedLine = trim(line);

            // Ignore empty lines.
            if (trimmedLine.empty())
            {
                continue;
            }

            // Semicolon is treated as an INI comment.
            if (trimmedLine.front() == ';')
            {
                continue;
            }

            // Section: [section.name]
            if (trimmedLine.front() == '[')
            {
                if (trimmedLine.back() != ']')
                {
                    throw std::runtime_error(
                        "Malformed section declaration at line " +
                        std::to_string(lineNumber));
                }

                const std::string sectionName =
                    trim(trimmedLine.substr(
                        1,
                        trimmedLine.size() - 2));

                if (sectionName.empty())
                {
                    throw std::runtime_error(
                        "Empty section name at line " +
                        std::to_string(lineNumber));
                }

                currentSection = sectionName;

                continue;
            }

            // A key/value pair must belong to a section.
            if (currentSection.empty())
            {
                throw std::runtime_error(
                    "Key/value pair appears before a section at line " +
                    std::to_string(lineNumber));
            }

            const std::size_t equalsPosition = trimmedLine.find('=');

            if (equalsPosition == std::string::npos)
            {
                throw std::runtime_error(
                    "Malformed key/value pair at line " +
                    std::to_string(lineNumber));
            }

            const std::string key =
                trim(trimmedLine.substr(0, equalsPosition));

            const std::string value =
                trim(trimmedLine.substr(equalsPosition + 1));

            if (key.empty())
            {
                throw std::runtime_error(
                    "Empty key at line " +
                    std::to_string(lineNumber));
            }

            data.set(currentSection, key, value);
        }

        if (file.bad())
        {
            throw std::runtime_error(
                "Error while reading configuration file: " +
                std::string(filePath));
        }

        return data;
    }
}