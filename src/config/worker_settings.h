#ifndef WORKER_SETTINGS_H
#define WORKER_SETTINGS_H

// Check if CLUSTER_ID is defined, otherwise default to -1
#ifndef CLUSTER_ID
#define CLUSTER_ID -1
#endif

#ifndef CLUSTER_ID
#define DMQTT_CLIENT_ID "TCLedTableWorker"
#endif

#endif // WORKER_SETTINGS_H