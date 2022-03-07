#pragma once

#include "otto/base.h"
#include "otto/util/file.h"
#include "otto/util/result.h"
#include "otto/util/file_path.h"
#include "otto/window/window_settings.h"
#include "otto/serialization/serialization.h"

namespace otto
{
	class WindowSettingsLoader
	{
	private:
		WindowSettingsLoader() = delete;
		WindowSettingsLoader(const WindowSettingsLoader& other) = delete;

	public:
		enum class WindowSettingsLoadingError
		{
			UNKNOWN_FILETYPE,
			PARSING_ERROR,
			TITLE_MISSING,
			SIZE_MISSING,
		};

		static Result<WindowSettings, WindowSettingsLoadingError> loadWindowSettings(const FilePath& filePath);
		static void saveWindowSettingsToBinotto(const WindowSettings& settings, BinaryFile& file);
		static void saveWindowSettingsToOtto(const WindowSettings& settings, Serialized initialSettings, const FilePath& filePath);

	private:
		static Result<WindowSettings, WindowSettingsLoader::WindowSettingsLoadingError>
			_loadWindowSettingsFromSerialized(const Serialized& serialized);

		static WindowSettings _loadWindowSettingsFromBinotto(BinaryFile& file);

		friend class Window;
	};

} // namespace otto
