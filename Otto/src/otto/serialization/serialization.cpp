#include "serialization.h"

#include "otto/serialization/texture_loader.h"

namespace otto
{
	template<>
	Serialized serialize(const String& string)
	{
		return Serialized(string);
	}

	template<>
	Serialized serialize(const Color& color)
	{
		ColorCode colorCode = color.toColorCode();
		return "#" + String::valueOf(colorCode, 16);
	}

	template<>
	String deserialize(const Serialized& serialized)
	{
		return serialized.toString();
	}

	template<>
	Color deserialize(const Serialized& serialized)
	{
		Color color = 0x00000000;

		if (serialized.isValue())
		{
			String string = serialized.get<String>();
			if (string.getSize() == 9)
				color = std::stoul(String::subString(string, 1, string.getSize()).getData(), nullptr, 16);
			else if (string.getSize() == 7)
				color = std::stoul(String::subString(string, 1, string.getSize()).getData(), nullptr, 16) | 0x000000ff;
		}
		else if (serialized.isDictionary())
		{
			if (serialized.contains("r"))
				color.r = serialized.get<uint8>("r");
			if (serialized.contains("g"))
				color.g = serialized.get<uint8>("g");
			if (serialized.contains("b"))
				color.b = serialized.get<uint8>("b");
			if (serialized.contains("a"))
				color.a = serialized.get<uint8>("a");
		}
		else if (serialized.isList())
		{
			if (serialized.getList().getSize() == 3 || serialized.getList().getSize() == 4)
			{
				color.r = serialized.getList()[0].get<uint8>();
				color.g = serialized.getList()[1].get<uint8>();
				color.b = serialized.getList()[2].get<uint8>();
				if (serialized.getList().getSize() == 4)
					color.a = serialized.getList()[3].get<uint8>();
			}
		}

		return color;
	}

	template<>
	Sprite deserialize(const Serialized& serialized)
	{
		return TextureLoader::loadSpriteFromSerialized(serialized);
	}

	template<>
	Vec2<float32> deserialize(const Serialized& serialized)
	{
		auto values = String::split(serialized.get<String>(), ", ");

		return { String::stringTo<float32>(values[0]), String::stringTo<float32>(values[1]) };
	}

} // namespace otto
