/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_SRC_NATIVE_PROTO_CLUSTERMESSAGESPROTO_PB_H_INCLUDED
#define PB_SRC_NATIVE_PROTO_CLUSTERMESSAGESPROTO_PB_H_INCLUDED
#include <pb.h>
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb_common.h"
#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _CommandTypeProto {
    CommandTypeProto_FILL_NODE = 0,
    CommandTypeProto_BLIT_NODE = 1,
    CommandTypeProto_SET_NODE_PIXEL = 2,
    CommandTypeProto_QUEUE_NODE_PIXEL = 3,
    CommandTypeProto_DEQUEUE_NODE_PIXEL = 4
} CommandTypeProto;

/* Struct definitions */
typedef struct _FillNodeParamsProto {
    int32_t node_id;
    uint32_t color;
} FillNodeParamsProto;

typedef struct _BlitNodeParamsProto {
    int32_t node_id;
    pb_callback_t colors;
    uint32_t pad_color;
} BlitNodeParamsProto;

typedef struct _SetNodePixelParamsProto {
    int32_t node_id;
    uint32_t color;
} SetNodePixelParamsProto;

typedef struct _QueueNodePixelParamsProto {
    int32_t node_id;
    uint32_t color;
} QueueNodePixelParamsProto;

typedef struct _DequeueNodePixelParamsProto {
    int32_t node_id;
    uint32_t color;
} DequeueNodePixelParamsProto;

typedef struct _ClusterMessageProto {
    int32_t cluster_id;
    CommandTypeProto command_type;
    pb_size_t which_params;
    union {
        FillNodeParamsProto fill_node_params;
        BlitNodeParamsProto blit_node_params;
        SetNodePixelParamsProto set_node_pixel_params;
        QueueNodePixelParamsProto queue_node_pixel_params;
        DequeueNodePixelParamsProto dequeue_node_pixel_params;
    } params;
} ClusterMessageProto;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _CommandTypeProto_MIN CommandTypeProto_FILL_NODE
#define _CommandTypeProto_MAX CommandTypeProto_DEQUEUE_NODE_PIXEL
#define _CommandTypeProto_ARRAYSIZE ((CommandTypeProto)(CommandTypeProto_DEQUEUE_NODE_PIXEL+1))






#define ClusterMessageProto_command_type_ENUMTYPE CommandTypeProto


/* Initializer values for message structs */
#define FillNodeParamsProto_init_default         {0, 0}
#define BlitNodeParamsProto_init_default         {0, {{NULL}, NULL}, 0}
#define SetNodePixelParamsProto_init_default     {0, 0}
#define QueueNodePixelParamsProto_init_default   {0, 0}
#define DequeueNodePixelParamsProto_init_default {0, 0}
#define ClusterMessageProto_init_default         {0, _CommandTypeProto_MIN, 0, {FillNodeParamsProto_init_default}}
#define FillNodeParamsProto_init_zero            {0, 0}
#define BlitNodeParamsProto_init_zero            {0, {{NULL}, NULL}, 0}
#define SetNodePixelParamsProto_init_zero        {0, 0}
#define QueueNodePixelParamsProto_init_zero      {0, 0}
#define DequeueNodePixelParamsProto_init_zero    {0, 0}
#define ClusterMessageProto_init_zero            {0, _CommandTypeProto_MIN, 0, {FillNodeParamsProto_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define FillNodeParamsProto_node_id_tag          1
#define FillNodeParamsProto_color_tag            2
#define BlitNodeParamsProto_node_id_tag          1
#define BlitNodeParamsProto_colors_tag           2
#define BlitNodeParamsProto_pad_color_tag        3
#define SetNodePixelParamsProto_node_id_tag      1
#define SetNodePixelParamsProto_color_tag        2
#define QueueNodePixelParamsProto_node_id_tag    1
#define QueueNodePixelParamsProto_color_tag      2
#define DequeueNodePixelParamsProto_node_id_tag  1
#define DequeueNodePixelParamsProto_color_tag    2
#define ClusterMessageProto_cluster_id_tag       1
#define ClusterMessageProto_command_type_tag     2
#define ClusterMessageProto_fill_node_params_tag 3
#define ClusterMessageProto_blit_node_params_tag 4
#define ClusterMessageProto_set_node_pixel_params_tag 5
#define ClusterMessageProto_queue_node_pixel_params_tag 6
#define ClusterMessageProto_dequeue_node_pixel_params_tag 7

