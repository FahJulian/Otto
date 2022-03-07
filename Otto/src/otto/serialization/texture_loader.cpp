#include "texture_loader.h"

#include "otto/util/map.h"
#include "otto/core/application.h"
#include "otto/serialization/serializer.h"

namespace otto
{
    namespace
    {
        constexpr Texture2D::Filter DEFAULT_FILTER = Texture2D::Filter::LINEAR;
        constexpr Texture2D::Wrap DEFAULT_WRAP = Texture2D::Wrap::REPEAT;

        Map<FilePath, Texture2D> sLoadedTextures;

        Texture2D::Filter _toFilter(const String& string)
        {
            if (string.equalsIgnoreCase("LINEAR"))
                return Texture2D::Filter::LINEAR;
            else if (string.equalsIgnoreCase("NEAREST"))
                return Texture2D::Filter::NEAREST;
            
            return DEFAULT_FILTER;
        }

        Texture2D::Wrap _toWrap(const String& string)
        {
            if (string.equalsIgnoreCase("REPEAT"))
                return Texture2D::Wrap::REPEAT;

            return DEFAULT_WRAP;
        }

    } // namespace

    Texture2D TextureLoader::loadTextureFromSerialized(const Serialized& serialized)
    {
        if (serialized.isDictionary())
        {
            FilePath filePath = serialized.get<String>("filePath");
            filePath = filePath.toString().startsWith("otto/") ?
                Application::getCoreRootDirectory() + "res/" + filePath : Application::getRootDirectory() + filePath;

            if (sLoadedTextures.containsKey(filePath))
                return sLoadedTextures.get(filePath);
            else
            {
                Texture2D::Filter filter = serialized.contains("filter") ? _toFilter(serialized.get<String>("filter")) : DEFAULT_FILTER;
                Texture2D::Wrap wrap = serialized.contains("wrap") ? _toWrap(serialized.get<String>("wrap")) : DEFAULT_WRAP;

                Texture2D texture = Texture2D(filePath, filter, wrap);

                sLoadedTextures.insert(filePath, texture);
                return texture;
            }
        }
        else if (serialized.isValue())
        {
            FilePath filePath = serialized.get<String>("filePath");
            filePath = filePath.toString().startsWith("otto/") ?
                Application::getCoreRootDirectory() + "res/" + filePath : Application::getRootDirectory() + filePath;

            if (filePath.toString().endsWith(".otto"))
            {
                auto result = Serializer::load(filePath);
                if (result.hasError())
                    return { };
                else
                    return loadTextureFromSerialized(result.getResult());
            }
            else
            {
                if (sLoadedTextures.containsKey(filePath))
                    return sLoadedTextures.get(filePath);
                else
                {
                    Texture2D texture = Texture2D(filePath);

                    sLoadedTextures.insert(filePath, texture);
                    return texture;
                }
            }
        }

        return { };
    }

    Sprite TextureLoader::loadSpriteFromSerialized(const Serialized& serialized)
    {
        if (serialized.contains("spritesheet"))
        {
            FilePath filePath = serialized.get<String>("spritesheet");
            auto result = Serializer::load(filePath.toString().startsWith("otto/") ?
                Application::getCoreRootDirectory() + "res/" + filePath : Application::getRootDirectory() + filePath);

            if (result.hasError())
                return { };

            float32 col = serialized.get<float32>("col");
            float32 row = serialized.get<float32>("row");

            float32 rows = result.getResult().get<float32>("rows");
            float32 cols = result.getResult().get<float32>("cols");
            float32 spriteWidth = result.getResult().get<float32>("spriteWidth");
            float32 spriteHeight = result.getResult().get<float32>("spriteHeight");
            float32 padding = result.getResult().get<float32>("padding");

            Sprite sprite;
            sprite.texture = loadTextureFromSerialized(result.getResult());

            sprite.x0 = col * (spriteWidth + padding) / sprite.texture.getWidth();
            sprite.x1 = sprite.x0 + (spriteWidth / sprite.texture.getWidth());
            sprite.y1 = (rows - row) * (spriteHeight + padding) / sprite.texture.getHeight();
            sprite.y0 = sprite.y1 - (spriteHeight / sprite.texture.getHeight());

            return sprite;
        }
        else if (serialized.contains("texture"))
        {
            Sprite sprite;
            sprite.texture = loadTextureFromSerialized(serialized.get<String>("texture"));
            sprite.x0 = serialized.get<float32>("x0");
            sprite.x1 = serialized.get<float32>("x1");
            sprite.y0 = serialized.get<float32>("y0");
            sprite.y1 = serialized.get<float32>("y1");

            return sprite;
        }

        return { };
    }

} // namespace otto
