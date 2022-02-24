#include "icon_loader.h"

#include <filesystem>

#include <stb/stb_image.h>

#include "otto/debug/log/log.h"
#include "otto/serialization/serialization.h"
//#include "Sonic/Window/Window.h"

namespace otto
{
	namespace
	{
		static const size_t ICO_CUR_HEADER_SIZE = 22;

		static DynamicArray<uint8*> sLoadedBitmaps;

		Icon _loadIconFromBinotto(const FilePath& filePath)
		{
			BinaryFile file = BinaryFile(filePath);

			Icon icon;
			icon.width = file.read<uint8>();
			icon.height = file.read<uint8>();

			icon.bitmap = new uint8[4 * icon.width * icon.height];
			sLoadedBitmaps.add(icon.bitmap);

			file.readCompressed(icon.bitmap, 4 * icon.width * icon.height);

			return icon;
		}

		Icon _loadIconFromPng(const FilePath& filePath)
		{
			stbi_set_flip_vertically_on_load(1);

			Icon icon;
			uint32 width;
			uint32 height;

			uint8* flippedBitmap = stbi_load(filePath.toString().getData(), 
				reinterpret_cast<int*>(&width), 
				reinterpret_cast<int*>(&height), 
				NULL, 
				4
			);

			icon.width = static_cast<uint8>(width);
			icon.height = static_cast<uint8>(height);

			icon.bitmap = new uint8[4 * icon.width * icon.height];
			sLoadedBitmaps.add(icon.bitmap);

			// Flip bitmap
			for (uint64 i = 0; i < icon.width * icon.height; i++)
			{
				icon.bitmap[4 * i + 0] = flippedBitmap[4 * i + 2];
				icon.bitmap[4 * i + 1] = flippedBitmap[4 * i + 1];
				icon.bitmap[4 * i + 2] = flippedBitmap[4 * i + 0];
				icon.bitmap[4 * i + 3] = flippedBitmap[4 * i + 3];
			}

			delete[] flippedBitmap;

			return icon;
		}

		Icon _loadIconFromIco(const FilePath& filePath)
		{
			Icon icon;
			BinaryFile file = BinaryFile(filePath);

			file.moveCursor(4);

			uint16 iconAmount = file.read<uint16>();
			if (iconAmount != 1)
				Log::warn("Icon file ", filePath, " contains multiple images. Loading only the first one.");

			icon.width = file.read<uint8>();
			icon.height = file.read<uint8>();

			if (icon.width == 0)
				icon.width = 256;
			if (icon.height == 0)
				icon.height = 256;

			file.moveCursor(10);

			uint64 bitmapOffset = static_cast<uint16>(file.read<uint32_t>());
			file.moveCursor(bitmapOffset - ICO_CUR_HEADER_SIZE + uint64(14));

			uint16 bitsPerPixel = file.read<uint16>();
			OTTO_ASSERT(bitsPerPixel == 32, "Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");

			file.moveCursor(4);

			uint32 bitmapSize = file.read<uint32>();

			file.moveCursor(16);

			icon.bitmap = new uint8[bitmapSize];
			sLoadedBitmaps.add(icon.bitmap);

			file.read(icon.bitmap, bitmapSize);

			return icon;
		}

		Result<Cursor, IconLoader::IconLoadingError> _loadCursorFromSerialized(const Serialized& serialized)
		{
			if (!serialized.contains("FilePath"))
				return IconLoader::IconLoadingError::OTTO_FILE_FILEPATH_MISSING;
			else if (!serialized.contains("Hotspot"))
				return IconLoader::IconLoadingError::OTTO_FILE_HOTSPOT_MISSING;

			auto [hotspotX, hotspotY] = serialized.get<Vec2<uint16>>("Hotspot");
			auto icon = IconLoader::loadIcon(serialized.get<String>("filePath"));

			if (icon.hasError())
				return icon.getError();
			else
				return Cursor{ icon.getResult().width, icon.getResult().height, hotspotX, hotspotY, icon.getResult().bitmap };
		}

		Cursor _loadCursorFromBinotto(const FilePath& filePath)
		{
			BinaryFile file = BinaryFile(filePath);

			Cursor cursor;
			cursor.width = file.read<uint8>();
			cursor.height = file.read<uint8>();
			cursor.hotspotX = file.read<uint16>();
			cursor.hotspotY = file.read<uint16>();

			cursor.bitmap = new uint8[4 * cursor.width * cursor.height];
			sLoadedBitmaps.add(cursor.bitmap);

			file.readCompressed(cursor.bitmap, 4 * cursor.width * cursor.height);

			return cursor;
		}

