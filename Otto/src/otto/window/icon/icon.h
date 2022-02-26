#pragma once

#include "otto/base.h"

namespace otto {

	struct Icon
	{
		uint16 width = 0;
		uint16 height = 0;
		uint8* bitmap = nullptr;
	};

} // namespace otto
