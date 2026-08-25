#pragma once

#include "ConfigData.h"

#include <string>
#include <string_view>

namespace config
{
    class IConfigParser
    {
    public:
        virtual ~IConfigParser() = default;

        [[nodiscard]]
        virtual ConfigData parse(std::string_view filePath) const = 0;
    };
}