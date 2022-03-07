#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/util/file.h"
#include "otto/core/application.h"
#include "otto/window/icon/icon.h"
#include "otto/window/icon/cursor.h"
#include "otto/serialization/serialization.h"

namespace otto {

	class IconLoader 
	{
	private:
		IconLoader() = delete;
		IconLoader(const IconLoader& other) = delete;

	public:
		enum class IconLoadingError
		{
			UNKNOWN_FILETYPE,
			OTTO_FILE_PARSING_ERROR,
			OTTO_FILE_FILEPATH_MISSING,
			OTTO_FILE_HOTSPOT_MISSING,
			INVALID_BITMAP,
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

	private:
		static Result<Map<String, Icon>, IconLoadingError> _loadIconSetFromSerialized(const Serialized& serialized);
		static Result<Map<String, Cursor>, IconLoadingError> _loadCursorSetFromSerialized(const Serialized& serialized);
		static Map<String, Icon> _loadIconSetFromBinotto(BinaryFile& file);
		static Map<String, Cursor> _loadCursorSetFromBinotto(BinaryFile& file);

		static void _saveIconSetToBinotto(const Map<String, Icon>& icons, BinaryFile& file);
		static void _saveCursorSetToBinotto(const Map<String, Cursor>& cursors, BinaryFile& file);

		friend class WindowSettingsLoader;
	};

} // namespace otto
