#include <SDL2/SDL.h>


class CGLib{
public:
	static void DrawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2);
	static void SlopeLine(SDL_Renderer *r, int x1, int y1, int x2, int y2);
	static void DDALineStep(SDL_Renderer *r, int x1, int y1, int x2, int y2, int Exch);
	static void DDALine(SDL_Renderer *r, int x1, int y1, int x2, int y2);
	static void BresenhamLine(SDL_Renderer *r, int x1,int y1,int x2,int y2);

	static void LineRect(SDL_Renderer *r, int cx, int cy, int width, int height);
	static void LineRectFilled(SDL_Renderer *r, int cx, int cy, int width, int height);

	static void CircleEqu(SDL_Renderer *r, int xc, int yc, int radius);
	static void CirclePolar(SDL_Renderer *r, int xc, int yc, int radius);

	static void CircleFilled(SDL_Renderer *r, int xc, int yc, int radius);	//no impl

	static void CircleLines(SDL_Renderer *r, int xc, int yc, int radius);
};