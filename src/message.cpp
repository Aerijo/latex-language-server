//
// Created by Benjamin Gray on 20/12/18.
//

#include <rapidjson/document.h>

#include "message.h"
#include "message_handler.h"

using namespace rapidjson;

Message::Message (Document &json) {
    this->id = getId(json);
}