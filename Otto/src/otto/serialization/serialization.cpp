#include "serialization.h"

namespace otto
{
	template<>
	Serialized serialize(const String& string)
	{
		return Serialized(string);
	}

	template<typename T> requires std::is_integral_v<T>
	Serialized serialize(const Vec2<T>& v)
	{
		return String::valueOf(v.x) + ", " + String::valueOf(v.y);
	}

	template<>
	Serialized serialize(const Vec3f32& v)
	{
		return String::valueOf(v.x) + ", " + String::valueOf(v.y) + ", " + String::valueOf(v.z);
	}

	template<>
	Serialized serialize(const Color& color)
	{
		ColorCode colorCode = color.toColorCode();
		return String::valueOf(colorCode, 16);
	}

	template<>
	String deserialize(const Serialized& serialized)
	{
		return serialized.toString();
	}

	template<typename T> requires std::is_integral_v<T>
	Vec2<T> deserialize(const Serialized& serialized)
	{
		auto values = String::split(serialized.get<String>(), ", ");
		return { values[0].to<T>(), values[1].to<T>() };
	}

	template<>
	Vec3f32 deserialize(const Serialized& serialized)
	{
		auto values = String::split(serialized.get<String>(), ", ");
		return { values[0].to<float32>(), values[1].to<float32>(), values[2].to<float32>() };
	}

	template<>
	Color deserialize(const Serialized& serialized)
	{
		Color color = 0x00000000;

		if (serialized.isValue())
		{
			String string = serialized.get<String>();
			if (string.getSize() == 10)
				color = std::stoul(string.getData(), nullptr, 16);
			else if (string.getSize() == 8)
				color = std::stoul(string.getData(), nullptr, 16) | 0x000000ff;
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

} // namespace otto
