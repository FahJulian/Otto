#include "window_settings_loader.h"

#include "otto/window/icon/icon_loader.h"

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
		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::deserialize(filePath);
			if (result.hasError())
				return WindowSettingsLoadingError::PARSING_ERROR;

			return _loadWindowSettingsFromSerialized(result.getResult());
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			BinaryFile file = BinaryFile(filePath);
			return _loadWindowSettingsFromBinotto(file);
		}
		else
		{
			return WindowSettingsLoadingError::UNKNOWN_FILETYPE;
		}
	}		
	
	Result<WindowSettings, WindowSettingsLoader::WindowSettingsLoadingError>
		WindowSettingsLoader::_loadWindowSettingsFromSerialized(const Serialized& serialized)
	{
		WindowSettings settings;

		if (!serialized.contains("title"))
			return WindowSettingsLoadingError::TITLE_MISSING;
		else
			settings.title = serialized.get<String>("title");

		if (serialized.contains("saveTitle"))
			settings.saveTitle = serialized.get<bool>("saveTitle");

		if (serialized.contains("clearColor"))
			settings.clearColor = serialized.get<Color>("clearColor");

		if (serialized.contains("saveClearColor"))
			settings.saveClearColor = serialized.get<bool>("saveClearColor");

		if (!serialized.contains("size"))
		{
			if (!serialized.contains("width") || !serialized.contains("height"))
				return WindowSettingsLoader::WindowSettingsLoadingError::SIZE_MISSING;

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
			settings.saveSize = serialized.get<bool>("saveSize");
		if (serialized.contains("windowMode"))
			settings.windowMode = _toWindowMode(serialized.get<String>("windowMode"));
		if (serialized.contains("saveWindowMode"))
			settings.saveWindowMode = serialized.get<bool>("saveWindowMode");
		if (serialized.contains("resizable"))
			settings.resizable = serialized.get<bool>("resizable");
		if (serialized.contains("closeButton"))
			settings.closeButton = serialized.get<bool>("closeButton");
		if (serialized.contains("closeOnAltF4"))
			settings.closeOnAltF4 = serialized.get<bool>("closeOnAltF4");
		if (serialized.contains("minimized"))
			settings.minimized = serialized.get<bool>("minimized");
		if (serialized.contains("maximized"))
			settings.maximized = serialized.get<bool>("maximized");
		if (serialized.contains("saveMinimized"))
			settings.saveMinimized = serialized.get<bool>("saveMinimized");
		if (serialized.contains("saveMaximized"))
			settings.saveMaximized = serialized.get<bool>("saveMaximized");

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

		settings.saveTitle = file.read<bool>();
		settings.saveSize = file.read<bool>();
		settings.saveWindowMode = file.read<bool>();
		settings.saveClearColor = file.read<bool>();

		settings.resizable = file.read<bool>();
		settings.closeButton = file.read<bool>();
		settings.closeOnAltF4 = file.read<bool>();

		settings.minimized = file.read<bool>();
		settings.maximized = file.read<bool>();
		settings.saveMinimized = file.read<bool>();
		settings.saveMaximized = file.read<bool>();

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
		if (settings.saveSize)
		{
			if (serialized.contains("size"))
				serialized.insert("size", Vec2i16(settings.unmaximizedWidth, settings.unmaximizedHeight));
			else
			{
				serialized.insert("width", settings.unmaximizedWidth);
				serialized.insert("beight", settings.unmaximizedHeight);
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

	/*
	static void loadJson(WindowInfo* info, InputFileStream file)
	{
		JSON json;

		try
		{
			file >> json;
		}
		catch (nlohmann::detail::parse_error)
		{
			// TODO: Print error
			return;
		}

		if (json.contains("size"))
		{
			JSON size = json["size"];
			if (size.is_array())
			{
				info->width = size[0].get<int>();
				info->height = size[1].get<int>();
			}
			else if (size.is_object())
			{
				info->width = size["width"].get<int>();
				info->height = size["height"].get<int>();
			}
			else if (size.is_string())
			{
				String sizeString = size.get<String>();
				size_t seperatorIndex = sizeString.find('x');

				info->width = std::stoi(sizeString, 0);
				info->height = std::stoi(sizeString.substr(seperatorIndex + 1), 0);
			}
		}

		if (json.contains("clearColor"))
		{
			JSON clearColor = json["clearColor"];
			if (clearColor.is_string())
			{
				String clearColorString = clearColor.get<String>();
				if (clearColorString.size() == 10)
					info->clearColor = std::stoul(clearColorString, nullptr, 16);
				else if (clearColorString.size() == 8)
					info->clearColor = std::stoul(clearColorString, nullptr, 16) | 0x000000ff;
			}
			else if (clearColor.is_array())
			{
				info->clearColor.r = clearColor[0].is_number_float() ? clearColor[0].get<float>() : clearColor[0].get<int>() / 255.0f;
				info->clearColor.g = clearColor[1].is_number_float() ? clearColor[1].get<float>() : clearColor[1].get<int>() / 255.0f;
				info->clearColor.b = clearColor[2].is_number_float() ? clearColor[2].get<float>() : clearColor[2].get<int>() / 255.0f;

				if (clearColor.size() > 3)
					info->clearColor.a = clearColor[3].is_number_float() ? clearColor[3].get<float>() : clearColor[3].get<int>() / 255.0f;
			}
			else
			{
				if (clearColor.contains("r"))
					info->clearColor.r = clearColor["r"].is_number_float() ? clearColor["r"].get<float>() : clearColor["r"].get<int>() / 255.0f;
				if (clearColor.contains("g"))
					info->clearColor.g = clearColor["g"].is_number_float() ? clearColor["g"].get<float>() : clearColor["g"].get<int>() / 255.0f;
				if (clearColor.contains("b"))
					info->clearColor.b = clearColor["b"].is_number_float() ? clearColor["b"].get<float>() : clearColor["b"].get<int>() / 255.0f;
				if (clearColor.contains("a"))
					info->clearColor.a = clearColor["a"].is_number_float() ? clearColor["a"].get<float>() : clearColor["a"].get<int>() / 255.0f;
			}
		}

		if (json.contains("title"))
			info->title = json["title"].get<String>();
		if (json.contains("mode"))
			info->mode = toWindowMode(json["mode"].get<String>());
		if (json.contains("saveTitle"))
			info->saveTitle = json["saveTitle"].get<bool>();
		if (json.contains("saveSize"))
			info->saveSize = json["saveSize"].get<bool>();
		if (json.contains("saveMode"))
			info->saveMode = json["saveMode"].get<bool>();
		if (json.contains("resizable"))
			info->resizable = json["resizable"].get<bool>();
		if (json.contains("closeButton"))
			info->closeButton = json["closeButton"].get<bool>();
		if (json.contains("closeOnAltF4"))
			info->closeOnAltF4 = json["closeOnAltF4"].get<bool>();
		if (json.contains("minimized"))
			info->minimized = json["minimized"].get<bool>();
		if (json.contains("maximized"))
			info->maximized = json["maximized"].get<bool>();
		if (json.contains("saveMinimized"))
			info->saveMinimized = json["saveMinimized"].get<bool>();
		if (json.contains("saveMaximized"))
			info->saveMaximized = json["saveMaximized"].get<bool>();

		std::vector<String> iconFilePaths;
		if (json.contains("iconSet") && json["iconSet"].is_array())
			for (JSON& iconFilePath : json["iconSet"])
				if (iconFilePath.is_string())
					iconFilePaths.push_back(iconFilePath.get<String>());

		String standardCursorSet = defaultStandardCursorSet;
		if (json.contains("standardCursorSet") && json["standardCursorSet"].is_string())
			String standardCursorSet = json["standardCursorSet"];

		std::unordered_map<String, String> cursorFilePaths;
		if (json.contains("cursorSet") && json["cursorSet"].is_object())
			for (auto& [cursorName, cursorFilePath] : json["cursorSet"].items())
				if (cursorFilePath.is_string())
					cursorFilePaths.emplace(cursorName, cursorFilePath.get<String>());

		info->cursorSet = Util::loadCursors(standardCursorSet, cursorFilePaths);
		info->iconSet = Util::loadIcons(iconFilePaths);
	}

	static void loadBinary(WindowInfo* info, Util::BinaryInputFileStream&& file)
	{
		uint16_t nameSize = file.read<uint16_t>();
		const char* name = file.read<char>(nameSize);
		info->title = String(name, nameSize);
		delete[] name;

		info->width = file.read<uint16_t>();
		info->height = file.read<uint16_t>();

		info->mode = file.read<WindowMode>();

		info->clearColor = file.read<uint32_t>();

		info->saveTitle = file.read<bool>();
		info->saveSize = file.read<bool>();
		info->saveMode = file.read<bool>();
		info->saveClearColor = file.read<bool>();
		info->resizable = file.read<bool>();
		info->closeButton = file.read<bool>();
		info->closeOnAltF4 = file.read<bool>();
		info->minimized = file.read<bool>();
		info->maximized = file.read<bool>();
		info->saveMinimized = file.read<bool>();
		info->saveMaximized = file.read<bool>();

		info->cursorSet = Util::loadCursors(file);
		info->iconSet = Util::loadIcons(file);
	}


	WindowInfo Util::loadWindowInfo(String filePath, bool overrideBinary)
	{
		WindowInfo info;

		size_t i_fileEnding = filePath.find('.');
		info.fileNamePrefix = i_fileEnding != String::npos ? filePath.substr(0, i_fileEnding) : filePath;

		if (!overrideBinary && std::filesystem::exists(info.fileNamePrefix + ".sonicwindow"))
		{
			info.isLoadedFromJson = false;
			loadBinary(&info, BinaryInputFileStream(info.fileNamePrefix + ".sonicwindow"));
		}
		else if (String resourceDirFilePath = resourceDir() + info.fileNamePrefix;
			!overrideBinary && std::filesystem::exists(resourceDirFilePath + ".sonicwindow"))
		{
			info.isLoadedFromJson = false;
			info.fileNamePrefix = resourceDirFilePath;
			loadBinary(&info, BinaryInputFileStream(info.fileNamePrefix + ".sonicwindow"));
		}
		else if (std::filesystem::exists(info.fileNamePrefix + ".sonicwindow.json"))
		{
			info.isLoadedFromJson = true;
			loadJson(&info, InputFileStream(info.fileNamePrefix + ".sonicwindow.json"));
		}
		else if (String resourceDirFilePath = resourceDir() + info.fileNamePrefix;
			std::filesystem::exists(resourceDirFilePath + ".sonicwindow.json"))
		{
			info.isLoadedFromJson = true;
			info.fileNamePrefix = resourceDirFilePath;
			loadJson(&info, InputFileStream(info.fileNamePrefix + ".sonicwindow.json"));
		}

		return info;
	}

	bool Util::saveWindowInfo(WindowInfo& info)
	{
		BinaryOutputFileStream file = BinaryOutputFileStream(info.fileNamePrefix + ".sonicwindow");

		uint16_t sizeOfName = (uint16_t)(info.title.size());
		file.write((const char*)&sizeOfName, sizeof(sizeOfName));
		file.write((const char*)info.title.c_str(), sizeOfName);

		uint16_t width = (uint16_t)info.width;
		file.write((const char*)&width, sizeof(width));

		uint16_t height = (uint16_t)info.height;
		file.write((const char*)&height, sizeof(height));

		file.write((const char*)&info.mode, sizeof(WindowMode));

		file.write<uint8_t>((uint8_t)(info.clearColor.a * 255.0f));
		file.write<uint8_t>((uint8_t)(info.clearColor.b * 255.0f));
		file.write<uint8_t>((uint8_t)(info.clearColor.g * 255.0f));
		file.write<uint8_t>((uint8_t)(info.clearColor.r * 255.0f));

		file.write((const char*)&info.saveTitle, sizeof(info.saveTitle));
		file.write((const char*)&info.saveSize, sizeof(info.saveSize));
		file.write((const char*)&info.saveMode, sizeof(info.saveMode));
		file.write((const char*)&info.saveClearColor, sizeof(info.saveClearColor));

		file.write((const char*)&info.resizable, sizeof(info.resizable));
		file.write((const char*)&info.closeButton, sizeof(info.closeButton));

		file.write((const char*)&info.closeOnAltF4, sizeof(info.closeOnAltF4));

		file.write((const char*)&info.minimized, sizeof(info.minimized));
		file.write((const char*)&info.maximized, sizeof(info.maximized));

		file.write((const char*)&info.saveMinimized, sizeof(info.saveMinimized));
		file.write((const char*)&info.saveMaximized, sizeof(info.saveMaximized));

		saveCursors(info.cursorSet, file);
		saveIcons(info.iconSet, file);

		return file.save();
	} */

} // namespace otto
