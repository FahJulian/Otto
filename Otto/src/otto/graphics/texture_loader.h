#pragma once

#include "otto/base.h"
#include "otto/graphics/sprite.h"
#include "otto/serialization/serialized.h"

namespace otto
{
    class TextureLoader
    {
    private:
        TextureLoader() = delete;
        TextureLoader(const TextureLoader& other) = delete;

    public:
        static Texture2D loadTextureFromSerialized(const Serialized& serialized);

        static Sprite loadSpriteFromSerialized(const Serialized& serialized);
    };

} // namespace otto