/* Struct field encoding specification for nanopb */
#define FillNodeParamsProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    node_id,           1) \
X(a, STATIC,   SINGULAR, UINT32,   color,             2)
#define FillNodeParamsProto_CALLBACK NULL
#define FillNodeParamsProto_DEFAULT NULL

#define BlitNodeParamsProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    node_id,           1) \
X(a, CALLBACK, REPEATED, UINT32,   colors,            2) \
X(a, STATIC,   SINGULAR, UINT32,   pad_color,         3)
#define BlitNodeParamsProto_CALLBACK pb_default_field_callback
#define BlitNodeParamsProto_DEFAULT NULL

#define SetNodePixelParamsProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    node_id,           1) \
X(a, STATIC,   SINGULAR, UINT32,   color,             2)
#define SetNodePixelParamsProto_CALLBACK NULL
#define SetNodePixelParamsProto_DEFAULT NULL

#define QueueNodePixelParamsProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    node_id,           1) \
X(a, STATIC,   SINGULAR, UINT32,   color,             2)
#define QueueNodePixelParamsProto_CALLBACK NULL
#define QueueNodePixelParamsProto_DEFAULT NULL

#define DequeueNodePixelParamsProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    node_id,           1) \
X(a, STATIC,   SINGULAR, UINT32,   color,             2)
#define DequeueNodePixelParamsProto_CALLBACK NULL
#define DequeueNodePixelParamsProto_DEFAULT NULL

#define ClusterMessageProto_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    cluster_id,        1) \
X(a, STATIC,   SINGULAR, UENUM,    command_type,      2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (params,fill_node_params,params.fill_node_params),   3) \
X(a, STATIC,   ONEOF,    MESSAGE,  (params,blit_node_params,params.blit_node_params),   4) \
X(a, STATIC,   ONEOF,    MESSAGE,  (params,set_node_pixel_params,params.set_node_pixel_params),   5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (params,queue_node_pixel_params,params.queue_node_pixel_params),   6) \
X(a, STATIC,   ONEOF,    MESSAGE,  (params,dequeue_node_pixel_params,params.dequeue_node_pixel_params),   7)
#define ClusterMessageProto_CALLBACK NULL
#define ClusterMessageProto_DEFAULT NULL
#define ClusterMessageProto_params_fill_node_params_MSGTYPE FillNodeParamsProto
#define ClusterMessageProto_params_blit_node_params_MSGTYPE BlitNodeParamsProto
#define ClusterMessageProto_params_set_node_pixel_params_MSGTYPE SetNodePixelParamsProto
#define ClusterMessageProto_params_queue_node_pixel_params_MSGTYPE QueueNodePixelParamsProto
#define ClusterMessageProto_params_dequeue_node_pixel_params_MSGTYPE DequeueNodePixelParamsProto

extern const pb_msgdesc_t FillNodeParamsProto_msg;
extern const pb_msgdesc_t BlitNodeParamsProto_msg;
extern const pb_msgdesc_t SetNodePixelParamsProto_msg;
extern const pb_msgdesc_t QueueNodePixelParamsProto_msg;
extern const pb_msgdesc_t DequeueNodePixelParamsProto_msg;
extern const pb_msgdesc_t ClusterMessageProto_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define FillNodeParamsProto_fields &FillNodeParamsProto_msg
#define BlitNodeParamsProto_fields &BlitNodeParamsProto_msg
#define SetNodePixelParamsProto_fields &SetNodePixelParamsProto_msg
#define QueueNodePixelParamsProto_fields &QueueNodePixelParamsProto_msg
#define DequeueNodePixelParamsProto_fields &DequeueNodePixelParamsProto_msg
#define ClusterMessageProto_fields &ClusterMessageProto_msg

/* Maximum encoded size of messages (where known) */
/* BlitNodeParamsProto_size depends on runtime parameters */
/* ClusterMessageProto_size depends on runtime parameters */
#define DequeueNodePixelParamsProto_size         17
#define FillNodeParamsProto_size                 17
#define QueueNodePixelParamsProto_size           17
#define SRC_NATIVE_PROTO_CLUSTERMESSAGESPROTO_PB_H_MAX_SIZE FillNodeParamsProto_size
#define SetNodePixelParamsProto_size             17

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
