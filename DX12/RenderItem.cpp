#include"RenderItem.h"
#include"Model.h"

namespace FRenderer
{
	long RenderItem::guid_static = LONG_MIN;
	RenderItem::RenderItem() noexcept
		:guid(guid_static++), model(nullptr)
	{

	}



	long RenderItem::GetGUID() const noexcept
	{
		return guid;
	}

}