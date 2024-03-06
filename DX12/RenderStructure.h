#pragma once

namespace FRenderer
{
//禁用拷贝构造函数
#define NO_COPY_CTOR(DATA) DATA(const DATA& other)=delete;
//禁用赋值拷贝构造函数
#define NO_ASSIGN_OPERATOR(DATA) DATA& operator=(const DATA& other)=delete;
//禁用拷贝构造和赋值拷贝构造函数
#define NO_COPY(DATA) NO_COPY_CTOR(DATA) NO_ASSIGN_OPERATOR(DATA)

//无移动构造
#define NO_MOVE_CTOR(DATA) DATA(DATA&& other)=delete;
//无移动赋值构造
#define NO_MOVE_ASSIGN_OPERATOR(DATA) DATA& operator=(DATA&& other)=delete;
//无移动构造和无移动赋值构造
#define NO_MOVE(DATA) NO_MOVE_CTOR(DATA) NO_MOVE_ASSIGN_OPERATOR(DATA)
//没有拷贝和移动
#define NO_COPY_AND_MOVE(DATA) NO_COPY(DATA) NO_MOVE(DATA)


	//渲染结构基类，所有渲染结构继承该类
	class RenderStructure
	{
	public:
		
		RenderStructure() = default;
		~RenderStructure() = default;

		NO_COPY_AND_MOVE(RenderStructure)
	};
}
