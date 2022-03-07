#include "window_settings_loader.h"

#include "otto/serialization/icon_loader.h"

namespace otto
{
	namespace
	{
		static WindowMode _toWindowMode(const String& name)
		{
			if (name == "Fullscreen")
				return WindowMode::FULLSCREEN;
			else if (name == "WindowedBorderless")
				return WindowMode::WINDOWED_BORDERLESS;
			else if (name == "WindowedFullscreen")
				return WindowMode::WINDOWED_FULLSCREEN;
			else
				return WindowMode::WINDOWED;
		}

	} // namespace

	Result<WindowSettings, WindowSettingsLoader::WindowSettingsLoadingError>
		WindowSettingsLoader::loadWindowSettings(const FilePath& filePath)
	{
		Log::trace("Loading window settings ", filePath, "...");

		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::load(filePath);
			if (result.hasError())
			{
				Log::error("Failed to load window settings: Syntax error.");
				return WindowSettingsLoadingError::PARSING_ERROR;
			}

			return _loadWindowSettingsFromSerialized(result.getResult());
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			BinaryFile file = BinaryFile(filePath);
			return _loadWindowSettingsFromBinotto(file);
		}
		else
		{
			Log::error("Failed to load window setttings: Unknown filetype.");
			return WindowSettingsLoadingError::UNKNOWN_FILETYPE;
		}
	}		
	
	Result<WindowSettings, WindowSettingsLoader::WindowSettingsLoadingError>
		WindowSettingsLoader::_loadWindowSettingsFromSerialized(const Serialized& serialized)
	{
		WindowSettings settings;

		if (!serialized.contains("title"))
		{
			Log::error("Failed to load Window Settings: Title missing");
			return WindowSettingsLoadingError::TITLE_MISSING;
		}
		else
			settings.title = serialized.get<String>("title");

		if (serialized.contains("saveTitle"))
			settings.saveTitle = serialized.get<bool8>("saveTitle");

		if (serialized.contains("clearColor"))
			settings.clearColor = serialized.get<Color>("clearColor");

		if (serialized.contains("saveClearColor"))
			settings.saveClearColor = serialized.get<bool8>("saveClearColor");

		if (!serialized.contains("size"))
		{
			if (!serialized.contains("width") || !serialized.contains("height"))
			{
				Log::error("Failed to load Window Settings: Size missing");
				return WindowSettingsLoader::WindowSettingsLoadingError::SIZE_MISSING;
			}

			settings.width = serialized.get<uint32>("width");
			settings.height = serialized.get<uint32>("height");
		}
		else
		{
			Vec2i32 size = serialized.get<Vec2i32>("size");
			settings.width = size.x;
			settings.height = size.y;
		}

		if (serialized.contains("saveSize"))
			settings.saveSize = serialized.get<bool8>("saveSize");
		if (serialized.contains("windowMode"))
			settings.windowMode = _toWindowMode(serialized.get<String>("windowMode"));
		if (serialized.contains("saveWindowMode"))
			settings.saveWindowMode = serialized.get<bool8>("saveWindowMode");
		if (serialized.contains("resizable"))
			settings.resizable = serialized.get<bool8>("resizable");
		if (serialized.contains("closeButton"))
			settings.closeButton = serialized.get<bool8>("closeButton");
		if (serialized.contains("closeOnAltF4"))
			settings.closeOnAltF4 = serialized.get<bool8>("closeOnAltF4");
		if (serialized.contains("minimized"))
			settings.minimized = serialized.get<bool8>("minimized");
		if (serialized.contains("maximized"))
			settings.maximized = serialized.get<bool8>("maximized");
		if (serialized.contains("saveMinimized"))
			settings.saveMinimized = serialized.get<bool8>("saveMinimized");
		if (serialized.contains("saveMaximized"))
			settings.saveMaximized = serialized.get<bool8>("saveMaximized");

		if (serialized.contains("iconSet"))
		{
			auto result = IconLoader::_loadIconSetFromSerialized(serialized.get("iconSet"));
			if (!result.hasError())
				settings.iconSet = result.getResult();
		}

		if (serialized.contains("cursorSet"))
		{
			auto result = IconLoader::_loadCursorSetFromSerialized(serialized.get("cursorSet"));
			if (!result.hasError())
				settings.cursorSet = result.getResult();
		}

		if (serialized.contains("defaultCursor"))
			settings.defaultCursor = serialized.get<String>("defaultCursor");

		return settings;
	}

	WindowSettings WindowSettingsLoader::_loadWindowSettingsFromBinotto(BinaryFile& file)
	{
		WindowSettings settings;

		uint64 titleSize = static_cast<uint64>(file.read<uint16>());
		char8* titleData = new char8[titleSize];
		file.read(reinterpret_cast<uint8*>(titleData), titleSize);
		settings.title = String(titleData, titleSize);
		delete[] titleData;

		settings.width = file.read<uint16>();
		settings.height = file.read<uint16>();
		settings.windowMode = file.read<WindowMode>();
		settings.clearColor = file.read<uint32>();

		settings.saveTitle = file.read<bool8>();
		settings.saveSize = file.read<bool8>();
		settings.saveWindowMode = file.read<bool8>();
		settings.saveClearColor = file.read<bool8>();

		settings.resizable = file.read<bool8>();
		settings.closeButton = file.read<bool8>();
		settings.closeOnAltF4 = file.read<bool8>();

		settings.minimized = file.read<bool8>();
		settings.maximized = file.read<bool8>();
		settings.saveMinimized = file.read<bool8>();
		settings.saveMaximized = file.read<bool8>();

		settings.iconSet = IconLoader::_loadIconSetFromBinotto(file);
		settings.cursorSet = IconLoader::_loadCursorSetFromBinotto(file);

		uint64 defaultCursorSize = static_cast<uint64>(file.read<uint16>());
		char8* defaultCursorData = new char8[defaultCursorSize];
		file.read(reinterpret_cast<uint8*>(defaultCursorData), defaultCursorSize);
		settings.defaultCursor = String(defaultCursorData, defaultCursorSize);
		delete[] defaultCursorData;

		return settings;
	}

	void WindowSettingsLoader::saveWindowSettingsToBinotto(const WindowSettings& settings, BinaryFile& file)
	{
		Log::trace("Saving window settings to ", file.getFilePath(), "...");

		file.write(static_cast<uint16>(settings.title.getSize()));
		file.write(reinterpret_cast<const uint8*>(settings.title.getData()), settings.title.getSize());
		file.write(settings.width);
		file.write(settings.height);
		file.write(settings.windowMode);
		file.write(settings.clearColor.toColorCode());
		file.write(settings.saveTitle);
		file.write(settings.saveSize);
		file.write(settings.saveWindowMode);
		file.write(settings.saveClearColor);
		file.write(settings.resizable);
		file.write(settings.closeButton);
		file.write(settings.closeOnAltF4);
		file.write(settings.minimized);
		file.write(settings.maximized);
		file.write(settings.saveMinimized);
		file.write(settings.saveMaximized);

		IconLoader::_saveIconSetToBinotto(settings.iconSet, file);
		IconLoader::_saveCursorSetToBinotto(settings.cursorSet, file);

		file.write(static_cast<uint16>(settings.defaultCursor.getSize()));
		file.write(reinterpret_cast<const uint8*>(settings.defaultCursor.getData()), settings.defaultCursor.getSize());
	}

	void WindowSettingsLoader::saveWindowSettingsToOtto(const WindowSettings& settings, Serialized serialized, const FilePath& filePath)
	{
		Log::trace("Saving window settings to ", filePath, "...");

		if (settings.saveSize)
		{
			if (serialized.contains("size"))
				serialized.insert("size", Vec2i16(settings.unmaximizedWidth, settings.unmaximizedHeight));
			else
			{
				serialized.insert("width", settings.unmaximizedWidth);
				serialized.insert("height", settings.unmaximizedHeight);
			}
		}

		if (settings.saveTitle)
			serialized.insert("title", settings.title);
		if (settings.saveWindowMode)
			serialized.insert("windowMode", uint8(settings.windowMode));
		if (settings.saveClearColor)
			serialized.insert("clearColor", settings.clearColor);
		if (settings.saveMaximized)
			serialized.insert("maximized", settings.maximized);
		if (settings.saveMinimized)
			serialized.insert("minimized", settings.minimized);

		File(filePath).write(serialized.toString());
	}

} // namespace otto
