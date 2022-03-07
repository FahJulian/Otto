#pragma once

#include "otto.h"

#include "packages/otto/core.h"
#include "packages/otto/window.h"

#include "packages/otto/ui/components/UIComponent.hpp"
#include "packages/otto/ui/events/UIRendererRebufferEvent.hpp"

namespace otto
{
    namespace
    {
        struct UIVertex
        {
            float32 x, y, zIndex;
            float32 r, g, b, a;
            float32 textureX, textureY;
            float32 textureSlot;

            float32 rectX, rectY;
            float32 rectWidth, rectHeight;
            float32 borderR, borderG, borderB, borderA;
            float32 borderWeight;
            float32 edgeRadius;
        };

        static constexpr uint64 MAX_ELEMENTS = 5000;
        static constexpr uint64 MAX_TEXTURES = 16;

    } // namespace

    class UIRenderer : public System
    {
    public:
        UIRenderer(View<UIComponent>* uiComponentView)
        {
            mUIComponentView = uiComponentView;
        }

        void onEvent(const InitEvent& initEvent)
        {
            mShader = Shader(FilePath::getResourceFilePath("otto/shaders/ui.shader"));

            mShader.bind();
            mShader.setUniform("uViewportWidth", Window::getWidth());
            mShader.setUniform("uViewportHeight", Window::getHeight());

            StaticArray<int32, MAX_TEXTURES> textureSlots;
            for (uint64 i = 0; i < MAX_TEXTURES; i++)
                textureSlots[i] = int32(i);

            mShader.setUniform("uTextures", textureSlots);

            mTextures.setCapacity(MAX_TEXTURES);
            mVertices.setSize(MAX_ELEMENTS * 4);

            DynamicArray<uint32> indices;
            indices.setSize(6 * MAX_ELEMENTS);
            for (uint64 i = 0; i < MAX_ELEMENTS; i++)
            {
                indices[6 * i + 0] = uint32(4 * i + 0);
                indices[6 * i + 1] = uint32(4 * i + 1);
                indices[6 * i + 2] = uint32(4 * i + 2);

                indices[6 * i + 3] = uint32(4 * i + 1);
                indices[6 * i + 4] = uint32(4 * i + 3);
                indices[6 * i + 5] = uint32(4 * i + 2);
            }

            mVBO = VertexBuffer(4 * MAX_ELEMENTS * sizeof(UIVertex), {
                BufferElement::create<Vec3f32>(),
                BufferElement::create<Vec4f32>(), 
                BufferElement::create<Vec2f32>(), 
                BufferElement::create<float32>(), 
                BufferElement::create<Vec2f32>(), 
                BufferElement::create<Vec2f32>(), 
                BufferElement::create<Vec4f32>(), 
                BufferElement::create<float32>(),
                BufferElement::create<float32>() 
            });

            mVAO = VertexArray(indices, { mVBO });
        }

        void onEvent(const RebufferEvent& rebufferEvent)
        {
            mElementCount = mUIComponentView->getSize();
            if (mElementCount > MAX_ELEMENTS)
                mElementCount = MAX_ELEMENTS;

            if (mRebuffer)
            {
                mTextures.clear();

                for (uint64 i = 0; i < mUIComponentView->getSize() && i < MAX_ELEMENTS; i++)
                    _drawElement(i, mUIComponentView->get(i).second);

                mVBO.bind();
                mVBO.setData(mVertices, 4 * mElementCount);

                mRebuffer = false;
            }
        }

        void onEvent(const RenderEvent& renderEvent)
        {
            if (mElementCount == 0)
                return;

            mShader.bind();
            mVAO.bind();

            for (uint64 i = 0; i < mTextures.getSize(); i++)
                mTextures[i].bind(uint32(i));

            GraphicsAPI::drawIndexed(mVAO, uint32(6 * mElementCount));
        }

        void onEvent(const WindowResizedEvent& resizedEvent)
        {
            mShader.bind();
            mShader.setUniform("uViewportWidth", resizedEvent.width);
            mShader.setUniform("uViewportHeight", resizedEvent.height);

            mRebuffer = true;
        }

        void onEvent(const ComponentAddedEvent<UIComponent>& e)
        {
            mRebuffer = true;
        }

        void onEvent(const ComponentRemovedEvent<UIComponent>& e)
        {
            mRebuffer = true;
        }

        void onEvent(const UIRendererRebufferEvent& rebufferEvent)
        {
            mRebuffer = true;
        }

    private:
        void _drawElement(uint64 index, const UIComponent& component)
        {
            float32 x = /*floor*/(component.x.absoluteValue);
            float32 y = /*floor*/(component.y.absoluteValue);
            float32 width = /*floor*/(component.width.absoluteValue);
            float32 height = /*floor*/(component.height.absoluteValue);
            const Sprite& sprite = component.renderingProperties.sprite;

            for (uint64 i = 0; i < 4; i++)
            {
                UIVertex& vertex = mVertices[4 * index + i];

                vertex.x = x + (i % 2) * width;
                vertex.y = y + (i / 2) * height;
                vertex.zIndex = component.zIndex;
                vertex.r = component.renderingProperties.color.r;
                vertex.g = component.renderingProperties.color.g;
                vertex.b = component.renderingProperties.color.b;
                vertex.a = component.renderingProperties.color.a;
                vertex.textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
                vertex.textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
                vertex.textureSlot = _textureSlotOf(sprite.texture);

                vertex.rectX = x;
                vertex.rectY = y;

                vertex.rectWidth = width;
                vertex.rectHeight = height;

                vertex.borderR = component.renderingProperties.borderColor.r;
                vertex.borderG = component.renderingProperties.borderColor.g;
                vertex.borderB = component.renderingProperties.borderColor.b;
                vertex.borderA = component.renderingProperties.borderColor.a;
                vertex.borderWeight = component.renderingProperties.borderWeight;
                vertex.edgeRadius = component.renderingProperties.edgeRadius;
            }
        }   

        float32 _textureSlotOf(const Texture2D& texture)
        {
            if (!texture.isValid())
                return -1.0f;

            for (uint64 i = 0; i < mTextures.getSize(); i++)
            {
                if (mTextures[i] == texture)
                    return static_cast<float32>(i);
            }

            if (mTextures.getSize() < MAX_TEXTURES)
            {
                mTextures.add(texture);
                return static_cast<float32>(mTextures.getSize() - 1);
            }

            return -1.0f;
        }

        View<UIComponent>* mUIComponentView = nullptr;
        bool8 mRebuffer = true;
        uint64 mElementCount = 0;
        Shader mShader;
        VertexArray mVAO;
        VertexBuffer mVBO;  
        DynamicArray<UIVertex> mVertices;
        DynamicArray<Texture2D> mTextures;
    };

} // namespace otto
