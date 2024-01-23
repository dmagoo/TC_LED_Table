// ClusterMessage.h
#ifndef CLUSTERMESSAGE_H
#define CLUSTERMESSAGE_H

#include "ClusterMessage.h"
#include "ClusterCommands.h"

class ClusterMessage {
private:
    int clusterId;
    ClusterCommandType commandType;
    CommandParamsVariant params;

public:
    ClusterMessage(int clusterId, ClusterCommandType commandType, CommandParamsVariant params)
        : clusterId(clusterId), commandType(commandType), params(params) {}

    int getClusterId() const;
    ClusterCommandType getCommandType() const;    
    CommandParamsVariant getParams() const;
};

std::vector<uint8_t> serializeClusterMessage(const ClusterMessage& message);

ClusterMessage deserializeClusterMessage(const std::vector<uint8_t>& data);

#endif // CLUSTERMESSAGE_H
