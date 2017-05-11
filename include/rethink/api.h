// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#define API_LIB_CLIENT __attribute__((visibility("default")))
#define API_LIB_DYNAMIC __attribute__((visibility("default")))
#define API_LIB_STATIC __attribute__((visibility("hidden")))

#ifndef RETHINK_API
#if defined STATIC_RETHINK_API
#define RETHINK_API API_LIB_STATIC
#elif defined EXPORT_RETHINK_API
#define RETHINK_API API_LIB_DYNAMIC
#else
#define RETHINK_API API_LIB_CLIENT
#endif
#endif  // RETHINK_API

/* Don't need to export/import fully inline APIs */
#ifndef RETHINK_INLINE_API
#define RETHINK_INLINE_API
#endif  // RETHINK_INLINE_API
