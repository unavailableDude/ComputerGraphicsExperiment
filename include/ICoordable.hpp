#pragma once
#include <Vec2int.hpp>


class ICoordable{
	protected:
	Vec2int _coords;
	
	public:
	virtual ~ICoordable(){}
	ICoordable();
	ICoordable(int x, int y);
	
	virtual void SetPos(int x, int y) = 0;
	virtual void MovePos(int dx, int dy) = 0;
};