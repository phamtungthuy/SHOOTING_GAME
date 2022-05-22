#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>
#include <fstream>
using namespace std;

namespace SDLCommonFunc {
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

using namespace SDLCommonFunc;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

enum TypeMove{
    STATIC_THREAT = 70,
    MOVE_IN_SPACE_THREAT = 71,
    MOVE_IN_SKY_THREAT = 72,

};

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
static Mix_Music* g_music = NULL;
static Mix_Chunk* g_chunk = NULL;
static TTF_Font* font_menu = NULL;
static int time_over = 0;
const int MAXSHOW = 2;



const int FRAME_PER_SECOND = 40;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;
const SDL_Color REDCOLOR = {255, 0, 0};
const SDL_Color BLUECOLOR = {0, 0, 255};
const SDL_Color GREENCOLOR = {0, 255, 0};
const SDL_Color BLACKCOLOR = {0, 0, 0};
const SDL_Color WHITECOLOR = {255, 255, 255};
const int RENDER_DRAW_COLOR = 0xff;

const string WINDOW_TITLE = "Game Cpp SDL 2.0 - Pham Tung Thuy";
#define BLANK_TILE 0
#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 12

#define STATE_MONEY 4
#define DESTINATION 10
#define PLAYER_SPEED 8

void logSDLError(ostream& os, const string &msg, bool fatal = false);
void loadChunk(string path, Mix_Chunk* &chunk);
void LoadText(SDL_Renderer* renderer, TTF_Font* font, string str, SDL_Color color, SDL_Texture* &texture, SDL_Rect &rect);

typedef struct Map {
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

typedef struct Input {
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
    int add_bullet_;
};


#endif // COMMON_FUNCTION_H_
