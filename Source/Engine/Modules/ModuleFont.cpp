#include "../../Globals.h"
#include "../Application.h"
#include "ModuleFont.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "../Font.h"
#include <SDL.h>

using namespace std;

const int ModuleFont::MAX_TIME_TO_LIVE = 25;
const int ModuleFont::MAX_CACHE_SIZE_PER_FONT = 1000;

ModuleFont::ModuleFont() {

}

ModuleFont::~ModuleFont() {

}

bool ModuleFont::Init() {
	LOG("Loading Fonts");

	if (!LoadFontRed()) {
		LOG("Can not load Red font from its path");
		return false;
	}
	if (!LoadFontBlue()) {
		LOG("Can not load Blue font from its path");
		return false;
	}
	if (!LoadFontGreen()) {
		LOG("Can not load Green font from its path");
		return false;
	}
	if (!LoadFontYellow()) {
		LOG("Can not load Yellow font from its path");
		return false;
	}
	return true;
}

bool ModuleFont::CleanUp() {
	LOG("Unloading all fonts");
	if(!VerifyLinks()){
		LOG("Some files didn't freed their fonts");
	}
	fontMap.clear();
	links.clear();
	messageCache.clear();
	return true;
}

update_status ModuleFont::PreUpdate() {
	for (map<const Font*, map<string, CacheInfo>>::iterator it = messageCache.begin(); it != messageCache.end(); ++it) {
		for (map<string, CacheInfo>::iterator it2 = (*it).second.begin(), next_it2= (*it).second.begin(); it2 != (*it).second.end(); it2 = next_it2) {
			next_it2 = it2;
			++next_it2;
			if ((--(*it2).second.timeToLive) <= 0) {
				if ((*it2).second.texture != nullptr) {
					SDL_DestroyTexture((*it2).second.texture);
				}
				(*it2).second.texture = nullptr;
				(*it).second.erase(it2);
			}
		}
	}

	return UPDATE_CONTINUE;
}


const Font* ModuleFont::GetFont(const string& fontName, const string& file,const int& line) {
	if (fontMap.count(fontName)) {
		links[fontMap[fontName]][file][line] = true;
		return fontMap[fontName];
	}
	LOG("Error, there is no %s in our database", fontName.c_str());
	return nullptr;
}

void ModuleFont::FreeFont(const Font ** p,const string& file,const int& line) {
	ASSERT(p != nullptr, AT("FreeFont received a null Font"));

	ASSERT(*p != nullptr, AT("FreeFont received a null *Font"));

	map<const Font*, map<string, map<int, bool>>>::iterator it = links.find(*p);
	if (it != links.end()) {
		map<string, map<int, bool>>::iterator it2 = (*it).second.find(file);
		if (it2 != (*it).second.end()) {
			map<int, bool>::iterator it3 = (*it2).second.find(line);
			if (it3 != (*it2).second.end()) {
				(*it3).second = false;
				*p = nullptr;
			}
		}
	}
}

bool ModuleFont::VerifyLinks() {

	bool ret = true;

	for (map<string, Font*>::iterator it = fontMap.begin(); it != fontMap.end(); ++it) {


		for (map<string, map<int, bool>>::iterator it2 = links[it->second].begin(); it2 != links[it->second].end(); ++it2) {
			for (map<int, bool>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
				if (it3->second) {
					string err= "La font " + it->first + " no se pudo liberar por el fichero " + it2->first + " linea %d";
					LOG(err.c_str(),it3->first);
					ret = false;
				}
			}
		}

		delete it->second;
	}
	return ret;
}

SDL_Texture* ModuleFont::GetMessage(const Font* font,const string& message) {
	
	ASSERT(font != nullptr,AT("Font parameter was received as null"));

	//First we search if the message is in cache
	if (!messageCache.empty()) {
		map<const Font*, map<string, CacheInfo>>::iterator tempIt = messageCache.find(font);
		if (tempIt != messageCache.end()) {
			if (!(*tempIt).second.empty()) {
				map<string, CacheInfo>::iterator tempIt2 = (*tempIt).second.find(message);
				if (tempIt2 != (*tempIt).second.end()) {
					(*tempIt2).second.timeToLive = MAX_TIME_TO_LIVE;
					return (*tempIt2).second.texture;
				}
			}
		}
	}

	//If we dont fint it, we made it
	SDL_Texture* temp = CreateMessage(font,message);
	ASSERT(temp != nullptr,AT("Failed on creating new message texture"));
	if (messageCache[font].size() > MAX_CACHE_SIZE_PER_FONT) {//Cache full case
		int minTtl= MAX_TIME_TO_LIVE+1;
		string messageToRemove="";
		for (map<string, CacheInfo>::iterator it = messageCache[font].begin(); it != messageCache[font].end(); ++it) {
			if ((*it).second.timeToLive < minTtl) {
				minTtl = (*it).second.timeToLive;
				messageToRemove = (*it).first;
			}
		}
		//Error
		ASSERT(messageToRemove != "",AT("Could not find any space to allocate the new message"));

		SDL_DestroyTexture(messageCache[font][messageToRemove].texture);
		messageCache[font][messageToRemove].texture=nullptr;
		messageCache[font].erase(messageCache[font].find(messageToRemove));
		messageCache[font][message] = { temp,MAX_TIME_TO_LIVE };
	}
	else {
		messageCache[font][message] = { temp,MAX_TIME_TO_LIVE };
	}

	return temp;
}

bool ModuleFont::LoadFontRed() {
	string fontName = "Red";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' '};
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}
bool ModuleFont::LoadFontBlue() {
	string fontName = "Blue";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' ' };
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}

bool ModuleFont::LoadFontGreen() {
	string fontName = "Green";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' ' };
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}

bool ModuleFont::LoadFontYellow() {
	string fontName = "Yellow";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' ' };
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}

SDL_Texture* ModuleFont::CreateMessage(const Font* font,const string& message) {

	ASSERT(font != nullptr,AT("Font parameter was received as null"));

	int xSize = font->GetXSize();
	int ySize = font->GetYSize();

	SDL_Surface* tempSurface = font->GetImage();
	SDL_Surface* surfaceFinal = SDL_CreateRGBSurface(0, message.length() * xSize, ySize, 32, 0, 0, 0, 0);
	SDL_FillRect(surfaceFinal, NULL, 0xFF00FF);
	SDL_SetColorKey(surfaceFinal, SDL_TRUE, SDL_MapRGB(surfaceFinal->format, 255, 0, 255));

	SDL_Rect srcrect;
	srcrect.h = ySize;
	srcrect.w = xSize;
	SDL_Rect dstrect;
	dstrect.h = ySize;
	dstrect.w = xSize;
	if (tempSurface == nullptr)
	{
		LOG("Unable to create a temporal SDL surface for message SDL Error: %s\n", SDL_GetError());
		return nullptr;
	}
	for (unsigned int i = 0; i < message.size(); ++i) {
		int offset = font->GetCharOffset(message[i]);
		srcrect.x = offset*xSize;
		srcrect.y = 0;
		dstrect.x = i*xSize;
		dstrect.y = 0;
		SDL_BlitSurface(tempSurface, &srcrect, surfaceFinal, &dstrect);
	}
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Renderer->GetRenderer(), surfaceFinal);
	if (tempTexture == nullptr) {
		LOG("Unable to create message texture from surface SDL Error: %s\n", SDL_GetError());
		return nullptr;
	}

	SDL_FreeSurface(surfaceFinal);
	return tempTexture;
}
