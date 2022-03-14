#pragma once

#include "otto.h"

#include "packages/otto/core.h"
#include "packages/otto/window.h"

#include "packages/otto/2D/components/Camera2DComponent.hpp"
#include "packages/otto/2D/components/Renderer2DComponent.hpp"
#include "packages/otto/2D/components/Transform2DComponent.hpp"

namespace otto
{
    class Renderer2D : public System
    {
    public:        
        struct Vertex
        {
            Vec2<float32> position;
            Color color;
            Vec2<float32> textureCoords;
            float32 textureSlot;
        };

        static constexpr uint64 MAX_ELEMENTS = 5000;
        static constexpr uint64 MAX_TEXTURES = 16;

        Renderer2D(MultiView<Transform2DComponent, Renderer2DComponent>* view, View<Camera2DComponent>* cameraView)
        {
            mView = view;
            mCameraView = cameraView;
        }

        void onEvent(const InitEvent& initEvent)
        {
            mDefaultCamera = Camera2D({ 0.0f, 0.0f }, 0.0f, 0.0f, Window::getWidth(), 0.0f, Window::getHeight(), 1.0f, 0.0f);

            mShader = Shader(FilePath::getResourceFilePath("otto/shaders/2D.shader"));

            StaticArray<int32, MAX_TEXTURES> textureSlots;
            for (uint64 i = 0; i < MAX_TEXTURES; i++)
                textureSlots[i] = int32(i);

            mShader.bind();
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

            mVBO = VertexBuffer(4 * MAX_ELEMENTS * sizeof(Vertex), {
                BufferElement::create<Vec2<float32>>(),
                BufferElement::create<Vec4<float32>>(),
                BufferElement::create<Vec2<float32>>(),
                BufferElement::create<float32>(),
            });

            mVAO = VertexArray(indices, { mVBO });
        }

        void onEvent(const RebufferEvent& rebufferEvent)
        {
            Camera2D* camera = &mDefaultCamera;
            for (auto [entity, component] : *mCameraView)
            {
                if (component.isActive || camera == &mDefaultCamera)
                    camera = &component.camera;
            }

            mShader.bind();
            mShader.setUniform("uViewProjectionMatrix", camera->getViewProjectionMatrix());

            mElementCount = mView->getSize();
            if (mElementCount > MAX_ELEMENTS)
                mElementCount = MAX_ELEMENTS;

            mTextures.clear();

            for (uint64 i = 0; i < mElementCount; i++)
                _drawElement(i, mView->get(i).second, mView->get(i).third);

            mVBO.bind();
            mVBO.setData(mVertices, 4 * mElementCount);
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
            mDefaultCamera.setProjection(0.0f, resizedEvent.width, 0.0f, resizedEvent.height, 0.0f, 0.0f);
        }

    private:
        void _drawElement(uint64 index, const Transform2DComponent& transform, const Renderer2DComponent& render)
        {
            if (transform.rotation == 0)
            {
                for (uint64 i = 0; i < 4; i++)
                {
                    Vertex& vertex = mVertices[4 * index + i];
                    vertex.position.x = transform.translation.x + (i % 2) * transform.scale.x;
                    vertex.position.y = transform.translation.y + (i / 2) * transform.scale.y;
                    vertex.color = render.color;
                    vertex.textureCoords.x = i % 2 == 0 ? render.sprite.x0 : render.sprite.x1;
                    vertex.textureCoords.y = i / 2 == 0 ? render.sprite.y0 : render.sprite.y1;
                    vertex.textureSlot = _textureSlotOf(render.sprite.texture);
                }
            }
            else
            {
                Vec2<float32> center = { 0.5f * transform.scale.x, 0.5f * transform.scale.y };
                float32 r = sqrt(center.x * center.x + center.y * center.y);

                float32 sinus = r * sin(PI_32 / 4 - radians(transform.rotation));
                float32 cosinus = r * cos(PI_32 / 4 - radians(transform.rotation));

                mVertices[4 * index + 0].position.x = transform.translation.x + center.x - cosinus;
                mVertices[4 * index + 0].position.y = transform.translation.y + center.y - sinus;
                mVertices[4 * index + 1].position.x = transform.translation.x + center.x + sinus;
                mVertices[4 * index + 1].position.y = transform.translation.y + center.y - cosinus;
                mVertices[4 * index + 2].position.x = transform.translation.x + center.x - sinus;
                mVertices[4 * index + 2].position.y = transform.translation.y + center.y + cosinus;
                mVertices[4 * index + 3].position.x = transform.translation.x + center.x + cosinus;
                mVertices[4 * index + 3].position.y = transform.translation.y + center.y + sinus;

                for (uint64 i = 0; i < 4; i++)
                {
                    Vertex& vertex = mVertices[4 * index + i];
                    vertex.color = render.color;
                    vertex.textureCoords.x = i % 2 == 0 ? render.sprite.x0 : render.sprite.x1;
                    vertex.textureCoords.y = i / 2 == 0 ? render.sprite.y0 : render.sprite.y1;
                    vertex.textureSlot = _textureSlotOf(render.sprite.texture);
                }
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

    private:
        MultiView<Transform2DComponent, Renderer2DComponent>* mView = nullptr;
        View<Camera2DComponent>* mCameraView = nullptr;
        uint64 mElementCount = 0;
        Shader mShader;
        VertexArray mVAO;
        VertexBuffer mVBO;
        Camera2D mDefaultCamera;
        DynamicArray<Vertex> mVertices;
        DynamicArray<Texture2D> mTextures;
    };

} // namespace otto
