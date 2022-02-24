#pragma once

#include "otto/base.h"

namespace otto {

	struct Cursor
	{
		uint8 width;
		uint8 height;
		uint16 hotspotX;
		uint16 hotspotY;
		uint8* bitmap;
	};

} // namespace otto
