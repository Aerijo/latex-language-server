#ifndef LANGUAGESERVER_TOOLS_QUEUEMANAGER_H
#define LANGUAGESERVER_TOOLS_QUEUEMANAGER_H

#include <rapidjson/document.h>

#include "ThreadedQueue.h"

using namespace rapidjson;

class QueueManager {
private:
    QueueManager() = default;
    static QueueManager *instance;
public:
    static QueueManager *getInstance () { return instance; };
    static void init ();
    static void writeStdout (Document &message);
    static void pushMessage (Document &message);

    ThreadedQueue<Document> for_stdout;
    ThreadedQueue<Document> for_handlers;
};


#endif //LANGUAGESERVER_TOOLS_QUEUEMANAGER_H
