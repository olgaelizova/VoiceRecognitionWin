
#ifdef _WIN32

#pragma once

#include "resource.h"

#include "Recognition.h"

#else
#ifdef __unix__
//TODO realization for UNIX console

#else
#error Unsupported Implementation
#endif
#endif
