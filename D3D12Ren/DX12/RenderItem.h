#pragma once
#include <climits>

#include "RenderStructure.h"
namespace FRenderer
{
	class Model;
	class RenderItem :public RenderStructure
	{
		static long guid_static;
		long guid;
	public:
		//渲染对象对应的模型
		Model* model;

		RenderItem() noexcept;

		long GetGUID() const noexcept;
	};

	
}