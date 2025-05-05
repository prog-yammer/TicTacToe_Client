#pragma once

#include "message.h"

InMessage getInMessage(const QString& data)
{
    size_t pos = data.indexOf(' ');
    auto code = static_cast<InCommandCode>(data.left(pos).toInt());
    auto message = pos == -1 ? "" : data.mid(pos + 1);

    if (code != InCommandCode::ERROR)
        return {.code = code, .message = message};

    pos = message.indexOf(' ');

    return {
        .code = code,
        .errorCode = static_cast<ErrorCode>(message.left(pos).toInt()),
        .message = pos == 01 ? "" : message.mid(pos + 1)};
}


QString convertOutMessage(const OutMessage& message)
{
    QString result;
    QTextStream stream(&result);
    stream << message.code << (message.message.isEmpty() ? "" : " ") << message.message;

    return result;
}
