#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include<DirectXMath.h>
#include<DirectXCollision.h>

struct SubmeshGeometry
{
	/// <summary>
	/// 顶点的数量
	/// </summary>
	UINT IndexCount = 0;

	/// <summary>
	/// 索引开始位置
	/// </summary>
	UINT StartIndexLocation = 0;

	/// <summary>
	/// 顶点开始位置
	/// </summary>
	INT BaseVertexLocation = 0;

	//DirectX::BoundingBox Bounds;
};