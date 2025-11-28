/* Copyright (c) 2025, Open Brain Institute
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define PRAGMA(x) _Pragma(#x)

#if defined(__clang__)
    #define DISABLE_WARNING_PUSH           PRAGMA(clang diagnostic push)
    #define DISABLE_WARNING_POP            PRAGMA(clang diagnostic pop)
    #define DISABLE_WARNING(warningName)   PRAGMA(clang diagnostic ignored #warningName)
#elif defined(__GNUC__)
    #define DISABLE_WARNING_PUSH           PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            PRAGMA(GCC diagnostic pop)
    #define DISABLE_WARNING(warningName)   PRAGMA(GCC diagnostic ignored #warningName)
#elif defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH           __pragma(warning(push))
    #define DISABLE_WARNING_POP            __pragma(warning(pop))
    #define DISABLE_WARNING(warningNumber) __pragma(warning(disable: warningNumber))
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING(warningName)
#endif

