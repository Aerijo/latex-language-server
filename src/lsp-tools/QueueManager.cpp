#include "QueueManager.h"

QueueManager* QueueManager::instance;

void QueueManager::init () {
    delete instance;
    instance = new QueueManager();
}

void QueueManager::writeStdout (Document &message) {
    getInstance()->for_stdout.enqueue(std::move(message));
}

void QueueManager::pushMessage (Document &message) {
    getInstance()->for_handlers.enqueue(std::move(message));
}

