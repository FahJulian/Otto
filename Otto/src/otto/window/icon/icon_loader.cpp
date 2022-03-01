#include "icon_loader.h"

#include <filesystem>

#include <stb/stb_image.h>

#include "otto/debug/log/log.h"
#include "otto/core/application.h"

namespace otto
{
	namespace
	{
		static const size_t ICO_CUR_HEADER_SIZE = 22;

		static DynamicArray<uint8*> sLoadedBitmaps;

		Icon _loadIconFromBinotto(BinaryFile& file)
		{
			Icon icon;
			icon.width = file.read<uint16>();
			icon.height = file.read<uint16>();

			icon.bitmap = new uint8[uint64(4) * uint64(icon.width) * uint64(icon.height)];
			sLoadedBitmaps.add(icon.bitmap);

			file.readCompressed(icon.bitmap, uint64(4) * uint64(icon.width) * uint64(icon.height));

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

			icon.width = static_cast<uint16>(width);
			icon.height = static_cast<uint16>(height);

			icon.bitmap = new uint8[uint64(4) * uint64(icon.width) * uint64(icon.height)];
			sLoadedBitmaps.add(icon.bitmap);

			// Flip bitmap
			for (uint64 i = 0; i < uint64(icon.width) * uint64(icon.height); i++)
			{
				icon.bitmap[4 * i + 0] = flippedBitmap[4 * i + 2];
				icon.bitmap[4 * i + 1] = flippedBitmap[4 * i + 1];
				icon.bitmap[4 * i + 2] = flippedBitmap[4 * i + 0];
				icon.bitmap[4 * i + 3] = flippedBitmap[4 * i + 3];
			}

			delete[] flippedBitmap;

			return icon;
		}

		Result<Icon, IconLoader::IconLoadingError> _loadIconFromIco(const FilePath& filePath)
		{
			Icon icon;
			BinaryFile file = BinaryFile(filePath);

			file.moveCursor(4);

			uint16 iconAmount = file.read<uint16>();
			if (iconAmount != 1)
				Log::warn("Icon file ", filePath, " contains multiple images. Loading only the first one.");

			icon.width = static_cast<uint16>(file.read<uint8>());
			icon.height = static_cast<uint16>(file.read<uint8>());

			if (icon.width == 0)
				icon.width = 256;
			if (icon.height == 0)
				icon.height = 256;

			file.moveCursor(10);

			uint64 bitmapOffset = static_cast<uint16>(file.read<uint32_t>());
			file.moveCursor(bitmapOffset - ICO_CUR_HEADER_SIZE + uint64(14));

			uint16 bitsPerPixel = file.read<uint16>();
			if (bitsPerPixel != 32)
			{
				Log::error("Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");
				return IconLoader::IconLoadingError::INVALID_BITMAP;
			}

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
			{
				Log::error("Failed to load cursor: FilePath missing.");
				return IconLoader::IconLoadingError::OTTO_FILE_FILEPATH_MISSING;
			}
			else if (!serialized.contains("Hotspot"))
			{
				Log::error("Failed to load cursor: Hotspot missing.");
				return IconLoader::IconLoadingError::OTTO_FILE_HOTSPOT_MISSING;
			}

			auto [hotspotX, hotspotY] = serialized.get<Vec2<uint16>>("Hotspot");
			auto icon = IconLoader::loadIcon(serialized.get<String>("filePath"));

			if (icon.hasError())
				return icon.getError();
			else
				return Cursor{ icon.getResult().width, icon.getResult().height, hotspotX, hotspotY, icon.getResult().bitmap };
		}

		Cursor _loadCursorFromBinotto(BinaryFile& file)
		{
			Cursor cursor;
			cursor.width = file.read<uint16>();
			cursor.height = file.read<uint16>();
			cursor.hotspotX = file.read<uint16>();
			cursor.hotspotY = file.read<uint16>();

			cursor.bitmap = new uint8[uint64(4) * uint64(cursor.width) * uint64(cursor.height)];
			sLoadedBitmaps.add(cursor.bitmap);

			file.readCompressed(cursor.bitmap, uint64(4) * uint64(cursor.width) * uint64(cursor.height));

			return cursor;
		}

