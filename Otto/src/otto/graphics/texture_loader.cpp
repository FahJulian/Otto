#include "texture_loader.h"

#include "otto/util/map.h"
#include "otto/core/application.h"
#include "otto/serialization/serializer.h"

namespace otto
{
    namespace
    {
        Map<FilePath, Texture2D> sLoadedTextures;

    } // namespace

    Texture2D TextureLoader::loadTextureFromSerialized(const Serialized& serialized)
    {
        if (serialized.isValue())
        {
            FilePath filePath = serialized.get<String>();

            if (sLoadedTextures.containsKey(filePath))
                return sLoadedTextures.get(filePath);
            else
            {
                Texture2D texture = Texture2D(filePath.toString().startsWith("otto/") ?
                    Application::getCoreRootDirectory() + "res/" + filePath : Application::getRootDirectory() + filePath);

                sLoadedTextures.insert(filePath, texture);
                return texture;
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

            uint16 col = serialized.get<uint16>("col");
            uint16 row = serialized.get<uint16>("row");

            uint16 rows = result.getResult().get<uint16>("rows");
            uint16 cols = result.getResult().get<uint16>("cols");
            uint16 spriteWidth = result.getResult().get<uint16>("spriteWidth");
            uint16 spriteHeight = result.getResult().get<uint16>("spriteHeight");
            uint16 padding = result.getResult().get<uint16>("padding");

            Sprite sprite;
            sprite.texture = loadTextureFromSerialized(result.getResult().get<String>("texture"));
            sprite.x0 = float32(col * (spriteWidth + padding)) / sprite.texture.getWidth();
            sprite.x1 = float32(sprite.x0) + (spriteWidth / sprite.texture.getWidth());
            sprite.y1 = float32((rows - row) * (spriteHeight + padding)) / sprite.texture.getHeight();
            sprite.y0 = float32(sprite.y1) - (spriteHeight / sprite.texture.getHeight());

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
