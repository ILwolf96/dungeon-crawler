#pragma once

#include "IConfigParser.h"

namespace config
{
    class JsonParser final : public IConfigParser
    {
    public:
        [[nodiscard]]
        ConfigData parse(std::string_view filePath) const override;
    };
}