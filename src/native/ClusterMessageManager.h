// ClusterMessageManager.h
#ifndef CLUSTERMESSAGEMANAGER_H
#define CLUSTERMESSAGEMANAGER_H

#include "ClusterMessage.h"

class ClusterMessageManager {
private:
    int lastSequenceNumber;

public:
    ClusterMessageManager(int lastSequenceNumber): lastSequenceNumber(0) {}
};

#endif // CLUSTERMESSAGEMANAGER_H
