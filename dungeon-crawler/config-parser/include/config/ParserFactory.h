#pragma once

#include "IConfigParser.h"

#include <memory>
#include <string_view>

namespace config
{
    [[nodiscard]]
    std::unique_ptr<IConfigParser> makeParser(
        std::string_view filePath);
}