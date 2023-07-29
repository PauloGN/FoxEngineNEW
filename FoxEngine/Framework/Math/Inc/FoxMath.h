#pragma once

#include "Common.h"

namespace FoxEngine::FoxMath
{
	template<class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template<class T>
	constexpr T Lerp(T a, T b, T t)
	{
		return a + ((b - a) * t);
	}

	template<class T>
	constexpr T Abs(T value)
	{
		return value >= 0 ? value : -value;
	}

	template<class T>
	constexpr T Sqr(T value)
	{
		return value * value;
	}

	//Vector 2

	constexpr float Dot(Vector2 a, Vector2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	constexpr float MagnitudeSqr(Vector2 a)
	{
		return a.x * a.x + a.y * a.y;
	}

	inline float Magnitude(Vector2 a)
	{
		return sqrt(a.x * a.x + a.y * a.y);
	}

	inline float DistanceSqr(Vector2 a, Vector2 b)
	{
		return abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	inline float Distance(Vector2 a, Vector2 b)
	{
		return sqrt(abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
	}

	inline Vector2 Normalize(Vector2 a)
	{
		float magA = sqrt(a.x * a.x + a.y * a.y);
		return a / magA;
	}

	//Vector 3
	constexpr float Dot(Vector3 a, Vector3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	constexpr float MagnitudeSqr(Vector3 a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}

	inline float Magnitude(Vector3 a)
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	inline float DistanceSqr(Vector3 a, Vector3 b)
	{
		return abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

	inline float Distance(Vector3 a, Vector3 b)
	{
		return sqrt(abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)));
	}

	inline Vector3 Normalize(Vector3 a)
	{
		float magA = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		return a / magA;
	}

	constexpr Vector3 Cross(Vector3 a, Vector3 b)
	{
		float x = a.y * b.z - a.z * b.y;
		float y = a.z * b.x - a.x * b.z;
		float z = a.x * b.y - a.y * b.x;

		return { x, y, z };
	}

	inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		float x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
		float y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
		float z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

		return { x, y, z };
	}

	inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		float x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		float y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		float z = v.x * m._13 + v.y * m._23 + v.z * m._33;

		return { x, y, z };
	}

	inline Vector3 quaternionToVector3(const Quaternion& q)
	{
		Vector3 v;

		v.x = 2 * (q.x * q.z - q.w * q.y);
		v.y = 2 * (q.y * q.z + q.w * q.x);
		v.z = 1 - 2 * (q.x * q.x + q.y * q.y);

		return v;
	}

	//Vector 4

	constexpr float Dot(Vector4 a, Vector4 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	constexpr float MagnitudeSqr(Vector4 a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	}

	inline float Magnitude(Vector4 a)
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}

	inline float DistanceSqr(Vector4 a, Vector4 b)
	{
		return abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z) + (a.w - b.w) * (a.w - b.w));
	}

	inline float Distance(Vector4 a, Vector4 b)
	{
		return sqrt(abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z) + (a.w - b.w) * (a.w - b.w)));
	}

	inline Vector4 Normalize(Vector4 a)
	{
		float magA = sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
		return a / magA;
	}

	//constexpr Vector4 Cross(Vector4 a, Vector4 b)
	//{
	//	float x = a.y * b.z - a.z * b.y;
	//	float y = a.z * b.x - a.x * b.z;
	//	float z = a.x * b.y - a.y * b.x;
	//	float w = a.w * b.y - a.y * b.x;

	//	return { x, y, z, w };
	//}

	//inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	//{
	//	float x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	//	float y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	//	float z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

	//	return { x, y, z };
	//}

	//inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	//{
	//	float x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	//	float y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	//	float z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	//	return { x, y, z };
	//}

	//Matrix

	inline Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4(
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		);
	}

	inline Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return {
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
}