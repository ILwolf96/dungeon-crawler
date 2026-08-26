#include "config/ParserFactory.h"

#include "config/IniParser.h"
#include "config/JsonParser.h"

#include <filesystem>
#include <stdexcept>
#include <string>

namespace config
{
    std::unique_ptr<IConfigParser> makeParser(
        std::string_view filePath)
    {
        const std::filesystem::path path{ std::string(filePath) };
        const std::string extension = path.extension().string();

        if (extension == ".ini")
        {
            return std::make_unique<IniParser>();
        }

        if (extension == ".json")
        {
            return std::make_unique<JsonParser>();
        }

        throw std::invalid_argument(
            "Unsupported configuration format: " + extension);
    }
}