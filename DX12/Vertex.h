#pragma once

#include<DirectXMath.h>

struct Vertex
{
	//位置
	DirectX::XMFLOAT3 Pos;
	//颜色
	DirectX::XMFLOAT4 Color;
	//法线
	DirectX::XMFLOAT3 Normal;
};