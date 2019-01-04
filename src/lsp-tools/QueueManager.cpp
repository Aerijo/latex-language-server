#include "QueueManager.h"
#include "library.h"

QueueManager* QueueManager::instance;

void QueueManager::init () {
    delete instance;
    instance = new QueueManager();
}

void QueueManager::writeStdout (Document &message) {
//    std::cerr << "writing message to stdout...\n";
//    sendMessage(message);

//    if (!message.IsObject()) {
//        std::cerr << "NULL ON THE WAY OUT BOIS!!!\n";
//    }
    getInstance()->for_stdout.enqueue(std::move(message));
}

void QueueManager::pushMessage (Document &message) {
    std::cerr << "pushing message for handlers...\n";
    getInstance()->for_handlers.enqueue(std::move(message));
}

