#include "config/JsonParser.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

namespace config
{
    ConfigData JsonParser::parse(std::string_view filePath) const
    {
        std::ifstream file{ std::string(filePath) };

        if (!file.is_open())
        {
            throw std::runtime_error(
                "Unable to open configuration file: " +
                std::string(filePath));
        }

        try
        {
            const nlohmann::json jsonData = nlohmann::json::parse(file);

            if (!jsonData.is_object())
            {
                throw std::runtime_error(
                    "JSON configuration root must be an object.");
            }

            ConfigData data;

            for (const auto& [sectionName, sectionData] : jsonData.items())
            {
                if (!sectionData.is_object())
                {
                    throw std::runtime_error(
                        "JSON section must be an object: " +
                        sectionName);
                }

                for (const auto& [key, value] : sectionData.items())
                {
                    if (value.is_string())
                    {
                        data.set(
                            sectionName,
                            key,
                            value.get<std::string>());
                    }
                    else if (value.is_number() ||
                        value.is_boolean())
                    {
                        data.set(
                            sectionName,
                            key,
                            value.dump());
                    }
                    else
                    {
                        throw std::runtime_error(
                            "Unsupported JSON value for " +
                            sectionName + "." + key);
                    }
                }
            }

            return data;
        }
        catch (const nlohmann::json::parse_error& error)
        {
            throw std::runtime_error(
                "Malformed JSON configuration '" +
                std::string(filePath) +
                "': " +
                error.what());
        }
    }
}