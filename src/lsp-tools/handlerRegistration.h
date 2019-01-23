#ifndef LATEX_LANGUAGE_SERVER_HANDLERREGISTRATION_H
#define LATEX_LANGUAGE_SERVER_HANDLERREGISTRATION_H

#include "MessageHandler.h"

#include "handlers/file/FileChangeHandler.h"
#include "handlers/file/FileOpenHandler.h"

#include "handlers/autocomplete/CompletionProvider.h"

#include "handlers/hover/HoverHandler.h"
#include "handlers/InitializeHandler.h"
#include "handlers/ShutDownHandler.h"

void registerHandlers (MessageHandler *messageHandler) {
    messageHandler->registerHandler(new InitializeHandler());
    messageHandler->registerHandler(new FileChangeHandler());
    messageHandler->registerHandler(new FileOpenHandler());
//    messageHandler->registerHandler(new HoverHandler());
    messageHandler->registerHandler(new ShutDownHandler());
    messageHandler->registerHandler(new CompletionProvider());
}

#endif //LATEX_LANGUAGE_SERVER_HANDLERREGISTRATION_H
