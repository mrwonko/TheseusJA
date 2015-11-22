#pragma once

#include <cstdint>

#include "qcommon/safe/gsl.h"
#include "../qcommon/q_int.h"

namespace Com
{
	std::uint32_t BlockChecksum( const gsl::array_view< const byte >& buffer );
}
