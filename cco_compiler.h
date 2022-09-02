#ifndef CCO_COMPILER_H
#define CCO_COMPILER_H

#ifndef typeof
#define typeof(x) __typeof__((x))
#endif

/*
 * __has_builtin is supported on gcc >= 10, clang >= 3 and icc >= 21.
 * In the meantime, to support gcc < 10, we implement __has_builtin
 * by hand.
 */
#ifndef __has_builtin

#define __has_builtin(x) (0)

#else

#if !__has_builtin(__builtin_types_compatible_p)
#error "__builtin_types_compatible_p not available"
#endif

#if !__has_builtin(__builtin_clzll)
#error "__builtin_clzll not available"
#endif

#if !__has_builtin(__builtin_constant_p)
#error "__builtin_constant_p not available"
#endif

#endif

#endif
