/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_SRC_CORE_PROTO_SENSORDATAPROTO_PB_H_INCLUDED
#define PB_SRC_CORE_PROTO_SENSORDATAPROTO_PB_H_INCLUDED
#include <pb.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb_common.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* Whether the message is a result of a touch event or a periodic notification */
typedef enum _SensorEventDataProto_EventType {
    SensorEventDataProto_EventType_TOUCH_EVENT = 0,
    SensorEventDataProto_EventType_PERIODIC_NOTIFICATION = 1
} SensorEventDataProto_EventType;

/* Struct definitions */
/* Message definition for sensor data */
typedef struct _SensorDataProto {
    /* Data sent always */
    uint32_t node_id;
    uint32_t current_value;
    bool touched;
    /* Data sent periodically */
    uint32_t threshold_off;
    uint32_t threshold_on;
} SensorDataProto;

/* Message to differentiate touch event vs. periodic notification */
typedef struct _SensorEventDataProto {
    /* Type of event */
    SensorEventDataProto_EventType event_type;
    /* Sensor data associated with the event */
    bool has_sensor_data;
    SensorDataProto sensor_data;
} SensorEventDataProto;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _SensorEventDataProto_EventType_MIN SensorEventDataProto_EventType_TOUCH_EVENT
#define _SensorEventDataProto_EventType_MAX SensorEventDataProto_EventType_PERIODIC_NOTIFICATION
#define _SensorEventDataProto_EventType_ARRAYSIZE ((SensorEventDataProto_EventType)(SensorEventDataProto_EventType_PERIODIC_NOTIFICATION+1))


#define SensorEventDataProto_event_type_ENUMTYPE SensorEventDataProto_EventType


/* Initializer values for message structs */
#define SensorDataProto_init_default             {0, 0, 0, 0, 0}
#define SensorEventDataProto_init_default        {_SensorEventDataProto_EventType_MIN, false, SensorDataProto_init_default}
#define SensorDataProto_init_zero                {0, 0, 0, 0, 0}
#define SensorEventDataProto_init_zero           {_SensorEventDataProto_EventType_MIN, false, SensorDataProto_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define SensorDataProto_node_id_tag              1
#define SensorDataProto_current_value_tag        2
#define SensorDataProto_touched_tag              3
#define SensorDataProto_threshold_off_tag        4
#define SensorDataProto_threshold_on_tag         5
#define SensorEventDataProto_event_type_tag      1
#define SensorEventDataProto_sensor_data_tag     2

/* Struct field encoding specification for nanopb */
#define SensorDataProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   node_id,           1) \
X(a, STATIC,   SINGULAR, UINT32,   current_value,     2) \
X(a, STATIC,   SINGULAR, BOOL,     touched,           3) \
X(a, STATIC,   SINGULAR, UINT32,   threshold_off,     4) \
X(a, STATIC,   SINGULAR, UINT32,   threshold_on,      5)
#define SensorDataProto_CALLBACK NULL
#define SensorDataProto_DEFAULT NULL

#define SensorEventDataProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    event_type,        1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  sensor_data,       2)
#define SensorEventDataProto_CALLBACK NULL
#define SensorEventDataProto_DEFAULT NULL
#define SensorEventDataProto_sensor_data_MSGTYPE SensorDataProto

extern const pb_msgdesc_t SensorDataProto_msg;
extern const pb_msgdesc_t SensorEventDataProto_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define SensorDataProto_fields &SensorDataProto_msg
#define SensorEventDataProto_fields &SensorEventDataProto_msg

/* Maximum encoded size of messages (where known) */
#define SRC_CORE_PROTO_SENSORDATAPROTO_PB_H_MAX_SIZE SensorEventDataProto_size
#define SensorDataProto_size                     26
#define SensorEventDataProto_size                30

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif