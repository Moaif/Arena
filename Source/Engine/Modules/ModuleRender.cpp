#include "../../Globals.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleFont.h"
#include <SDL.h>
#include <iostream>
#include "../Font.h"
#include <math.h>
#ifdef _DEBUG
#include "Brofiler.h"
#endif // _DEBUG

using namespace std;



ModuleRender::ModuleRender()
{
	nearClippingPlane = 2;

}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	while (!blitQueue.empty())
	{
		BlitStruct temp = blitQueue.top();
		if(temp.isBlitSectionValid && temp.isSectionValid)
		{
			Blit(temp.texture, temp.x, temp.y, &temp.section, &temp.blitSection);
		}
		else if(temp.isSectionValid)
		{
			Blit(temp.texture, temp.x, temp.y, &temp.section,nullptr);
		}
		else if(temp.isBlitSectionValid)
		{
			Blit(temp.texture, temp.x, temp.y, nullptr, &temp.blitSection);
		}
		else
		{
			Blit(temp.texture, temp.x, temp.y, nullptr, nullptr);
		}

		blitQueue.pop();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	#ifdef _DEBUG
		BROFILER_CATEGORY("Render", Profiler::Color::Orchid)
	#endif // _DEBUG
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

void ModuleRender::AddToBlitBuffer(SDL_Texture* texture,const float& x,const float& y, const Layer& layer, SDL_Rect* section, resizeStruct* resizeInfo) {
	
	ASSERT(texture != nullptr,AT("Texture parameter was received as null"));

	SDL_Rect empty = { 0,0,0,0 };
	resizeStruct noResize= { 0,0 };
	BlitStruct temp;
	temp = { texture,x,y,layer,*section,*resizeInfo,section!=nullptr,resizeInfo!=nullptr };
	
	blitQueue.push(temp);
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture,float x,float y, SDL_Rect* section, resizeStruct* resizeInfo)
{

	ASSERT(texture != nullptr,AT("Texture parameter was received as null"));

	bool ret = true;
	SDL_Rect rect;

	if(resizeInfo)
	{
		rect.w = resizeInfo->w;
		rect.h = resizeInfo->h;
	}
	else if (section) {
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

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::Print(const Font* font,const float& x,const float& y,const string& mesage, float fontSize) {

	ASSERT(font != nullptr,AT("Font parameter received as null"));

	bool ret = true;

	if (fontSize < 0) {
		fontSize = 0;
	}

	SDL_Texture* tempTexture = App->fonts->GetMessage(font,mesage);
	ASSERT(tempTexture != nullptr,AT("Failed on creating new message texture"));

	SDL_Rect rect;
	SDL_QueryTexture(tempTexture, NULL, NULL, &rect.w, &rect.h);
	resizeStruct size = { (int)(rect.w*fontSize),(int)(rect.h*fontSize) };

	AddToBlitBuffer(tempTexture, x, y, Layer::UI, nullptr, &size);
	return ret;
}

bool ModuleRender::DirectPrint(const Font* font,const float& x,const float& y,const string& mesage, float fontSize) {

	ASSERT(font != nullptr, AT("Font parameter received as null"));

	bool ret = true;

	if (fontSize < 0) {
		fontSize = 0;
	}

	SDL_Texture* tempTexture = App->fonts->GetMessage(font, mesage);
	ASSERT(tempTexture != nullptr, AT("Failed on creating new message texture"));

	SDL_Rect rect;
	SDL_QueryTexture(tempTexture, NULL, NULL, &rect.w, &rect.h);
	resizeStruct size = { (int)(rect.w*fontSize),(int)(rect.h*fontSize) };

	Blit(tempTexture, x, y, nullptr, &size);

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;
	SDL_Rect tempRect;

	//Center (0,0) is in the mid of the window
	tempRect.x = rect.x + (SCREEN_WIDTH / 2);
	tempRect.y = (SCREEN_HEIGHT / 2) - rect.y;

	//Now we calculate the left-top point where the image should start
	tempRect.x = tempRect.x - (rect.w / 2);
	tempRect.y = tempRect.y - (rect.h / 2);
	tempRect.w = rect.w;
	tempRect.h = rect.h;


	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderFillRect(renderer, &tempRect) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuads(const SDL_Rect rects[],const int& count, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderFillRects(renderer, rects, count) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRects error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
