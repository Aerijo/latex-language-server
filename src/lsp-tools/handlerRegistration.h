#ifndef LATEX_LANGUAGE_SERVER_HANDLERREGISTRATION_H
#define LATEX_LANGUAGE_SERVER_HANDLERREGISTRATION_H

#include "MessageHandler.h"

#include "handlers/initialize.h"

void registerHandlers (MessageHandler *messageHandler) {
    messageHandler->registerHandler(new InitializeHandler());
}

#endif //LATEX_LANGUAGE_SERVER_HANDLERREGISTRATION_H
