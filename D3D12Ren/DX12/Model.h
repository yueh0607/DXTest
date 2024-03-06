#pragma once
#include<vector>
#include"MeshGeometry.h"


namespace FRenderer
{
	class Model
	{
	public:
		std::vector<std::shared_ptr<MeshGeometry>> meshes;


	};
}