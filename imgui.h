#ifndef IMGUI_H
#define IMGUI_H

#include "SDL.h"
#include "rect.h"
#include "vec.h"
#include <string>

namespace imgui {
	bool Button(Rect rect, const std::string text);
	void Label(Veci pos, const std::string text, bool center=false);
}

#endif