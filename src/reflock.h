#ifndef REFLOCK_H_
#define REFLOCK_H_

/* The reflock is a special kind of lock that normally prevents a chunk of
 * memory from being freed, but does allow the chunk of memory to eventually be
 * released in a coordinated fashion.
 *
 * Under normal operation, threads increase and decrease the reference count,
 * which are wait-free operations.
 *
 * Exactly once during the reflock's lifecycle, a thread holding a reference to
 * the lock may "destroy" the lock; this operation blocks until all other
 * threads holding a reference to the lock have dereferenced it. After
 * "destroy" returns, the calling thread may assume that no other threads have
 * a reference to the lock.
 *
 * Attemmpting to lock or destroy a lock after reflock_unref_and_destroy() has
 * been called is invalid and results in undefined behavior. Therefore the user
 * should use another lock to guarantee that this can't happen.
 */

#include <stdint.h>

#include "internal.h"

typedef struct reflock {
  uint32_t state;
} reflock_t;

EPOLL_INTERNAL int reflock_global_init(void);

EPOLL_INTERNAL void reflock_init(reflock_t* reflock);
EPOLL_INTERNAL void reflock_ref(reflock_t* reflock);
EPOLL_INTERNAL void reflock_unref(reflock_t* reflock);
EPOLL_INTERNAL void reflock_unref_and_destroy(reflock_t* reflock);

#endif /* REFLOCK_H_ */
