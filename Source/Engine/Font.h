#pragma once
#include <map>
#include <vector>
#include <SDL.h>

class Font {
public:
	Font(std::string name, int xSize, int ySize, std::vector<char> chars);
	~Font();

	SDL_Surface* GetImage() const;
	std::string GetName() const;
	int GetXSize() const;
	void SetXSize(const int& x);
	int GetYSize() const;
	void SetYSize(const int& y);
	int GetVocabSize()const;
	int GetCharOffset(char c) const;
	void SetOnlyCaps(bool value);
	bool IsOnlyCaps()const;
private:
	std::string name;
	int xSize;
	int ySize;
	std::map<char, int> charMap;
	SDL_Surface* surface=nullptr;
	bool onlyCaps = true;
};

