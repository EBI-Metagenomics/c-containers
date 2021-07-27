#ifndef CONTAINERS_CONTAINER_OF_H
#define CONTAINERS_CONTAINER_OF_H

#include <stddef.h>

/**
 * cco_container_of - cast a member of a structure out to the containing
 * structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define cco_container_of(ptr, type, member)                                    \
  ({                                                                           \
    char *__mptr = (char *)(ptr);                                              \
    ((type *)(__mptr - offsetof(type, member)));                               \
  })

/**
 * cco_container_of_safe - cast a member of a structure out to the containing
 * structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 * Return NULL if ptr is NULL.
 */
#define cco_container_of_safe(ptr, type, member)                               \
  ({                                                                           \
    char *__mptr = (char *)(ptr);                                              \
    __mptr == NULL ? (type *)__mptr                                            \
                   : ((type *)(__mptr - offsetof(type, member)));              \
  })

#endif
