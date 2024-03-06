#pragma once
#include<DirectXMath.h>

#include <d3d12.h>
#include <d3dx12.h>

namespace MathHelper
{
	const float Infinity = FLT_MAX;
	const float PI = 3.1415926535f;
	using namespace DirectX;

	static DirectX::XMFLOAT4X4 Identity4x4()
	{
		static DirectX::XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}

	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	static int Rand(int a, int b)
	{
		return a + rand() % ((b - a) + 1);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	//计算逆转置矩阵（仅适用于4x4）
	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{

		XMMATRIX A = M;
		//将A的最后一行置为(0,0,0,1)
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		//计算A的行列式
		XMVECTOR det = XMMatrixDeterminant(A);
		//求逆转置
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMFLOAT3 Substract(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
	}


	/// <summary>
	/// 加等于
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	static void AddEqual(XMFLOAT3& a, const XMFLOAT3& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	}

	
}


