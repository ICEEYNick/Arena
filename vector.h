#pragma once

#define M_PI 3.14159265358979323846264338327950288419716939937510

struct Vector3
{
public:
	float x;
	float y;
	float z;
	inline Vector3() : x(0.f), y(0.f), z(0.f) {}
	inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	inline Vector3 ToFRotator()
	{
		float Radpi = (float)(180.f / 3.1415926f);
		float Yaw = atan2f(y, x) * Radpi;
		float Pitch = atan2f(z, sqrtf((x * x) + (y * y))) * Radpi;
		float Roll = 0.f;
		return Vector3(Pitch, Yaw, Roll);
	}
	inline Vector3 Clamp()
	{
		if (x > 180.0f)
			x -= 360.0f;
		else if (x < -180.0f)
			x += 360.0f;

		if (y > 180.0f)
			y -= 360.0f;
		else if (y < -180.0f)
			y += 360.0f;

		if (x < -89.0f) { x = -89.0f; }

		if (x > 89.0f) { x = 89.0f; }

		while (y < -180.0f) { y += 360.0f; }

		while (y > 180.0f) { y -= 360.0f; }
		z = 0.0f;
		return Vector3(x, y, z);
	}
	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
	inline float Distance(Vector3 v)
	{
		return sqrtf((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
	}
	inline float Length()
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}
	inline float LengthWithoutZ() {
		return sqrtf((x * x) + (y * y));
	}
	inline Vector3 operator*(float v) const
	{
		return Vector3(x * v, y * v, z * v);
	}
	inline Vector3 operator/(float v) const
	{
		return Vector3(x / v, y / v, z / v);
	}
	inline Vector3 operator+(const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	inline Vector3 operator-(const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	inline Vector3& operator+=(const Vector3& v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}
	inline Vector3& operator-=(const Vector3& v)
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	inline Vector3& operator*=(const Vector3& v)
	{
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}
	inline Vector3& operator/=(const Vector3& v)
	{
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}
};
struct Vector2
{
public:
	float x;
	float y;
	inline Vector2() : x(0), y(0) {}
	inline Vector2(float x, float y) : x(x), y(y) {}
	inline float Distance(Vector2 v)
	{
		return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
	}
	inline Vector2 operator+(const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}
	inline Vector2 operator-(const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}
};
struct Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	inline Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
	inline Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};
D3DXMATRIX ToMatrix(Vector3 Rotation, Vector3 origin = Vector3(0, 0, 0));
D3DXMATRIX ToMatrix(Vector3 Rotation, Vector3 origin)
{
	float Pitch = (Rotation.x * float(M_PI) / 180.f);
	float Yaw = (Rotation.y * float(M_PI) / 180.f);
	float Roll = (Rotation.z * float(M_PI) / 180.f);

	float SP = sinf(Pitch);
	float CP = cosf(Pitch);
	float SY = sinf(Yaw);
	float CY = cosf(Yaw);
	float SR = sinf(Roll);
	float CR = cosf(Roll);

	D3DXMATRIX Matrix;
	Matrix._11 = CP * CY;
	Matrix._12 = CP * SY;
	Matrix._13 = SP;
	Matrix._14 = 0.f;

	Matrix._21 = SR * SP * CY - CR * SY;
	Matrix._22 = SR * SP * SY + CR * CY;
	Matrix._23 = -SR * CP;
	Matrix._24 = 0.f;

	Matrix._31 = -(CR * SP * CY + SR * SY);
	Matrix._32 = CY * SR - CR * SP * SY;
	Matrix._33 = CR * CP;
	Matrix._34 = 0.f;

	Matrix._41 = origin.x;
	Matrix._42 = origin.y;
	Matrix._43 = origin.z;
	Matrix._44 = 1.f;

	return Matrix;
}
struct FTransform
{
public:
	Vector4 Rotation;
	Vector3 Translation;
	CHAR Data1[4];
	Vector3 Scale3D;
	CHAR Data2[4];
	D3DXMATRIX ToMatrixWithScale()
	{
		D3DXMATRIX m;
		m._41 = Translation.x;
		m._42 = Translation.y;
		m._43 = Translation.z;

		float x2 = Rotation.x + Rotation.x;
		float y2 = Rotation.y + Rotation.y;
		float z2 = Rotation.z + Rotation.z;

		float xx2 = Rotation.x * x2;
		float yy2 = Rotation.y * y2;
		float zz2 = Rotation.z * z2;

		m._11 = (1 - (yy2 + zz2)) * Scale3D.x;
		m._22 = (1 - (xx2 + zz2)) * Scale3D.y;
		m._33 = (1 - (xx2 + yy2)) * Scale3D.z;

		float yz2 = Rotation.y * z2;
		float wx2 = Rotation.w * x2;

		m._32 = (yz2 - wx2) * Scale3D.z;
		m._23 = (yz2 + wx2) * Scale3D.y;

		float xy2 = Rotation.x * y2;
		float wz2 = Rotation.w * z2;

		m._21 = (xy2 - wz2) * Scale3D.y;
		m._12 = (xy2 + wz2) * Scale3D.x;

		float xz2 = Rotation.x * z2;
		float wy2 = Rotation.w * y2;

		m._31 = (xz2 + wy2) * Scale3D.z;
		m._13 = (xz2 - wy2) * Scale3D.x;

		m._14 = 0;
		m._24 = 0;
		m._34 = 0;
		m._44 = 1;
		return m;
	}
};
D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

D3DMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

struct FMinimalViewInfo
{
	Vector3 Location;
	Vector3 Rotation;
	float FOV;
	float                                         ShadowFOV;                                         // 0x001C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         DesiredFOV;                                        // 0x0020(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};
struct FCameraCacheEntry
{
	float TimeStamp;
	char UnknownData00[0xC];
	FMinimalViewInfo POV;
};
Vector3 WorldToScreen(Vector3 WorldLocation, FMinimalViewInfo CameraCacheL)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Rotation = CameraCacheL.Rotation;
	D3DMATRIX tempMatrix = Matrix(Rotation);
	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - CameraCacheL.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float ratio = (float)globals::width / globals::height;
	if (ratio < 4.0f / 3.0f)
		ratio = 4.0f / 3.0f;

	float FovAngle = ratio / (16.0f / 9.0f) * CameraCacheL.ShadowFOV;

	float ScreenCenterX = globals::width / 2.0f;
	float ScreenCenterY = globals::height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	if (Screenlocation.x >= 0 && Screenlocation.x <= globals::width && Screenlocation.y >= 0 && Screenlocation.y <= globals::height)
	{
		return Screenlocation;
	}
	else
	{
		return Vector3(0, 0, 0);
	}
}
