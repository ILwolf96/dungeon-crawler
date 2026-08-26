#include "config/JsonParser.h"

#include <stdexcept>

namespace config
{
    ConfigData JsonParser::parse(std::string_view /*filePath*/) const
    {
        throw std::runtime_error(
            "JSON parsing is not implemented yet.");
    }
}