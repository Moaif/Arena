#pragma once
#include "Module.h"
#include <map>

class Font;
struct SDL_Texture;

struct CacheInfo {
	SDL_Texture* texture;
	int timeToLive;
};

class ModuleFont : public Module
{
public:
	ModuleFont();
	~ModuleFont();

	bool Init()override;
	bool CleanUp()override;
	update_status PreUpdate()override;

	const Font* GetFont(const std::string& fontName, const std::string& file, const int& line);
	void FreeFont(const Font ** p,const std::string& file,const int& line);
	bool VerifyLinks();
	SDL_Texture* GetMessage(const Font* font,const std::string& message);

private:
	bool LoadFontRed();
	bool LoadFontBlue();
	bool LoadFontGreen();
	bool LoadFontYellow();
	SDL_Texture* CreateMessage(const Font* font,const std::string& message);

private:
	std::map<std::string, Font*> fontMap;
	std::map<const Font*, std::map<std::string, std::map<int, bool>>> links;
	std::map <const Font*, std::map<std::string, CacheInfo>> messageCache;
	static const int MAX_TIME_TO_LIVE;
	static const int MAX_CACHE_SIZE_PER_FONT;

};
