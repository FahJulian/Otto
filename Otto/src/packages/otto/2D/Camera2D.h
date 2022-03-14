#pragma once

#include "otto.h"

namespace otto
{
    class Camera2D
    {
    public:
        Camera2D() = default;

        Camera2D(Vec2<float32> position, float32 rotation, float32 left, float32 right, 
            float32 bottom, float32 top, float32 near, float32 far)
            : mPosition(position), mRotation(rotation), mViewMatrix(_calculateViewMatrix()), 
            mProjectionMatrix(Mat4x4<float32>::orthographicProjection(left, right, bottom, top, near, far)),
            mViewProjectionMatrix(mProjectionMatrix * mViewMatrix)
        {
        }

        const Mat4x4<float32>& getViewMatrix() const { return mViewMatrix; };
        const Mat4x4<float32>& getProjectionMatrix() const { return mProjectionMatrix; }
        const Mat4x4<float32>& getViewProjectionMatrix() const { return mViewProjectionMatrix; }

        void setProjection(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far)
        {
            mProjectionMatrix = Mat4x4<float32>::orthographicProjection(left, right, bottom, top, near, far);
            mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
        }

        void setPosition(const Vec2<float32>& position)
        {
            mPosition = position;
            mViewMatrix = _calculateViewMatrix();
            mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
        }

        void setPosition(float32 x, float32 y) { setPosition({ x, y }); }

        void setRotation(float32 rotation)
        {
            mRotation = rotation;
            mViewMatrix = _calculateViewMatrix();
            mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
        }

    private:
        Mat4x4<float32> _calculateViewMatrix()
        {
            Mat4x4<float32> translation = Mat4x4<float32>::translation(mPosition);
            Mat4x4<float32> rotation = Mat4x4<float32>::rotationZ(radians(-mRotation));

            return inverse(translation * rotation);
        }

    private:
        Vec2<float32> mPosition;
        float32 mRotation;
        Mat4x4<float32> mViewMatrix;
        Mat4x4<float32> mProjectionMatrix;
        Mat4x4<float32> mViewProjectionMatrix;
    };

    template<>
    Camera2D deserialize(const Serialized& serialized)
    {
        Vec2<float32> position;
        float32 rotation = 0.0f;
        float32 left = 0.0f;
        float32 right = Window::getWidth();
        float32 bottom = 0.0f;
        float32 top = Window::getHeight();
        float32 near = 1.0f;
        float32 far = -1.0f;

        if (serialized.contains("position"))
            position = serialized.get<Vec2<float32>>("position");
        if (serialized.contains("rotation"))
            rotation = serialized.get<float32>("rotation");
        if (serialized.contains("left"))
            left = serialized.get<float32>("left");
        if (serialized.contains("right"))
            right = serialized.get<float32>("right");
        if (serialized.contains("bottom"))
            bottom = serialized.get<float32>("bottom");
        if (serialized.contains("top"))
            top = serialized.get<float32>("top");
        if (serialized.contains("near"))
            near = serialized.get<float32>("near");
        if (serialized.contains("far"))
            far = serialized.get<float32>("far");

        return Camera2D(position, rotation, left, right, bottom, top, near, far);
    }

} // namespace otto