		Cursor _loadCursorFromCur(const FilePath& filePath)
		{
			Cursor cursor;
			BinaryFile file = BinaryFile(filePath);

			file.moveCursor(4);

			uint16 cursorAmount = file.read<uint16>();
			if (cursorAmount != 1)
				Log::warn("Cursor file ", filePath, " contains multiple images. Loading only the first one");

			cursor.width = file.read<uint8>();
			cursor.height = file.read<uint8>();

			if (cursor.width == 0)
				cursor.width = 256;
			if (cursor.height == 0)
				cursor.height = 256;

			file.moveCursor(2);

			cursor.hotspotX = file.read<uint16>();
			cursor.hotspotY = file.read<uint16>();

			file.moveCursor(4);

			uint64 bitmapOffset = static_cast<uint64>(file.read<uint32_t>());
			file.moveCursor(bitmapOffset - ICO_CUR_HEADER_SIZE + uint64(14));

			uint16 bitsPerPixel = file.read<uint16>();
			OTTO_ASSERT(bitsPerPixel == 32, "Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");

			file.moveCursor(4);

			uint32 bitmapSize = file.read<uint32>();

			file.moveCursor(16);

			cursor.bitmap = new uint8[bitmapSize];
			file.read(cursor.bitmap, bitmapSize);

			return cursor;
		}

		Map<String, Icon> _loadIconSetFromBinotto(const FilePath& filePath)
		{
			BinaryFile file = BinaryFile(filePath);

			Map<String, Icon> icons;
			uint16 iconAmount = file.read<uint16>();
			for (uint16 i = 0; i < iconAmount; i++)
			{
				Icon icon;
				String name; 

				uint16 iconNameSize = file.read<uint16>();
				char8* iconNameData = new char8[iconNameSize];
				file.read(reinterpret_cast<uint8*>(iconNameData), iconNameSize);

				name = String(iconNameData, iconNameSize);
				delete[] iconNameData;

				icon.width = file.read<uint8>();
				icon.height = file.read<uint8>();
				icon.bitmap = new uint8[4 * icon.width * icon.height];
				sLoadedBitmaps.add(icon.bitmap);

				file.readCompressed(icon.bitmap, 4 * icon.width * icon.height);
				icons.insert(name, icon);
			}

			return icons;
		}

		Map<String, Cursor> _loadCursorSetFromBinotto(const FilePath& filePath)
		{
			BinaryFile file = BinaryFile(filePath);

			Map<String, Cursor> cursors;
			uint16 cursorAmount = file.read<uint16>();
			for (uint16 i = 0; i < cursorAmount; i++)
			{
				Cursor cursor;
				String name;

				uint16 cursorNameSize = file.read<uint16>();
				char8* cursorNameData = new char8[cursorNameSize];
				file.read(reinterpret_cast<uint8*>(cursorNameData), cursorNameSize);

				name = String(cursorNameData, cursorNameSize);
				delete[] cursorNameData;

				cursor.width = file.read<uint8>();
				cursor.height = file.read<uint8>();
				cursor.hotspotX = file.read<uint16>();
				cursor.hotspotY = file.read<uint16>();

				cursor.bitmap = new uint8[4 * cursor.width * cursor.height];
				sLoadedBitmaps.add(cursor.bitmap);

				file.readCompressed(cursor.bitmap, 4 * cursor.width * cursor.height);
				cursors.insert(name, cursor);
			}

			return cursors;
		}

	} // namespace 

