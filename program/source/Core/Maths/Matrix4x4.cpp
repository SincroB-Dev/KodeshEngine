#include "Core/Maths/Matrix4x4.hpp"

#include <cmath>
#include <cstring>
#include <memory>

namespace core
{
	namespace mathutils
	{
		void Matrix4x4::LoadIdentity()
		{
            std::memset(m, 0, sizeof(m));
            for (int i = 0; i < 4; ++i)
                m[i][i] = 1.0f;
        }

		Matrix4x4 Matrix4x4::Identity() 
		{
            return Matrix4x4();
        }

		Matrix4x4 Matrix4x4::Inverse() const 
		{
            Matrix4x4 result;

            // Separar componentes
            Vector3 right   = Vector3(m[0][0], m[1][0], m[2][0]);
            Vector3 up      = Vector3(m[0][1], m[1][1], m[2][1]);
            Vector3 forward = Vector3(m[0][2], m[1][2], m[2][2]);
            Vector3 pos     = Vector3(m[0][3], m[1][3], m[2][3]);

            // Assumindo rotação ortonormal (sem escala): Inversa = transposta
            result.m[0][0] = right.x;
            result.m[1][0] = right.y;
            result.m[2][0] = right.z;
            result.m[3][0] = 0.0f;

            result.m[0][1] = up.x;
            result.m[1][1] = up.y;
            result.m[2][1] = up.z;
            result.m[3][1] = 0.0f;

            result.m[0][2] = forward.x;
            result.m[1][2] = forward.y;
            result.m[2][2] = forward.z;
            result.m[3][2] = 0.0f;

            // Inverter a translação: -Rᵗ * T
            result.m[0][3] = -(right.x * pos.x + up.x * pos.y + forward.x * pos.z);
            result.m[1][3] = -(right.y * pos.x + up.y * pos.y + forward.y * pos.z);
            result.m[2][3] = -(right.z * pos.x + up.z * pos.y + forward.z * pos.z);
            result.m[3][3] = 1.0f;

            return result;
        }

        Matrix4x4 Matrix4x4::Transpose() const {
            Matrix4x4 result;
            for (int col = 0; col < 4; ++col)
                for (int row = 0; row < 4; ++row)
                    result.m[row][col] = m[col][row];
            return result;
        }

        Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
            Matrix4x4 result;
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    result.m[col][row] = 0.0f;
                    for (int k = 0; k < 4; ++k)
                        result.m[col][row] += m[k][row] * rhs.m[col][k];
                }
            }
            return result;
        }

        Vector4 Matrix4x4::operator*(const Vector4& v) const {
            Vector4 result;
            result.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0]*v.w;
            result.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1]*v.w;
            result.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]*v.w;
            result.w = m[0][3]*v.x + m[1][3]*v.y + m[2][3]*v.z + m[3][3]*v.w;
            return result;
        }

        // Static Members
        Matrix4x4 Matrix4x4::Translation(float x, float y, float z) 
        {
            Matrix4x4 result;
            result.m[3][0] = x;
            result.m[3][1] = y;
            result.m[3][2] = z;
            return result;
        }

        Matrix4x4 Matrix4x4::Scale(float sx, float sy, float sz) 
        {
            Matrix4x4 result;
            result.m[0][0] = sx;
            result.m[1][1] = sy;
            result.m[2][2] = sz;
            return result;
        }

        Matrix4x4 Matrix4x4::RotationZ(float angleRad) 
        {
            Matrix4x4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[0][0] = c;
            result.m[1][0] = -s;
            result.m[0][1] = s;
            result.m[1][1] = c;
            return result;
        }

        Matrix4x4 Matrix4x4::RotationX(float angleRad) 
        {
            Matrix4x4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[1][1] = c;
            result.m[2][1] = -s;
            result.m[1][2] = s;
            result.m[2][2] = c;
            return result;
        }

        Matrix4x4 Matrix4x4::RotationY(float angleRad) 
        {
            Matrix4x4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[0][0] = c;
            result.m[2][0] = s;
            result.m[0][2] = -s;
            result.m[2][2] = c;
            return result;
        }

        Matrix4x4 Matrix4x4::OrthoAspect(float space, int width, int height) 
        {
            float aspect = static_cast<float>(width) / height;

            if (width > height) {
                return Ortho(-space, space,
                            -space / aspect, space / aspect,
                            -space, space);
            } else {
                return Ortho(-space * aspect, space * aspect,
                            -space, space,
                            -space, space);
            }
        }

        Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float nearZ, float farZ) 
        {
            Matrix4x4 result;
            result.m[0][0] = 2.0f / (right - left);
            result.m[1][1] = 2.0f / (top - bottom);
            result.m[2][2] = -2.0f / (farZ - nearZ);
            result.m[3][0] = -(right + left) / (right - left);
            result.m[3][1] = -(top + bottom) / (top - bottom);
            result.m[3][2] = -(farZ + nearZ) / (farZ - nearZ);
            return result;
        }

        Matrix4x4 Matrix4x4::Perspective(float fovY, float aspect, float nearZ, float farZ) 
        {
            Matrix4x4 result;
            float tanHalfFov = std::tan(fovY / 2.0f);
            result.m[0][0] = 1.0f / (aspect * tanHalfFov);
            result.m[1][1] = 1.0f / tanHalfFov;
            result.m[2][2] = -(farZ + nearZ) / (farZ - nearZ);
            result.m[2][3] = -1.0f;
            result.m[3][2] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
            result.m[3][3] = 0.0f;
            return result;
        }
	}
}