#pragma once

#include "ConfigData.h"
#include "ParserFactory.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

namespace config
{
    class DataRepository final
    {
    public:
        [[nodiscard]]
        ConfigData load(std::string_view entryFilePath) const
        {
            const std::filesystem::path entryPath{
                std::string(entryFilePath) };

            auto entryParser = makeParser(entryFilePath);
            const ConfigData entryData =
                entryParser->parse(entryFilePath);

            if (!entryData.hasSection("data"))
            {
                throw std::runtime_error(
                    "Configuration manifest is missing the [data] section: " +
                    std::string(entryFilePath));
            }

            ConfigData combinedData;
            const std::filesystem::path entryDirectory =
                entryPath.parent_path();

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "game",
                entryPath.extension().string());

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "map",
                entryPath.extension().string());

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "player",
                entryPath.extension().string());

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "enemies",
                entryPath.extension().string());

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "gear",
                entryPath.extension().string());

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "consumables",
                entryPath.extension().string());

            loadAndMerge(
                combinedData,
                entryDirectory,
                entryData,
                "loot",
                entryPath.extension().string());

            return combinedData;
        }

    private:
        static std::string getRequiredString(
            const ConfigData& data,
            std::string_view section,
            std::string_view key)
        {
            const std::string sectionName(section);
            const std::string keyName(key);

            if (!data.hasValue(sectionName, keyName))
            {
                throw std::runtime_error(
                    "Missing configuration value: " +
                    sectionName + "." +
                    keyName);
            }

            const std::string value =
                data.getValue(sectionName, keyName);

            if (value.empty())
            {
                throw std::runtime_error(
                    "Configuration value cannot be empty: " +
                    sectionName + "." +
                    keyName);
            }

            return value;
        }

        static void loadAndMerge(
            ConfigData& destination,
            const std::filesystem::path& entryDirectory,
            const ConfigData& manifest,
            std::string_view manifestKey,
            std::string_view expectedExtension)
        {
            const std::string relativeFile =
                getRequiredString(
                    manifest,
                    "data",
                    manifestKey);

            const std::filesystem::path dataPath =
                entryDirectory / relativeFile;

            if (dataPath.extension().string() != expectedExtension)
            {
                throw std::runtime_error(
                    "Configuration data file uses a different format than the "
                    "entry file: " +
                    dataPath.string());
            }

            const std::string dataPathString =
                dataPath.string();

            auto parser = makeParser(dataPathString);
            const ConfigData data =
                parser->parse(dataPathString);

            for (const auto& [sectionName, sectionData] :
                data.sections())
            {
                for (const auto& [key, value] : sectionData)
                {
                    if (destination.hasValue(sectionName, key))
                    {
                        throw std::runtime_error(
                            "Duplicate configuration value: " +
                            sectionName + "." +
                            key);
                    }

                    destination.set(
                        sectionName,
                        key,
                        value);
                }
            }
        }
    };
}
