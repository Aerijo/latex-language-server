//
// Created by Benjamin Gray on 20/12/18.
//

#ifndef LATEX_LANGUAGE_SERVER_MESSAGE_H
#define LATEX_LANGUAGE_SERVER_MESSAGE_H

#include <string>
#include <rapidjson/document.h>

class Message {
    std::string id;
    std::string method;
    rapidjson::Document payload;

    explicit Message (rapidjson::Document &json);
};


#endif //LATEX_LANGUAGE_SERVER_MESSAGE_H
