#ifndef _Font_
#define _Font_

#include <map>
#include <vector>
#include <SDL.h>

using namespace std;

class Font {
public:
	Font(std::string name, int xSize, int ySize, vector<char> chars);
	~Font();

	SDL_Surface* GetImage() const;
	string GetName() const;
	int GetXSize() const;
	void SetXSize(const int& x);
	int GetYSize() const;
	void SetYSize(const int& y);
	int GetVocabSize()const;
	int GetCharOffset(char c) const;
	void SetOnlyCaps(bool value);
	bool GetOnlyCaps()const;
private:
	string name;
	int xSize;
	int ySize;
	map<char, int> charMap;
	SDL_Surface* surface=nullptr;
	bool onlyCaps = true;
};

#endif // !_Font_