		Result<Cursor, IconLoader::IconLoadingError> _loadCursorFromCur(const FilePath& filePath)
		{
			Cursor cursor;
			BinaryFile file = BinaryFile(filePath);

			file.moveCursor(4);

			uint16 cursorAmount = file.read<uint16>();
			if (cursorAmount != 1)
				Log::warn("Cursor file ", filePath, " contains multiple images. Loading only the first one");

			cursor.width = static_cast<uint16>(file.read<uint8>());
			cursor.height = static_cast<uint16>(file.read<uint8>());

			if (cursor.width == 0)
				cursor.width = 256;
			if (cursor.height == 0)
				cursor.height = 256;

			file.moveCursor(2);

			cursor.hotspotX = file.read<uint16>();
			cursor.hotspotY = file.read<uint16>();

			file.moveCursor(4);

			uint64 bitmapOffset = uint64(file.read<uint32_t>());
			file.moveCursor(bitmapOffset - ICO_CUR_HEADER_SIZE + uint64(14));

			uint16 bitsPerPixel = file.read<uint16>();
			if (bitsPerPixel != 32)
			{
				Log::error("Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");
				return IconLoader::IconLoadingError::INVALID_BITMAP;
			}

			file.moveCursor(4);

			uint32 bitmapSize = file.read<uint32>();

			file.moveCursor(16);

			cursor.bitmap = new uint8[bitmapSize];
			file.read(cursor.bitmap, bitmapSize);

			return cursor;
		}

	} // namespace 

