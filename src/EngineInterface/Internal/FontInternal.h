#pragma once

namespace Engine
{
	struct Glyph
	{
		int id;
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int advance;
	};

	inline bool operator<(const Glyph& glyph, int id) { return glyph.id < id; }
	inline bool operator<(int id, const Glyph& glyph) { return id < glyph.id; }

	extern Glyph Font[191];
}
