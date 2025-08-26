#pragma once

#include "Core/Utils/Vector4.hpp"

namespace core
{
    namespace mathutils
    {
        class Matrix4x4 {
        public:
            float m[4][4]; // Column-major: m[coluna][linha]

            inline Matrix4x4() { LoadIdentity(); }

            void LoadIdentity();

            Matrix4x4 Inverse() const;
            Matrix4x4 Transpose() const;

            static Matrix4x4 Identity();

            static Matrix4x4 Translation(float x, float y, float z);
            static Matrix4x4 Scale(float sx, float sy, float sz);

            static Matrix4x4 RotationZ(float angleRad);
            static Matrix4x4 RotationX(float angleRad);
            static Matrix4x4 RotationY(float angleRad);

            static Matrix4x4 Ortho(float left, float right, float bottom, float top, float nearZ, float farZ);
            static Matrix4x4 Perspective(float fovY, float aspect, float nearZ, float farZ);
            static Matrix4x4 OrthoAspect(float space, int width, int height);

            Matrix4x4 operator*(const Matrix4x4& rhs) const;
            Vector4 operator*(const Vector4& v) const;

            const float* ToPtr() const {
                return &m[0][0]; // OpenGL ready
            }
        };
    }
}