	Result<Icon, IconLoader::IconLoadingError> IconLoader::loadIcon(const FilePath& filePath)
	{
		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::deserialize(filePath);
			if (result.hasError())
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;
			else if (!result.getResult().contains("FilePath"))
				return IconLoadingError::OTTO_FILE_FILEPATH_MISSING;
			else
				return loadIcon(result.getResult().get<String>("filePath"));
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			return _loadIconFromBinotto(filePath);
		}
		else if (filePath.toString().endsWith(".png"))
		{
			return _loadIconFromPng(filePath);
		}
		else if (filePath.toString().endsWith(".ico"))
		{
			return _loadIconFromIco(filePath);
		}
		else
		{
			return IconLoadingError::UNKNOWN_FILETYPE;
		}
	}

	Result<Cursor, IconLoader::IconLoadingError> IconLoader::loadCursor(const FilePath& filePath)
	{
		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::deserialize(filePath);
			if (result.hasError())
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;
			else
				return _loadCursorFromSerialized(result.getResult());
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			return _loadCursorFromBinotto(filePath);
		}
		else if (filePath.toString().endsWith(".cur"))
		{
			return _loadCursorFromCur(filePath);
		}
		else
		{
			return IconLoadingError::UNKNOWN_FILETYPE;
		}
	}

	Result<Map<String, Icon>, IconLoader::IconLoadingError> IconLoader::loadIconSet(const FilePath& filePath)
	{
		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::deserialize(filePath);
			if (result.hasError())
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;

			Map<String, Icon> icons;
			for (auto& [name, filePath] : result.getResult().getDictionary())
			{
				auto icon = loadIcon(filePath.toString().startsWith("otto/") ? 
					Application::getCoreRootDirectory() + "res/" + filePath.get<String>() : Application::getRootDirectory() + filePath.get<String>());

				if (icon.hasError())
					return icon.getError();

				icons.insert(name, icon.getResult());
			}

			return icons;
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			return _loadIconSetFromBinotto(filePath);
		}
		else
		{
			return IconLoadingError::UNKNOWN_FILETYPE;
		}
	}

	Result<Map<String, Cursor>, IconLoader::IconLoadingError> IconLoader::loadCursorSet(const FilePath& filePath)
	{
		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::deserialize(filePath);
			if (result.hasError())
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;

			Map<String, Cursor> cursors;
			for (auto& [name, serialized] : result.getResult().getDictionary())
			{
				if (serialized.isValue())
				{
					auto cursor = loadCursor(filePath.toString().startsWith("otto/") ?
						Application::getCoreRootDirectory() + "res/" + serialized.get<String>() : Application::getRootDirectory() + serialized.get<String>());

					if (cursor.hasError())
						return cursor.getError();

					cursors.insert(name, cursor.getResult());
				}
				else if (serialized.isDictionary())
				{
					auto result = _loadCursorFromSerialized(serialized);
					if (result.hasError())
						return result.getError();
					else
						cursors.insert(name, result.getResult());
				}
			}

			return cursors;
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			return _loadCursorSetFromBinotto(filePath);
		}
		else
		{
			return IconLoadingError::UNKNOWN_FILETYPE;
		}
	}

	void IconLoader::saveIcon(const Icon& icon, const FilePath& filePath)
	{
		BinaryFile file = BinaryFile(filePath);

		file.write(icon.width);
		file.write(icon.height);
		file.writeCompressed(icon.bitmap, 4 * icon.width * icon.height);
	}

	void IconLoader::saveCursor(const Cursor& cursor, const FilePath& filePath)
	{
		BinaryFile file = BinaryFile(filePath);

		file.write(cursor.width);
		file.write(cursor.height);
		file.write(cursor.hotspotX);
		file.write(cursor.hotspotY);
		file.writeCompressed(cursor.bitmap, 4 * cursor.width * cursor.height);
	}

	void IconLoader::saveIconSet(const Map<String, Icon>& icons, const FilePath& filePath)
	{
		BinaryFile file = BinaryFile(filePath);

		file.write(static_cast<uint16>(icons.getSize()));
		for (auto& [name, icon] : icons)
		{
			file.write(static_cast<uint16>(name.getSize()));
			file.write(reinterpret_cast<const uint8*>(name.getData()), name.getSize());

			file.write(icon.width);
			file.write(icon.height);
			file.writeCompressed(icon.bitmap, 4 * icon.width * icon.height);
		}
	}

	void IconLoader::saveCursorSet(const Map<String, Cursor>& cursors, const FilePath& filePath)
	{
		BinaryFile file = BinaryFile(filePath);

		file.write(static_cast<uint16>(cursors.getSize()));
		for (auto& [name, cursor] : cursors)
		{
			file.write(static_cast<uint16>(name.getSize()));
			file.write(reinterpret_cast<const uint8*>(name.getData()), name.getSize());

			file.write(cursor.width);
			file.write(cursor.height);
			file.write(cursor.hotspotX);
			file.write(cursor.hotspotY);
			file.writeCompressed(cursor.bitmap, 4 * cursor.width * cursor.height);
		}
	}

	void IconLoader::destroy()
	{
		for (uint8* bitmap : sLoadedBitmaps)
			delete[] bitmap;

		sLoadedBitmaps.clear();
	}


