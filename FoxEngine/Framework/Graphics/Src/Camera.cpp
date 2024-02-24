#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void Camera::SetMode(ProjectionMode mode)
{
	mProjectionMode = mode;
}

void Camera::SetPosition(const FoxMath::Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const FoxMath::Vector3& direction)
{
	// Prevent setting direction straight up or down
	auto dir = FoxMath::Normalize(direction);
	if (FoxMath::Abs(FoxMath::Dot(dir, FoxMath::Vector3::YAxis)) < 0.995f)
		mDirection = dir;
}

void Camera::SetLookAt(const FoxMath::Vector3& target)
{
	SetDirection(target - mPosition);
}

void Camera::SetFov(float fov)
{
	constexpr float kMinFov = 10.0f * FoxMath::Constants::DegToRad;
	constexpr float kMaxFov = 170.0f * FoxMath::Constants::DegToRad;
	mFov = FoxMath::Clamp(fov, kMinFov, kMaxFov);
}

void Camera::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
}

void Camera::SetSize(float width, float height)
{
	mWidth = width;
	mHeight = height;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const FoxMath::Vector3 right = FoxMath::Normalize(Cross(FoxMath::Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += FoxMath::Vector3::YAxis * distance;
}

void Camera::Yaw(float radian)
{
	FoxMath::Matrix4 matRotate = FoxMath::Matrix4::RotationY(radian);
	mDirection = FoxMath::TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float radian)
{
	const FoxMath::Vector3 right = FoxMath::Normalize(Cross(FoxMath::Vector3::YAxis, mDirection));
	const FoxMath::Matrix4 matRot = FoxMath::Matrix4::RotationAxis(right, radian);
	const FoxMath::Vector3 newLook = FoxMath::TransformNormal(mDirection, matRot);
	SetDirection(newLook);
}

void Camera::Zoom(float amount)
{
	constexpr float minZoom = 170.0f * FoxMath::Constants::DegToRad;
	constexpr float maxZoom = 10.0f * FoxMath::Constants::DegToRad;
	mFov = FoxMath::Clamp(mFov - amount, maxZoom, minZoom);
}

const FoxMath::Vector3& Camera::GetPosition() const
{
	return mPosition;
}

const FoxMath::Vector3& Camera::GetDirection() const
{
	return mDirection;
}

FoxMath::Matrix4 Camera::GetViewMatrix() const
{
	const FoxMath::Vector3 l = mDirection;
	const FoxMath::Vector3 r = FoxMath::Normalize(FoxMath::Cross(FoxMath::Vector3::YAxis, l));
	const FoxMath::Vector3 u = FoxMath::Normalize(FoxMath::Cross(l, r));
	const float x = -FoxMath::Dot(r, mPosition);
	const float y = -FoxMath::Dot(u, mPosition);
	const float z = -FoxMath::Dot(l, mPosition);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}

FoxMath::Matrix4 Camera::GetProjectionMatrix() const
{
	return (mProjectionMode == ProjectionMode::Perspective) ? GetPerspectiveMatrix() : GetOrthographicMatrix();
}

FoxMath::Matrix4 Camera::GetPerspectiveMatrix() const
{
	const float a = (mAspectRatio == 0.0f) ? GraphicsSystem::Get()->GetBackBufferAspectRatio() : mAspectRatio;
	const float h = 1.0f / tan(mFov * 0.5f);
	const float w = h / a;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float q = zf / (zf - zn);

	return {
		w,    0.0f, 0.0f,    0.0f,
		0.0f, h,    0.0f,    0.0f,
		0.0f, 0.0f, q,       1.0f,
		0.0f, 0.0f, -zn * q, 0.0f
	};
}

FoxMath::Matrix4 Camera::GetOrthographicMatrix() const
{
	const float w = (mWidth == 0.0f) ? GraphicsSystem::Get()->GetBackBufferWidth() : mWidth;
	const float h = (mHeight == 0.0f) ? GraphicsSystem::Get()->GetBackBufferHeight() : mHeight;
	const float f = mFarPlane;
	const float n = mNearPlane;
	return 	{
		2 / w, 0.0f,  0.0f,        0.0f,
		0.0f,  2 / h, 0.0f,        0.0f,
		0.0f,  0.0f,  1 / (f - n), 0.0f,
		0.0f,  0.0f,  n / (n - f), 1.0f
	};
}

void Camera::SetIsActive(bool active)
{
	isActive = active;
}

const bool Camera::GetIsActive() const
{
	return isActive;
}
