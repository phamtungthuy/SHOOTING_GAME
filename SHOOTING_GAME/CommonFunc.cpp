#include "CommonFunc.h"
#include "TextObject.h"

void logSDLError(ostream& os, const string &msg, bool fatal) {
    os << msg << "Error: " << SDL_GetError() << endl;
    if(fatal) {
        SDL_Quit();
        exit(1);
    }
}

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) {
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // Case 1: size object 1 < size object 2
    if (left_a >= left_b && left_a <= right_b)
    {
    if (top_a >= top_b && top_a <= bottom_b)
    {
      return true;
    }
    }

    if (left_a >= left_b && left_a <= right_b)
    {
    if (bottom_a >= top_b && bottom_a <= bottom_b)
    {
      return true;
    }
    }

    if (right_a >= left_b && right_a <= right_b)
    {
    if (top_a >= top_b && top_a <= bottom_b)
    {
      return true;
    }
    }

    if (right_a >= left_b && right_a <= right_b)
    {
    if (bottom_a >= top_b && bottom_a <= bottom_b)
    {
      return true;
    }
    }

    // Case 2: size object 1 < size object 2
    if (left_b >= left_a && left_b <= right_a)
    {
    if (top_b >= top_a && top_b <= bottom_a)
    {
      return true;
    }
    }

    if (left_b >= left_a && left_b <= right_a)
    {
    if (bottom_b >= top_a && bottom_b <= bottom_a)
    {
      return true;
    }
    }

    if (right_b >= left_a && right_b <= right_a)
    {
    if (top_b >= top_a && top_b <= bottom_a)
    {
      return true;
    }
    }

    if (right_b >= left_a && right_b <= right_a)
    {
    if (bottom_b >= top_a && bottom_b <= bottom_a)
    {
      return true;
    }
    }

    // Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
    return true;
    }

    return false;
}


void loadChunk(string path, Mix_Chunk* &chunk) {
    chunk = Mix_LoadWAV(path.c_str());
    if(chunk == NULL) {
        logSDLError(cout, "Failed to load chunk", true);
    }
}


void LoadText(SDL_Renderer* renderer, TTF_Font* font, string str, SDL_Color color, SDL_Texture* &texture, SDL_Rect &rect) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str.c_str(), color);
    if(text_surface) {
        texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        rect.w = text_surface->w;
        rect.h = text_surface->h;
        SDL_FreeSurface(text_surface);
    }
    if(texture == NULL) {
        logSDLError(cout, "Load Text", true);
    }

}