/*
	Result<Map<String, Cursor>, IconLoader::IconLoadingError> IconLoader::loadCursorSet(const FilePath& filePath)
	{
		// Check if binotto file or otto file

		Map<String, Cursor> cursors;

		uint64 cursorAmount = static_cast<uint64>(file.read<uint16>());

		for (uint64 i = 0, cursorAmount = file.read<uint16_t>(); i < cursorAmount; i++)
		{
			CursorInfo cursor;

			uint16_t nameSize = file.read<uint16_t>();
			const char* name = file.read<char>(nameSize);

			cursor.width = file.read<uint16_t>();
			cursor.height = file.read<uint16_t>();
			cursor.hotspotX = file.read<uint16_t>();
			cursor.hotspotY = file.read<uint16_t>();

			cursor.bitmap = file.readCompressed<uint8_t>(4 * cursor.width * cursor.height);

			cursors.emplace(String(name, nameSize), cursor);
			delete[] name;
		}

		return cursors;
	}

	std::unordered_map<String, CursorInfo> Util::loadCursors(const String& standardCursorSet, const std::unordered_map<String, String> filePaths)
	{
		std::unordered_map<String, CursorInfo> cursors;

		for (auto& [name, filePath] : filePaths)
		{
			if (Util::endsWith(filePath, ".cur"))
			{
				if (std::filesystem::exists(filePath))
				{
					cursors.emplace(name, loadCursorFromCur(filePath));
				}
				else if (String resourceDirFilePath = resourceDir() + filePath;
					std::filesystem::exists(resourceDirFilePath))
				{
					cursors.emplace(name, loadCursorFromCur(resourceDirFilePath));
				}
			}
		}

		String standardCursorsDir = standardCursorSetDirs.find(standardCursorSet) != standardCursorSetDirs.end() ?
			standardCursorSetDirs.at(standardCursorSet) : standardCursorSetDirs.at(defaultStandardCursorSet);

		for (auto& standardCursor : allStandardCursors)
		{
			if (cursors.find(standardCursor) == cursors.end())
			{
				String filePath = standardCursorsDir + standardCursor + ".cur";
				cursors.emplace(standardCursor, loadCursorFromCur(filePath));
			}
		}

		return cursors;
	}

	std::vector<IconInfo> Util::loadIcons(BinaryInputFileStream& file)
	{
		std::vector<IconInfo> icons;

		for (int i = 0, iconAmount = file.read<uint16_t>(); i < iconAmount; i++)
		{
			IconInfo icon;

			icon.width = file.read<uint16_t>();
			icon.height = file.read<uint16_t>();
			icon.bitmap = file.readCompressed<uint8_t>(4 * icon.width * icon.height);

			icons.push_back(icon);
		}

		return icons;
	}

	std::vector<IconInfo> Util::loadIcons(std::vector<String>& filePaths)
	{
		std::vector<IconInfo> icons;

		if (filePaths.size() == 0)
			filePaths = defaultIconFilePaths;

		for (auto& filePath : filePaths)
		{
			if (Util::endsWith(filePath, ".ico"))
			{
				if (std::filesystem::exists(filePath))
				{
					icons.push_back(loadIconFromIco(filePath));
				}
				else if (String resourceDirFilePath = resourceDir() + filePath;
					std::filesystem::exists(resourceDirFilePath))
				{
					icons.push_back(loadIconFromIco(resourceDirFilePath));
				}
			}
			else if (Util::endsWith(filePath, ".png"))
			{
				if (std::filesystem::exists(filePath))
				{
					icons.push_back(loadIconFromPng(filePath));
				}
				else if (String resourceDirFilePath = resourceDir() + filePath;
					std::filesystem::exists(resourceDirFilePath))
				{
					icons.push_back(loadIconFromPng(resourceDirFilePath));
				}
			}
		}

		return icons;
	}

	void Util::saveCursors(std::unordered_map<String, CursorInfo>& cursors, BinaryOutputFileStream& file)
	{
		file.write<uint16_t>((uint16_t)cursors.size());

		for (auto& [cursorName, cursor] : cursors)
		{
			file.write<uint16_t>((uint16_t)cursorName.size());
			file.write(cursorName.c_str(), cursorName.size());

			file.write<uint16_t>(cursor.width);
			file.write<uint16_t>(cursor.height);
			file.write<uint16_t>(cursor.hotspotX);
			file.write<uint16_t>(cursor.hotspotY);

			file.writeCompressed(cursor.bitmap, 4 * cursor.width * cursor.height);

			delete[] cursor.bitmap;
		}

		cursors.clear();
	}

	void Util::saveIcons(std::vector<IconInfo>& icons, BinaryOutputFileStream& file)
	{
		file.write<uint16_t>((uint16_t)icons.size());

		for (auto& icon : icons)
		{
			file.write<uint16_t>(icon.width);
			file.write<uint16_t>(icon.height);
			file.writeCompressed(icon.bitmap, 4 * icon.width * icon.height);

			delete[] icon.bitmap;
		}

		icons.clear();
	}

	static CursorInfo loadCursorFromCur(Util::BinaryInputFileStream&& file)
	{
		CursorInfo cursor;

		file.moveCursor(4);

		uint16_t cursorAmount = file.read<uint16_t>();
		if (cursorAmount != 1)
			SONIC_LOG_WARN("Cursor file contains multiple images. Loading only the first one");

		cursor.width = file.read<uint8_t>();
		cursor.height = file.read<uint8_t>();

		if (cursor.width == 0)
			cursor.width = 256;
		if (cursor.height == 0)
			cursor.height = 256;

		file.moveCursor(2);

		cursor.hotspotX = file.read<uint16_t>();
		cursor.hotspotY = file.read<uint16_t>();

		file.moveCursor(4);

		size_t bitmapOffset = (size_t)file.read<uint32_t>();
		file.moveCursor(bitmapOffset - ICO_CUR_HEADER_SIZE + (size_t)14);

		uint16_t bitsPerPixel = file.read<uint16_t>();
		SONIC_LOG_DEBUG_ASSERT(bitsPerPixel == 32, "Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");

		file.moveCursor(4);

		uint32_t bitmapSize = file.read<uint32_t>();

		file.moveCursor(16);

		cursor.bitmap = file.read<uint8_t>(bitmapSize);

		return cursor;
	}

	static IconInfo loadIconFromIco(Util::BinaryInputFileStream&& file)
	{
		IconInfo icon;

		file.moveCursor(4);

		uint16_t iconAmount = file.read<uint16_t>();
		if (iconAmount != 1)
			SONIC_LOG_WARN("Icon file contains multiple images. Loading only the first one");

		icon.width = file.read<uint8_t>();
		icon.height = file.read<uint8_t>();

		if (icon.width == 0)
			icon.width = 256;
		if (icon.height == 0)
			icon.height = 256;

		file.moveCursor(10);

		size_t bitmapOffset = (size_t)file.read<uint32_t>();
		file.moveCursor(bitmapOffset - ICO_CUR_HEADER_SIZE + (size_t)14);

		uint16_t bitsPerPixel = file.read<uint16_t>();
		SONIC_LOG_DEBUG_ASSERT(bitsPerPixel == 32, "Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");

		file.moveCursor(4);

		uint32_t bitmapSize = file.read<uint32_t>();

		file.moveCursor(16);

		icon.bitmap = file.read<uint8_t>(bitmapSize);

		return icon;
	}

	static IconInfo loadIconFromPng(const String& file)
	{
		stbi_set_flip_vertically_on_load(1);

		IconInfo icon;

		uint8_t* tmpBitmap = stbi_load(file.c_str(), &icon.width, &icon.height, NULL, 4);

		icon.bitmap = new uint8_t[4 * icon.width * icon.height];
		for (int i = 0; i < icon.width * icon.height; i++)
		{
			icon.bitmap[4 * i + 0] = tmpBitmap[4 * i + 2];
			icon.bitmap[4 * i + 1] = tmpBitmap[4 * i + 1];
			icon.bitmap[4 * i + 2] = tmpBitmap[4 * i + 0];
			icon.bitmap[4 * i + 3] = tmpBitmap[4 * i + 3];
		}

		delete[] tmpBitmap;

		return icon;
	}*/

} // namespace otto
