#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/util/file.h"
#include "otto/core/application.h"
#include "otto/window/icon/icon.h"
#include "otto/window/icon/cursor.h"

namespace otto {

	class IconLoader 
	{
	public:
		enum IconLoadingError
		{
			UNKNOWN_FILETYPE,
			OTTO_FILE_PARSING_ERROR,
			OTTO_FILE_FILEPATH_MISSING,
			OTTO_FILE_HOTSPOT_MISSING,
		};

		static Result<Icon, IconLoadingError> loadIcon(const FilePath& filePath);
		static Result<Cursor, IconLoadingError> loadCursor(const FilePath& filePath);

		static Result<Map<String, Icon>, IconLoadingError> loadIconSet(const FilePath& filePath);
		static Result<Map<String, Cursor>, IconLoadingError> loadCursorSet(const FilePath& filePath);

		static void saveIcon(const Icon& icon, const FilePath& filePath);
		static void saveCursor(const Cursor& cursor, const FilePath& filePath);

		static void saveIconSet(const Map<String, Icon>& icons, const FilePath& filePath);
		static void saveCursorSet(const Map<String, Cursor>& cursors, const FilePath& filePath);

		static void destroy();
	};

} // namespace otto
