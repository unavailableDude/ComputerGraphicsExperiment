#include <../include/ICoordable.hpp>


ICoordable::ICoordable() : coords(0, 0){}
ICoordable::ICoordable(int x, int y) : coords(x, y){}

void ICoordable::SetPos(int x, int y){
	coords = Vec2int(x, y);
}

void ICoordable::MovePos(int dx, int dy){
	coords = Vec2int(coords.x + dx, coords.y + dy);
}