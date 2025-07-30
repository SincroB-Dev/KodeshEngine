#ifndef MAT4_HPP
#define MAT4_HPP

#include <cmath>
#include <cstring>
#include "vec4.h"

namespace maths
{
    class mat4 {
    public:
        float m[4][4]; // Column-major: m[coluna][linha]

        mat4() {
            LoadIdentity();
        }

        void LoadIdentity() {
            std::memset(m, 0, sizeof(m));
            for (int i = 0; i < 4; ++i)
                m[i][i] = 1.0f;
        }

        static mat4 Identity() {
            return mat4();
        }

        mat4 Inverse() const {
            mat4 result;

            // Separar componentes
            vec3 right   = vec3(m[0][0], m[1][0], m[2][0]);
            vec3 up      = vec3(m[0][1], m[1][1], m[2][1]);
            vec3 forward = vec3(m[0][2], m[1][2], m[2][2]);
            vec3 pos     = vec3(m[0][3], m[1][3], m[2][3]);

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


        static mat4 Translation(float x, float y, float z) {
            mat4 result;
            result.m[3][0] = x;
            result.m[3][1] = y;
            result.m[3][2] = z;
            return result;
        }

        static mat4 Scale(float sx, float sy, float sz) {
            mat4 result;
            result.m[0][0] = sx;
            result.m[1][1] = sy;
            result.m[2][2] = sz;
            return result;
        }

        static mat4 RotationZ(float angleRad) {
            mat4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[0][0] = c;
            result.m[1][0] = -s;
            result.m[0][1] = s;
            result.m[1][1] = c;
            return result;
        }

        static mat4 RotationX(float angleRad) {
            mat4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[1][1] = c;
            result.m[2][1] = -s;
            result.m[1][2] = s;
            result.m[2][2] = c;
            return result;
        }

        static mat4 RotationY(float angleRad) {
            mat4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[0][0] = c;
            result.m[2][0] = s;
            result.m[0][2] = -s;
            result.m[2][2] = c;
            return result;
        }

        mat4 Transpose() const {
            mat4 result;
            for (int col = 0; col < 4; ++col)
                for (int row = 0; row < 4; ++row)
                    result.m[row][col] = m[col][row];
            return result;
        }

        mat4 operator*(const mat4& rhs) const {
            mat4 result;
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    result.m[col][row] = 0.0f;
                    for (int k = 0; k < 4; ++k)
                        result.m[col][row] += m[k][row] * rhs.m[col][k];
                }
            }
            return result;
        }

        vec4 operator*(const vec4& v) const {
            vec4 result;
            result.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0]*v.w;
            result.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1]*v.w;
            result.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]*v.w;
            result.w = m[0][3]*v.x + m[1][3]*v.y + m[2][3]*v.z + m[3][3]*v.w;
            return result;
        }

        static mat4 Ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
            mat4 result;
            result.m[0][0] = 2.0f / (right - left);
            result.m[1][1] = 2.0f / (top - bottom);
            result.m[2][2] = -2.0f / (farZ - nearZ);
            result.m[3][0] = -(right + left) / (right - left);
            result.m[3][1] = -(top + bottom) / (top - bottom);
            result.m[3][2] = -(farZ + nearZ) / (farZ - nearZ);
            return result;
        }

        static mat4 Perspective(float fovY, float aspect, float nearZ, float farZ) {
            mat4 result;
            float tanHalfFov = std::tan(fovY / 2.0f);
            result.m[0][0] = 1.0f / (aspect * tanHalfFov);
            result.m[1][1] = 1.0f / tanHalfFov;
            result.m[2][2] = -(farZ + nearZ) / (farZ - nearZ);
            result.m[2][3] = -1.0f;
            result.m[3][2] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
            result.m[3][3] = 0.0f;
            return result;
        }

        static mat4 OrthoAspect(float space, int width, int height) {
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

        const float* ToPtr() const {
            return &m[0][0]; // OpenGL ready
        }
    };
};

#endif
