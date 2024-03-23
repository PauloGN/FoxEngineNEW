#include "Precompiled.h"
#include "FoxMath.h"
#include "Quaternion.h"

using namespace FoxEngine::FoxMath;

const Vector2 Vector2::Zero(0.0f);
const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::XAxis(1.0f, 0.0f);
const Vector2 Vector2::YAxis(0.0f, 1.0f);

const Vector3 Vector3::Zero(0.0f);
const Vector3 Vector3::One(1.0f);
const Vector3 Vector3::XAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAxis(0.0f, 0.0f, 1.0f);

Vector3 Vector3::QuaternionToEuler(const Quaternion& quaternion) noexcept
{
    // Extracting yaw (Z), pitch (Y), and roll (X) from the quaternion

    // Yaw (Z) calculation
    float sinYaw = 2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
    float cosYaw = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
    float yaw = atan2f(sinYaw, cosYaw);

    // Pitch (Y) calculation
    float sinPitch = 2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
    float pitch = asinf(sinPitch);

    // Roll (X) calculation
    float sinRoll = 2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
    float cosRoll = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
    float roll = atan2f(sinRoll, cosRoll);

    // Convert angles to degrees using your constants
    yaw = yaw * Constants::RadToDeg;
    pitch = pitch * Constants::RadToDeg;
    roll = roll * Constants::RadToDeg;

    return Vector3(roll, pitch, yaw);
}


const Matrix4 Matrix4::Zero({ 0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0 });

const Matrix4 Matrix4::Identity({ 1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1 });

//------------------------------------------------------------------------------
// Quaternion operations
//------------------------------------------------------------------------------
const Quaternion Quaternion::Identity = { 0.0f, 0.0f, 0.0f, 1.0f };
const Quaternion Quaternion::Zero = { 0.0f, 0.0f, 0.0f, 0.0f };


void Quaternion::Conjugate() noexcept
{
    x = -x; y = -y; z = -z;
}

void Quaternion::Inverse() noexcept
{
     Conjugate();
    *this = *this / MagnitudeSquared();
}

float Quaternion::MagnitudeSquared() const noexcept
{
    return (x * x + y * y + z * z + w * w);
}

float Quaternion::Magnitude() const noexcept
{
    return sqrt(MagnitudeSquared());
}

void Quaternion::Normalize() noexcept
{
    *this = *this / Magnitude();
}

float Quaternion::Dot(const Quaternion& q) const noexcept
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
    return { -q.x, -q.y, -q.z, q.w };
}

float Quaternion::Magnitude(const Quaternion& q)
{
    return std::sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
    return q / Quaternion::Magnitude(q);
}

Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, float angle) noexcept
{
    const float c = cos(angle * 0.5f);
    const float s = sin(angle * 0.5f);
    const FoxMath::Vector3 a = FoxMath::Normalize(axis);
    return Quaternion(a.x * s, a.y * s, a.z * s, c);
}

Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
    const float cy = cos(yaw * 0.5f);
    const float sy = sin(yaw * 0.5f);
    const float cp = cos(pitch * 0.5f);
    const float sp = sin(pitch * 0.5f);
    const float cr = cos(roll * 0.5f);
    const float sr = sin(roll * 0.5f);

    return {
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    };
}

Quaternion Quaternion::CreateFromRotationMatrix(const Matrix4& m) noexcept
{
    const float w = sqrt(m._11 + m._22 + m._33 + 1) * 0.5f;
    const float x = sqrt(m._11 - m._22 - m._33 + 1) * 0.5f;
    const float y = sqrt(-m._11 + m._22 - m._33 + 1) * 0.5f;
    const float z = sqrt(-m._11 - m._22 + m._33 + 1) * 0.5f;

    Quaternion q;

    if (w >= x && w >= y && w >= z)
    {
        q.w = w;
        q.x = (m._23 - m._32) / (4.0f * w);
        q.y = (m._31 - m._13) / (4.0f * w);
        q.z = (m._12 - m._21) / (4.0f * w);
    }
    else if (x >= w && x >= y && x >= z)
    {
        q.w = (m._23 - m._32) / (4.0f * x);
        q.x = x;
        q.y = (m._12 - m._21) / (4.0f * x);
        q.z = (m._31 - m._13) / (4.0f * x);
    }
    else if (y >= w && y >= x && y >= z)
    {
        q.w = (m._31 - m._13) / (4.0f * y);
        q.x = (m._12 - m._21) / (4.0f * y);
        q.y = y;
        q.z = (m._23 - m._32) / (4.0f * y);
    }
    else if (z >= w && z >= x && z >= y)
    {
        q.w = (m._12 - m._21) / (4.0f * z);
        q.x = (m._31 - m._13) / (4.0f * z);
        q.y = (m._23 - m._32) / (4.0f * z);
        q.z = z;
    }

    return q;
}

 Vector3 FoxEngine::FoxMath::Quaternion::Rotate(const FoxMath::Vector3& v) noexcept
{     
    FoxMath::Vector3 qv = { x, y, z };
    FoxMath::Vector3 c = FoxMath::Cross(qv, v);
    FoxMath::Vector3 t(2.0f * c.x, 2.0f * c.y, 2.0f * c.z);
    
    // Manually perform element-wise multiplication
    return {
        v.x + w * t.x + FoxMath::Cross(qv, t).x,
        v.y + w * t.y + FoxMath::Cross(qv, t).y,
        v.z + w * t.z + FoxMath::Cross(qv, t).z
    };  
}

Quaternion Quaternion::EulerToQuaternion(const Vector3& euler) noexcept
{
    // Assuming Euler angles are in the order: yaw (Z), pitch (Y), roll (X)
    float cy = cos(euler.x * 0.5f);
    float sy = sin(euler.x * 0.5f);
    float cp = cos(euler.y * 0.5f);
    float sp = sin(euler.y * 0.5f);
    float cr = cos(euler.z * 0.5f);
    float sr = sin(euler.z * 0.5f);

    FoxEngine::FoxMath::Quaternion result;
    result.w = cy * cp * cr + sy * sp * sr;
    result.x = cy * cp * sr - sy * sp * cr;
    result.y = sy * cp * sr + cy * sp * cr;
    result.z = sy * cp * cr - cy * sp * sr;

    return result;
}

Quaternion Quaternion::Lerp(const Quaternion& q0, const Quaternion& q1, float t)
{
    return q0 * (1.0f - t) + (q1 * t);
}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{

    // Find the dot product
    float dot = q0.Dot(q1);
    float q1Scale = 1.0f;
    if (dot < 0.0f)
    {
        dot = -dot;
        q1Scale = -1.0f;
    }

    if (dot > 0.999f)
    {
        return Normalize(Lerp(q0, q1, t));
    }

    float theta = acosf(dot);
    float sintheta = sinf(theta);
    float scale0 = sinf(theta * (1.0f -t)) / sintheta;
    float scale1 = q1Scale * sinf(theta * t) / sintheta;

    Quaternion q =
    {
        (q0.x * scale0) + (q1.x * scale1),
        (q0.y * scale0) + (q1.y * scale1),
        (q0.z * scale0) + (q1.z * scale1),
        (q0.w * scale0) + (q1.w * scale1)
    };

    return Normalize(q);
}