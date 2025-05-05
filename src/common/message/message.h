#pragma once

#include "command_code.h"

#include <QString>

#include <optional>
#include <string>

struct InMessage
{
    InCommandCode code;
    std::optional<ErrorCode> errorCode;
    QString message;
};

struct OutMessage
{
    OutCommandCode code;
    QString message;
};
