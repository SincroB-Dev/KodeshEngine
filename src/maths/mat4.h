#ifndef mat4_HPP
#define mat4_HPP

#include <cmath>
#include <cstring>
#include "vec4.h"

namespace maths
{
    class mat4 {
    public:
        float m[4][4];

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

        static mat4 Translation(float x, float y, float z) {
            mat4 result;
            result.m[0][3] = x;
            result.m[1][3] = y;
            result.m[2][3] = z;
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
            result.m[0][1] = -s;
            result.m[1][0] = s;
            result.m[1][1] = c;
            return result;
        }

        static mat4 RotationX(float angleRad) {
            mat4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[1][1] = c;
            result.m[1][2] = -s;
            result.m[2][1] = s;
            result.m[2][2] = c;
            return result;
        }

        static mat4 RotationY(float angleRad) {
            mat4 result;
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            result.m[0][0] = c;
            result.m[0][2] = s;
            result.m[2][0] = -s;
            result.m[2][2] = c;
            return result;
        }

        mat4 Transpose() const {
            mat4 result;
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    result.m[i][j] = m[j][i];
            return result;
        }

        mat4 operator*(const mat4& rhs) const {
            mat4 result;
            for (int row = 0; row < 4; ++row)
                for (int col = 0; col < 4; ++col) {
                    result.m[row][col] = 0.0f;
                    for (int k = 0; k < 4; ++k)
                        result.m[row][col] += m[row][k] * rhs.m[k][col];
                }
            return result;
        }

        vec4 operator*(const vec4& v) const {
            vec4 result;
            result.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w;
            result.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w;
            result.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w;
            result.w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w;
            return result;
        }

        static mat4 Ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
            mat4 result;
            result.m[0][0] = 2.0f / (right - left);
            result.m[1][1] = 2.0f / (top - bottom);
            result.m[2][2] = -2.0f / (farZ - nearZ);
            result.m[0][3] = -(right + left) / (right - left);
            result.m[1][3] = -(top + bottom) / (top - bottom);
            result.m[2][3] = -(farZ + nearZ) / (farZ - nearZ);
            return result;
        }

        static mat4 Perspective(float fovY, float aspect, float nearZ, float farZ) {
            mat4 result;
            float tanHalfFov = std::tan(fovY / 2.0f);
            result.m[0][0] = 1.0f / (aspect * tanHalfFov);
            result.m[1][1] = 1.0f / tanHalfFov;
            result.m[2][2] = -(farZ + nearZ) / (farZ - nearZ);
            result.m[2][3] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
            result.m[3][2] = -1.0f;
            result.m[3][3] = 0.0f;
            return result;
        }
    };
};

#endif