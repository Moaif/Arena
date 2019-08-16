#include "Font.h"
#include <SDL_image.h>
#include <ctype.h>

using namespace std;

Font::Font(string name, int xSize, int ySize, vector<char> chars) :
	m_name(name), m_xSize(xSize), m_ySize(ySize)
{
	string path = "assets/fonts/" + name + ".png";
	m_surface = IMG_Load(path.c_str());
	int index = 0;
	for (vector<char>::iterator it = chars.begin(); it != chars.end(); ++it) {
		m_charMap[(char)*it] = index;
		++index;
	}
}

Font::~Font() {
	SDL_FreeSurface(m_surface);
	m_charMap.clear();
}

SDL_Surface* Font::GetImage() const {
	return m_surface;
}

string Font::GetName() const {
	return m_name;
}

int Font::GetXSize() const {
	return m_xSize;
}
void Font::SetXSize(const int& x) {
	m_xSize = x;
}
int Font::GetYSize() const {
	return m_ySize;
}
void Font::SetYSize(const int& y) {
	m_ySize = y;
}

int Font::GetVocabSize()const {
	return m_charMap.size();
}

int Font::GetCharOffset(char c) const {
	if (m_onlyCaps) {
		c=toupper(c);
		
	}
	return m_charMap.at(c);
}

void Font::SetOnlyCaps(bool value) {
	m_onlyCaps = value;
}

bool Font::IsOnlyCaps()const {
	return m_onlyCaps;
}