#pragma once

#include <exceptions/AssertionException.h>

#define assertz(x,y,z) if (! (x)) { throw zapata::AssertionException(y, z, #x, __LINE__, __FILE__); }