#ifdef _DEBUG
#include "Brofiler.h"
#endif // _DEBUG
#include "../../Globals.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleFont.h"
#include <SDL.h>
#include "../Font.h"
#include <math.h>

using namespace std;

ModuleRender::ModuleRender()
{
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = unique_ptr<SDL_Renderer,SDLRendererDestroyer>(SDL_CreateRenderer(Window->window, -1, flags));
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer.get());
	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	while (!blitQueue.empty())
	{
		BlitStruct temp = blitQueue.top();
		Blit(temp.texture, temp.x, temp.y, &temp.section, &temp.blitSection);

		blitQueue.pop();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	#ifdef _DEBUG
		BROFILER_CATEGORY("Render", Profiler::Color::Orchid)
	#endif // _DEBUG
	SDL_RenderPresent(renderer.get());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer)
	{
		SDL_DestroyRenderer(renderer.get());
	}

	return true;
}

void ModuleRender::AddToBlitBuffer(SDL_Texture* texture,const float& x,const float& y, const Layer& layer, SDL_Rect* section, ResizeStruct* resizeInfo) {
	
	ASSERT(texture != nullptr,AT("Texture parameter was received as null"));

	SDL_Rect rect = { 0,0,0,0 };
	ResizeStruct resize= { 0,0 };
	if(section)
	{
		rect = SDL_Rect(*section);
	}
	if(resizeInfo)
	{
		resize = ResizeStruct(*resizeInfo);
	}

	BlitStruct temp;
	temp = { texture,x,y,layer,rect,resize };
	
	blitQueue.push(temp);
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture,float x,float y, SDL_Rect* section, ResizeStruct* resizeInfo)
{
	ASSERT(texture,AT("Texture parameter was received as null"));

	SDL_Rect rect;

	if(resizeInfo && *resizeInfo != emptyResizeStruct)
	{
		rect.w = resizeInfo->w;
		rect.h = resizeInfo->h;
	}
	else if (section && SDL_RectEmpty(section)) {
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
	}

	//Center (0,0) is in the mid of the window
	x = x + (SCREEN_WIDTH / 2);
	y = (SCREEN_HEIGHT/2) - y;

	//Now we calculate the left-top point where the image should start
	rect.x =static_cast<int>(x - (rect.w / 2));
	rect.y = static_cast<int>(y - (rect.h / 2));

	if(SDL_RenderCopy(renderer.get(), texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool ModuleRender::Print(const Font* font,const float& x,const float& y,const string& mesage, float fontSize) 
{
	ASSERT(font != nullptr,AT("Font parameter received as null"));

	if (fontSize < 0) {
		fontSize = 0;
	}

	SDL_Texture* tempTexture = Fonts->GetMessage(font,mesage);
	ASSERT(tempTexture != nullptr,AT("Failed on creating new message texture"));

	SDL_Rect rect;
	SDL_QueryTexture(tempTexture, NULL, NULL, &rect.w, &rect.h);
	ResizeStruct size = { (int)(rect.w*fontSize),(int)(rect.h*fontSize) };

	AddToBlitBuffer(tempTexture, x, y, Layer::UI, nullptr, &size);
	return true;
}

bool ModuleRender::DirectPrint(const Font* font,const float& x,const float& y,const string& mesage, float fontSize) 
{
	ASSERT(font != nullptr, AT("Font parameter received as null"));

	if (fontSize < 0) {
		fontSize = 0;
	}

	SDL_Texture* tempTexture = Fonts->GetMessage(font, mesage);
	ASSERT(tempTexture != nullptr, AT("Failed on creating new message texture"));

	SDL_Rect rect;
	SDL_QueryTexture(tempTexture, NULL, NULL, &rect.w, &rect.h);
	ResizeStruct size = { (int)(rect.w*fontSize),(int)(rect.h*fontSize) };

	Blit(tempTexture, x, y, nullptr, &size);

	return true;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Rect tempRect;

	//Center (0,0) is in the mid of the window
	tempRect.x = rect.x + (SCREEN_WIDTH / 2);
	tempRect.y = (SCREEN_HEIGHT / 2) - rect.y;

	//Now we calculate the left-top point where the image should start
	tempRect.x = tempRect.x - (rect.w / 2);
	tempRect.y = tempRect.y - (rect.h / 2);
	tempRect.w = rect.w;
	tempRect.h = rect.h;


	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);

	if (SDL_RenderFillRect(renderer.get(), &tempRect) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool ModuleRender::DrawQuads(const SDL_Rect rects[],const int& count, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);

	if (SDL_RenderFillRects(renderer.get(), rects, count) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRects error: %s", SDL_GetError());
		return false;
	}

	return true;
}