	Result<Icon, IconLoader::IconLoadingError> IconLoader::loadIcon(const FilePath& filePath)
	{
		Log::trace("Loading icon ", filePath, "...");

		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::load(filePath);
			if (result.hasError())
			{
				Log::error("Failed to load icon: Syntax error");
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;
			}
			else if (!result.getResult().contains("filePath"))
			{
				Log::error("Failed to load icon: FilePath missing.");
				return IconLoadingError::OTTO_FILE_FILEPATH_MISSING;
			}
			else
				return loadIcon(result.getResult().get<String>("filePath"));
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			BinaryFile file = BinaryFile(filePath);
			return _loadIconFromBinotto(file);
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
		Log::trace("Loading cursor ", filePath, "...");

		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::load(filePath);
			if (result.hasError())
			{
				Log::error("Failed to load cursor: Syntax error");
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;
			}
			else
				return _loadCursorFromSerialized(result.getResult());
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			BinaryFile file = BinaryFile(filePath);
			return _loadCursorFromBinotto(file);
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
		Log::trace("Loading icon set ", filePath, "...");

		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::load(filePath);
			if (result.hasError())
			{
				Log::error("Failed to load icon set: Syntax error");
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;
			}

			return _loadIconSetFromSerialized(result.getResult());
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			BinaryFile file = BinaryFile(filePath);
			return _loadIconSetFromBinotto(file);
		}
		else
		{
			return IconLoadingError::UNKNOWN_FILETYPE;
		}
	}

	Result<Map<String, Cursor>, IconLoader::IconLoadingError> IconLoader::loadCursorSet(const FilePath& filePath)
	{
		Log::trace("Loading cursor set ", filePath, "...");

		if (filePath.toString().endsWith(".otto"))
		{
			auto result = Serializer::load(filePath);
			if (result.hasError())
			{
				Log::error("Failed to load icon set: Syntax error");
				return IconLoadingError::OTTO_FILE_PARSING_ERROR;
			}

			return _loadCursorSetFromSerialized(result.getResult());
		}
		else if (filePath.toString().endsWith(".binotto"))
		{
			BinaryFile file = BinaryFile(filePath);
			return _loadCursorSetFromBinotto(file);
		}
		else
		{
			return IconLoadingError::UNKNOWN_FILETYPE;
		}
	}

	void IconLoader::saveIcon(const Icon& icon, const FilePath& filePath)
	{
		Log::trace("Saving icon to ", filePath, "...");

		BinaryFile file = BinaryFile(filePath);

		file.write(icon.width);
		file.write(icon.height);
		file.writeCompressed(icon.bitmap, uint64(4) * uint64(icon.width) * uint64(icon.height));
	}

	void IconLoader::saveCursor(const Cursor& cursor, const FilePath& filePath)
	{
		Log::trace("Saving cursor to ", filePath, "...");

		BinaryFile file = BinaryFile(filePath);

		file.write(cursor.width);
		file.write(cursor.height);
		file.write(cursor.hotspotX);
		file.write(cursor.hotspotY);
		file.writeCompressed(cursor.bitmap, uint64(4) * uint64(cursor.width) * uint64(cursor.height));
	}

	void IconLoader::saveIconSet(const Map<String, Icon>& icons, const FilePath& filePath)
	{
		Log::trace("Saving icon set to ", filePath, "...");

		BinaryFile file = BinaryFile(filePath);
		_saveIconSetToBinotto(icons, file);
	}

	void IconLoader::saveCursorSet(const Map<String, Cursor>& cursors, const FilePath& filePath)
	{
		Log::trace("Saving cursor set to ", filePath, "...");

		BinaryFile file = BinaryFile(filePath);
		_saveCursorSetToBinotto(cursors, file);
	}

	void IconLoader::destroy()
	{
		Log::trace("Destroying IconLoader...");

		for (uint8* bitmap : sLoadedBitmaps)
			delete[] bitmap;

		sLoadedBitmaps.clear();
	}

	Result<Map<String, Icon>, IconLoader::IconLoadingError>
		IconLoader::_loadIconSetFromSerialized(const Serialized& serialized)
	{
		if (serialized.isValue())
		{
			String filePath = serialized.get<String>();
			return loadIconSet(filePath.startsWith("otto/") ? Application::getCoreRootDirectory() + "res/" + filePath
				: Application::getRootDirectory() + filePath);
		}

		Map<String, Icon> icons;
		for (auto& [name, filePath] : serialized.getDictionary())
		{
			auto icon = loadIcon(filePath.get<String>().startsWith("otto/") ?
				Application::getCoreRootDirectory() + "res/" + filePath.get<String>() : Application::getRootDirectory() + filePath.get<String>());

			if (icon.hasError())
				return icon.getError();

			icons.insert(name, icon.getResult());
		}

		return icons;
	}

	Result<Map<String, Cursor>, IconLoader::IconLoadingError>
		IconLoader::_loadCursorSetFromSerialized(const Serialized& serialized)
	{
		if (serialized.isValue())
		{
			String filePath = serialized.get<String>();
			return loadCursorSet(filePath.startsWith("otto/") ? Application::getCoreRootDirectory() + "res/" + filePath
				: Application::getRootDirectory() + filePath);
		}

		Map<String, Cursor> cursors;
		for (auto& [name, value] : serialized.getDictionary())
		{
			if (value.isValue())
			{
				auto cursor = loadCursor(value.get<String>().startsWith("otto/") ?
					Application::getCoreRootDirectory() + "res/" + value.get<String>() : Application::getRootDirectory() + value.get<String>());

				if (cursor.hasError())
					return cursor.getError();

				cursors.insert(name, cursor.getResult());
			}
			else if (value.isDictionary())
			{
				auto result = _loadCursorFromSerialized(value);
				if (result.hasError())
					return result.getError();
				else
					cursors.insert(name, result.getResult());
			}
		}

		return cursors;
	}

	Map<String, Icon> IconLoader::_loadIconSetFromBinotto(BinaryFile& file)
	{
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

			icon.width = file.read<uint16>();
			icon.height = file.read<uint16>();
			icon.bitmap = new uint8[uint64(4) * uint64(icon.width) * uint64(icon.height)];
			sLoadedBitmaps.add(icon.bitmap);

			file.readCompressed(icon.bitmap, uint64(4) * uint64(icon.width) * uint64(icon.height));
			icons.insert(name, icon);
		}

		return icons;
	}

	Map<String, Cursor> IconLoader::_loadCursorSetFromBinotto(BinaryFile& file)
	{
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

			cursor.width = file.read<uint16>();
			cursor.height = file.read<uint16>();
			cursor.hotspotX = file.read<uint16>();
			cursor.hotspotY = file.read<uint16>();

			cursor.bitmap = new uint8[uint64(4) * uint64(cursor.width) * uint64(cursor.height)];
			sLoadedBitmaps.add(cursor.bitmap);

			file.readCompressed(cursor.bitmap, uint64(4) * uint64(cursor.width) * uint64(cursor.height));
			cursors.insert(name, cursor);
		}

		return cursors;
	}

	void IconLoader::_saveIconSetToBinotto(const Map<String, Icon>& icons, BinaryFile& file)
	{
		file.write(static_cast<uint16>(icons.getSize()));
		for (auto& [name, icon] : icons)
		{
			file.write(static_cast<uint16>(name.getSize()));
			file.write(reinterpret_cast<const uint8*>(name.getData()), name.getSize());

			file.write(icon.width);
			file.write(icon.height);
			file.writeCompressed(icon.bitmap, uint64(4) * uint64(icon.width) * uint64(icon.height));
		}
	}

	void IconLoader::_saveCursorSetToBinotto(const Map<String, Cursor>& cursors, BinaryFile& file)
	{
		file.write(static_cast<uint16>(cursors.getSize()));
		for (auto& [name, cursor] : cursors)
		{
			file.write(static_cast<uint16>(name.getSize()));
			file.write(reinterpret_cast<const uint8*>(name.getData()), name.getSize());

			file.write(cursor.width);
			file.write(cursor.height);
			file.write(cursor.hotspotX);
			file.write(cursor.hotspotY);
			file.writeCompressed(cursor.bitmap, uint64(4) * uint64(cursor.width) * uint64(cursor.height));
		}
	}

} // namespace otto
