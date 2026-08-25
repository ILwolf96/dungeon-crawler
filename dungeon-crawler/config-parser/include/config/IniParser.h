#pragma once

#include "IConfigParser.h"

namespace config
{
    class IniParser final : public IConfigParser
    {
    public:
        [[nodiscard]]
        ConfigData parse(std::string_view filePath) const override;
    };
}