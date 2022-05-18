#ifndef PLAY_GAME_H_
#define PLAY_GAME_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include "PlaneThreats.h"
#include "BossObject.h"

static bool isQuit = false;
static bool fatal = true;
static bool firtStep = true;
bool InitData();
void HandleMouse(SDL_Event e, SDL_Rect rect, TextObject &text);
bool loadBackground();
void close();
vector<ThreatsObject*> MakeThreadList();
vector<Plane*> MakePlaneList();
void HandleConflict(MainObject &p_player, vector<auto> &threats_list, ExplosionObject &exp_threat);
void HandleBulletThreats(MainObject &p_player, vector<auto> &threats_list, ExplosionObject &exp_main, Map& map_data, PlayerPower &player_power);
void showMenu();
void HandlePlayer(MainObject& p_player, Map& map_data, PlayerPower& player_power);
void drawTextShow(const MainObject &p_player, PlayerMoney& player_money);
void playGame();
void showBox();

#endif // PLAY_GAME_H_

