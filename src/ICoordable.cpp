#include <../include/ICoordable.hpp>


ICoordable::ICoordable() : _coords(0, 0){}
ICoordable::ICoordable(int x, int y) : _coords(x, y){}

void ICoordable::SetPos(int x, int y){
	_coords = Vec2int(x, y);
}

void ICoordable::MovePos(int dx, int dy){
	_coords = Vec2int(_coords._x + dx, _coords._y + dy);
}