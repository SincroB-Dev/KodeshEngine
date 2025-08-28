#pragma once

// Operadores que faltam
inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline ImVec2 operator*(const ImVec2& lhs, float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
inline ImVec2 operator/(const ImVec2& lhs, float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }