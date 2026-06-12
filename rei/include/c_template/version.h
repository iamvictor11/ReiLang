#ifndef C_TEMPLATE_VERSION_H
#define C_TEMPLATE_VERSION_H

#include <stdint.h>

#define C_TEMPLATE_DEFN_VERSION(ATTR, SPREFIX, LPREFIX) \
    typedef struct LPREFIX##Version \
    { \
        const char* name; \
        uint8_t major; \
        uint8_t minor; \
        uint8_t patch; \
        void* user; \
    } LPREFIX##Version;
#define C_TEMPLATE_VERSION \
{ \
    .name = "Nanase Kurumi", \
    .major = 1, \
    .minor = 1, \
    .patch = 1, \
    .user = NULL \
}
#define C_TEMPLATE_VERSION_TO_UINT(MAJOR, MINOR, PATCH) \
    (((uint32_t)(MAJOR) << 16) | ((uint32_t)(MINOR) << 8) | (uint32_t)(PATCH))
#define C_TEMPLATE_VERSION_CAST_UINT(VERSION) \
    C_TEMPLATE_VERSION_TO_UINT((VERSION).major, (VERSION).minor, (VERSION).patch)
#define C_TEMPLATE_UINT_TO_MAJOR(VERSION) (((VERSION) >> 16) & 0xFF)
#define C_TEMPLATE_UINT_TO_MINOR(VERSION) (((VERSION) >> 8) & 0xFF)
#define C_TEMPLATE_UINT_TO_PATCH(VERSION) ((VERSION) & 0xFF)

#endif
