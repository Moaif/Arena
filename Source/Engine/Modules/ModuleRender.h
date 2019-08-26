#pragma once
#include "Module.h"
#include <queue>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class Font;
class Transform;

enum class Layer
{
	Background,
	Game,
	FX,
	UI,
};

struct ResizeStruct {
	int w;
	int h;
	ResizeStruct(){}
	ResizeStruct(int w, int h) : w(w), h(h){}

	bool operator !=(const ResizeStruct& other)
	{
		return other.h != h || other.w != w;
	}
};

static const ResizeStruct emptyResizeStruct = ResizeStruct(0, 0);

struct BlitStruct
{
	SDL_Texture* texture;
	float x;
	float y;
	float rotation;
	Layer layer;
	SDL_Rect section;
	ResizeStruct blitSection;
	SDL_RendererFlip flip;
};

struct CompareLayer {
	bool operator()(BlitStruct const & p1, BlitStruct const & p2) {
		return p1.layer < p2.layer;
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
	bool CleanUp() override;

	void AddToBlitBuffer(SDL_Texture* texture, const Transform& transform, const Layer& layer, SDL_Rect* section, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE);
	void AddToBlitBuffer(SDL_Texture* texture, float x, float y, const Layer& layer, SDL_Rect* section, ResizeStruct* resizeInfo, float rotation = 0, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE);
	bool Blit(SDL_Texture* texture, float x, float y, SDL_Rect* section, ResizeStruct* resizeInfo, float rotation = 0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE);
	bool Print(const Font* font, float x, float y,const std::string& message, float rotation= 0, float fontSize = 1);
	bool DirectPrint(const Font* font,const  float& x,const float& y,const std::string& message, float fontSize = 1);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool DrawQuads(const SDL_Rect rects[],const int& count, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	SDL_Renderer* GetRenderer()const{return m_renderer;};

private:
	//Depth buffer
	std::priority_queue<BlitStruct,std::vector<BlitStruct>,CompareLayer> m_blitQueue;
	SDL_Renderer* m_renderer = nullptr;
};