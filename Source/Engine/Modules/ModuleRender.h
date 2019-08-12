#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "../../Globals.h"
#include "../Point.h"
#include <queue>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class Font;


struct resizeStruct {
	int w;
	int h;
};

struct BlitStruct
{
	SDL_Texture* texture;
	float x;
	float y;
	float z;
	SDL_Rect section;
	resizeStruct blitSection;
	bool sectionNull;
	bool blitSectionNull;
};

struct CompareDepth {
	bool operator()(BlitStruct const & p1, BlitStruct const & p2) {
		return p1.z < p2.z;
	}
};

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init()override;
	update_status PreUpdate()override;
	update_status Update()override;
	update_status PostUpdate()override;
	bool CleanUp()override;

	void AddToBlitBuffer(SDL_Texture* texture, const float& x, const float& y, const float& z, SDL_Rect* section, resizeStruct* resizeInfo);
	bool Blit(SDL_Texture* texture, float x, float y, SDL_Rect* section, resizeStruct* resizeInfo);
	bool Print(const Font* font,const float& x, const float& y,const std::string& message, float fontSize = 1);
	bool DirectPrint(const Font* font,const  float& x,const float& y,const std::string& message, float fontSize = 1);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool DrawQuads(const SDL_Rect rects[],const int& count, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

public:
	SDL_Renderer* renderer = nullptr;
	int nearClippingPlane;

private:
	//Depth buffer
	std::priority_queue<BlitStruct,std::vector<BlitStruct>,CompareDepth> blitQueue;
};

#endif // __MODULERENDER_H__