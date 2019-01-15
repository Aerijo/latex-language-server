#ifndef LATEX_LANGUAGE_SERVER_INITIALIZE_H
#define LATEX_LANGUAGE_SERVER_INITIALIZE_H

#include <Handler.h>

class InitializeHandler : public RequestHandler {
public:
    InitializeHandler () : RequestHandler ("initialize") {};

    void run (Id &id, optional<Value> &params) override;
};

#endif //LATEX_LANGUAGE_SERVER_INITIALIZE_H
