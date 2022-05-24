#include "PlayGame.h"

BaseObject g_background;
BaseObject background_menu;
UINT mark_value = 0;
TTF_Font* font_time;
static TextObject time_game;
static TextObject mark_game;
static TextObject moneyCount;
Uint32 val_time;
BossObject* p_boss;

bool InitData() {
    fatal = true;
    val_time = 300;

    time_game.SetColor(TextObject::WHITE_TEXT);
    mark_game.SetColor(TextObject::WHITE_TEXT);
    moneyCount.SetColor(TextObject::WHITE_TEXT);

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(cout, "SDL_Init", true);
    int res = SDL_Init(SDL_INIT_VIDEO);
    if(res < 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow(WINDOW_TITLE.c_str(),
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);
    if(g_window == NULL) {
        logSDLError(cout, "CreateWindow", true);
    }
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(g_screen == NULL) {
        logSDLError(cout, "CreateRenderer", true);
    }
    SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) && imgFlags)){
        logSDLError(cout, "InitImage", true);
    }
    if(TTF_Init() == -1) {
        logSDLError(cout, "TTF Init", true);
    }

    font_time = TTF_OpenFont(".\\img\\game resource\\dlxfont_.ttf", 15);
    font_menu = TTF_OpenFont(".\\img\\game resource\\dlxfont_.ttf", 30);
    if(font_time == NULL || font_menu == NULL) {
        logSDLError(cout, "Init font_time", true);
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        logSDLError(cout, "SDL_mixer could not initialize! SDL_mixer Error:\n", true);
    }
    g_music = Mix_LoadMUS(".\\img\\game resource\\backsound.wav");
    if(g_music == NULL) {
        logSDLError(cout, "Failed to load music!\n", true);
    }

    return true;
}

bool loadBackground() {
    bool ret = g_background.LoadImg(".//img//background.png", g_screen);
    if(ret == false)
        return false;
    return true;
}

void close() {
    p_boss->Free();
    background_menu.Free();
    TTF_CloseFont(font_menu);
    Mix_FreeChunk(g_chunk);
    Mix_FreeMusic(g_music);
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}


vector<ThreatsObject*> MakeThreadList() {
    vector<ThreatsObject*> list_threats;

    ThreatsObject* dynamic_threats = new ThreatsObject[20];
    //Creat monsters can move
    for(int i = 0; i < 20; i++) {
        ThreatsObject* p_threat = (dynamic_threats + i);
        if(p_threat != NULL) {
            p_threat->LoadImg(".\\img\\threats5_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i*500);
            p_threat->set_y_pos(200);

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->setAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject* thread_objs = new ThreatsObject[20];
    //Creat static monsters
    for(int i = 0; i < 20; i++) {
        ThreatsObject* p_threat = (thread_objs + i);
        if(p_threat != NULL) {
            p_threat->LoadImg(".\\img\\threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(700 + i*1200);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(STATIC_THREAT);
            p_threat->set_input_left(0);
            p_threat->SetRect(p_threat->get_x_pos(), p_threat->get_y_pos());
            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);
        }
    }

    return list_threats;
}

vector<Plane*> MakePlaneList() {
    vector<Plane*> plane_list;
    Plane* plane = new Plane[10];
    for(int i = 0; i < 10; i++) {
        Plane* p_plane = (plane + i);
        if(p_plane!= NULL) {
            p_plane->LoadImg(".\\img\\plane.png", g_screen);
            p_plane->set_clips();
            p_plane->set_x_pos(2400 + i*2400);
            p_plane->set_y_pos(40);
            p_plane->set_input_left(1);
            p_plane->set_type_move(MOVE_IN_SKY_THREAT);
            p_plane->SetRect(p_plane->get_x_pos(), p_plane->get_y_pos());
            BulletObject* p_bullet = new BulletObject();
            p_plane->InitBullet(p_bullet, g_screen);
            plane_list.push_back(p_plane);
        }
    }
    return plane_list;
}

BossObject* makeBoss() {
    BossObject* boss = new BossObject();
    boss->LoadImg(".\\img\\boss_object.png", g_screen);
    boss->set_clips();
    boss->set_x_pos(25000);
    boss->set_y_pos(100);
    boss->set_type_move(STATIC_THREAT);
    boss->SetRect(boss->get_x_pos(), boss->get_y_pos());
    boss->InitBullet(new BulletObject(), g_screen);
    return boss;
}

void HandleBoss(BossObject* p_boss, MainObject& p_player, Map& map_data, ExplosionObject &exp_main, PlayerPower &player_power) {
    p_boss->SetMapXY(map_data.start_x_, map_data.start_y_);
    p_boss->DoPlayer(map_data, g_screen);
    p_boss->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, map_data, p_player.get_space_x(), p_player.get_space_y());
    p_boss->Show(g_screen);
    if(SDL_GetTicks()/1000 - p_player.get_inf() <= 2 || p_player.get_immortal()) return;
    SDL_Rect rect_player = p_player.GetRectFrame();
    bool bCol1 = false;
    vector<BulletObject*> tBullet_list = p_boss->get_bullet_list();
    //check if all the bullets of the monster collide with the character
    for(int j = 0; j < tBullet_list.size(); ++j) {
        BulletObject* pt_bullet = tBullet_list[j];
        if(pt_bullet != NULL) {
            SDL_Rect rect_bullet = pt_bullet->GetRect();
            bCol1 = CheckCollision(pt_bullet->GetRect(), rect_player);
            if(bCol1 == true) {
                p_boss->RemoveBullet(j);
                break;
            }
        }
    }
    SDL_Rect rect_boss = p_boss->GetRectFrame();
    bool bCol2 = CheckCollision(rect_player, rect_boss);

    if(bCol1 || bCol2) {
        for(int i = 0; i < NUM_FRAME_EXP; i++) {
            int x_pos = p_player.GetRect().x - exp_main.get_frame_width()*0.5;
            int y_pos = p_player.GetRect().y - exp_main.get_frame_height()*0.5;
            exp_main.set_frame(i);
            exp_main.SetRect(x_pos, y_pos);
            exp_main.Show(g_screen);
            SDL_RenderPresent(g_screen);
        }
        loadChunk(".\\img\\game resource\\Explosion+1.wav", g_chunk);
        Mix_PlayChannel(-1, g_chunk, 0);
        player_power.Decrease();
        if(player_power.getPower() < 1) {
                isQuit = true;
                fatal = false;
        }
        else {
            //MessageBox(NULL, "YOU DIE", "HE HE", MB_OK | MB_ICONSTOP);
            p_player.set_come_back_time(60);
            p_player.set_immortal(true);
            p_player.SetRect(0, 0);
            //SDL_Delay(1000);
        }
    }
}

void HandleConflictWithBoss(MainObject &p_player, BossObject* p_boss, ExplosionObject &exp_threat) {
    vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
    //check if all the character's bullets collide with monsters
    for(int r = 0; r < bullet_arr.size(); r++) {
        BulletObject* p_bullet = bullet_arr[r];
        if(p_bullet != NULL) {

            SDL_Rect tRect = p_boss->GetRect();
            tRect.w = p_boss->get_width_frame();
            tRect.h = p_boss->get_height_frame();

            SDL_Rect bRect = p_bullet->GetRect();
            bool bCol = CheckCollision(bRect, tRect);

            if(bCol) {
                for(int i = 0; i < NUM_FRAME_EXP; i++) {
                    int x_pos = p_bullet->GetRect().x - exp_threat.get_frame_width()*0.5;
                    int y_pos = p_bullet->GetRect().y - exp_threat.get_frame_height()*0.5;
                    exp_threat.set_frame(i);
                    exp_threat.SetRect(x_pos, y_pos);
                    exp_threat.Show(g_screen);
                }
                loadChunk(".\\img\\game resource\\Explosion+1.wav", g_chunk);
                Mix_PlayChannel(-1, g_chunk, 0);
                p_player.RemoveBullet(r);
                if(p_boss->get_health() > 0) p_boss->decrease();
                else {
                    mark_value += 200;
                    p_boss->Free();
                    p_boss->set_alive(false);
                }
                break;
            }
        }
    }
}

void HandleConflict(MainObject &p_player, vector<auto> &threats_list, ExplosionObject &exp_threat) {
    vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
    //check if all the character's bullets collide with monsters
    for(int r = 0; r < bullet_arr.size(); r++) {
        BulletObject* p_bullet = bullet_arr[r];
        if(p_bullet != NULL) {

            for(int t = 0; t < threats_list.size();  t++) {
                auto obj_threat = threats_list[t];
                if(obj_threat != NULL) {
                    SDL_Rect tRect = obj_threat->GetRect();
                    tRect.w = obj_threat->get_width_frame();
                    tRect.h = obj_threat->get_height_frame();

                    SDL_Rect bRect = p_bullet->GetRect();
                    bool bCol = CheckCollision(bRect, tRect);

                    if(bCol) {
                        if(obj_threat->get_type_move() == STATIC_THREAT)mark_value += 10;
                        else if(obj_threat->get_type_move() == MOVE_IN_SPACE_THREAT) mark_value += 5;
                        else mark_value += 20;
                        for(int i = 0; i < NUM_FRAME_EXP; i++) {
                            int x_pos = p_bullet->GetRect().x - exp_threat.get_frame_width()*0.5;
                            int y_pos = p_bullet->GetRect().y - exp_threat.get_frame_height()*0.5;
                            exp_threat.set_frame(i);
                            exp_threat.SetRect(x_pos, y_pos);
                            exp_threat.Show(g_screen);
                        }
                        loadChunk(".\\img\\game resource\\Explosion+1.wav", g_chunk);
                        Mix_PlayChannel(-1, g_chunk, 0);
                        p_player.RemoveBullet(r);
                        obj_threat->Free();
                        threats_list.erase(threats_list.begin() + t);
                        break;
                    }
                }
            }
        }
    }

}


void HandleBulletThreats(MainObject &p_player, vector<auto> &threats_list, ExplosionObject &exp_main, Map& map_data, PlayerPower &player_power) {
    for(int i = 0; i < threats_list.size(); i++) {
        auto p_threat = threats_list[i];
        if(p_threat != NULL) {
            if(p_threat->get_x_pos() < 0) {
                p_threat->Free();
                threats_list.erase(threats_list.begin() + i);
                continue;
            }
            p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
            p_threat->ImpMoveType(g_screen);
            p_threat->DoPlayer(map_data, g_screen);
            p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, map_data, p_player.get_space_x(), p_player.get_space_y());
            p_threat->Show(g_screen);
            if(SDL_GetTicks()/1000 - p_player.get_inf() <= 2 || p_player.get_immortal()) continue;
            SDL_Rect rect_player = p_player.GetRectFrame();
            bool bCol1 = false;
            vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
            //check if all bullets collide with the character
            for(int j = 0; j < tBullet_list.size(); ++j) {
                BulletObject* pt_bullet = tBullet_list[j];
                if(pt_bullet != NULL) {
                    SDL_Rect rect_bullet = pt_bullet->GetRect();
                    bCol1 = CheckCollision(pt_bullet->GetRect(), rect_player);
                    if(bCol1 == true) {
                        p_threat->RemoveBullet(j);
                        break;
                    }
                }
            }
            SDL_Rect rect_threat = p_threat->GetRectFrame();
            bool bCol2 = CheckCollision(rect_player, rect_threat);

            if(bCol1 || bCol2) {
                //Load image explosion
                for(int i = 0; i < NUM_FRAME_EXP; i++) {
                    int x_pos = p_player.GetRect().x - exp_main.get_frame_width()*0.5;
                    int y_pos = p_player.GetRect().y - exp_main.get_frame_height()*0.5;
                    exp_main.set_frame(i);
                    exp_main.SetRect(x_pos, y_pos);
                    exp_main.Show(g_screen);
                    SDL_RenderPresent(g_screen);
                }
                loadChunk(".\\img\\game resource\\Explosion+1.wav", g_chunk);
                Mix_PlayChannel(-1, g_chunk, 0);
                player_power.Decrease();
                if(player_power.getPower() < 1) {
                        p_threat->Free();
                        isQuit = true;
                        fatal = false;
                        break;
                }
                else {
                    //MessageBox(NULL, "YOU DIE", "HE HE", MB_OK | MB_ICONSTOP);
                    p_player.set_come_back_time(60);
                    p_player.set_immortal(true);
                    p_player.SetRect(0, 0);
                    //SDL_Delay(1000);
                    break;
                }
            }
        }
    }
}

void showMenu() {
    background_menu.LoadImg(".//img//menu.png", g_screen);
    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 0);
    SDL_RenderClear(g_screen);
    SDL_Event e;
    bool is_quit = false;
    TextObject text[3];
    text[0].SetText("Play Game");
    text[1].SetText("Tutorial");
    text[2].SetText("Exit");
    for(int i = 0; i < 3; i++) {
        int leng = text[i].GetText().length() * 30;
        text[i].SetRect((SCREEN_WIDTH - leng) / 2, 250 + 80*i);
        text[i].SetColor(BLACKCOLOR);
    }
    while(!is_quit) {

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
            case SDL_QUIT:
                close();
                exit(1);
            }
            for(int i = 0; i < 3; i++) {
                HandleMouse(e, text[i].getRect(), text[i]);
            }
        }
        background_menu.Render(g_screen, NULL);
        for(int i = 0; i < 3; i++) {
            text[i].LoadFromRenderText(font_menu, g_screen);
            text[i].RenderText(g_screen, text[i].getPosX(), text[i].getPosY());
        }
        for(int i = 0; i < 3; i++) {
            if(text[i].getType() == BUTTON_SPRITE_MOUSE_DOWN) {
                loadChunk(".\\img\\game resource\\beep_.wav", g_chunk);
                Mix_PlayChannel(-1, g_chunk, 0);
                //SDL_Delay(200);
                if(text[i].GetText() == "Exit") {
                    SDL_Delay(200);
                    close();
                    exit(1);

                }
                else if(text[i].GetText() == "Play Game")is_quit = true;
                else if(text[i].GetText() == "Tutorial") {
                    showTutorial();
                }
            }
        }

        SDL_RenderPresent(g_screen);
        SDL_Delay(100);
    }
}

void showTutorial() {
    SDL_Event e_tutorial;
    SDL_RenderClear(g_screen);
    TextObject sub_text[20];
    sub_text[0].SetText("MOVE: LEFT, RIGHT, UP, DOWN");
    sub_text[1].SetText("JUMP: SPACE");
    sub_text[2].SetText("FIRE: A");
    sub_text[3].SetText("PAUSE: ESC");
    sub_text[4].SetText("Good luck for your adventure!");
    sub_text[5].SetText("|BACK|");
    for(int i = 0; i < 4; i++) {
        int leng = sub_text[i].GetText().length() * 30;
        sub_text[i].SetRect(200, 50 + 80*i);
        sub_text[i].SetColor(BLACKCOLOR);
    }
    sub_text[4].SetRect((SCREEN_WIDTH - sub_text[4].GetText().length() * 30)/2, SCREEN_HEIGHT - 200);
    sub_text[4].SetColor(BLACKCOLOR);
    sub_text[5].SetRect(50, SCREEN_HEIGHT - 50);
    sub_text[5].SetColor(GREENCOLOR);
    bool is_quit = false;
    while(!is_quit) {
        while(SDL_PollEvent(&e_tutorial) != 0) {
            switch(e_tutorial.type) {
                case SDL_QUIT:
                    close();
                    exit(1);
            }
            HandleMouse(e_tutorial, sub_text[5].getRect(), sub_text[5], BLUECOLOR);
        }

        background_menu.Render(g_screen, NULL);
        for(int i = 0; i < 6; i++) {
            sub_text[i].LoadFromRenderText(font_menu, g_screen);
            sub_text[i].RenderText(g_screen, sub_text[i].getPosX(), sub_text[i].getPosY());
        }
        if(sub_text[5].getType() == BUTTON_SPRITE_MOUSE_DOWN) {
             Mix_PlayChannel(-1, g_chunk, 0);
            is_quit = true;
            //SDL_Delay(200);
        }

        SDL_RenderPresent(g_screen);
    }
}

void HandlePlayer(MainObject& p_player, Map& map_data, PlayerPower& player_power) {
    p_player.set_space_x(map_data.start_x_ - p_player.get_map_x());
    p_player.set_space_y(map_data.start_y_ - p_player.get_map_y());
    p_player.HandleBullet(g_screen, map_data);
    p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
    p_player.DoPlayer(map_data);
    p_player.show(g_screen);
    if(p_player.get_decrease()) {
        p_player.set_decrease(false);
        player_power.Decrease();
        if(player_power.getPower() <= 0) {
            p_player.Free();
            isQuit = true;
            fatal = false;
        }
    }
    if(p_player.get_win()) {

        p_player.Free();
        isQuit = true;
        fatal = true;
    }

}

void drawTextShow(const MainObject &p_player, PlayerMoney& player_money) {
    GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
    colorData color_data(0, 80, 150);
    Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

    GeometricFormat outlineSize(1, 1, SCREEN_WIDTH - 1, 38);
    colorData color_data2(255, 255, 255);
    Geometric::RenderOutline(outlineSize, color_data2, g_screen);
    Uint32 time_val = SDL_GetTicks() / 1000;
    val_time = 300 + time_over - time_val;
    if(val_time <= 0) {
        isQuit = true;
        fatal = false;
    }
    else {
        string str_time = "Time: " + to_string(val_time);
        time_game.SetText(str_time);
        time_game.LoadFromRenderText(font_time, g_screen);
        time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
    }

    string str_mark = "Mark: " + to_string(mark_value);
    mark_game.SetText(str_mark);
    mark_game.LoadFromRenderText(font_time, g_screen);
    mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5 + 150, 15);

    string money_str =  to_string(p_player.get_money_count()) + " X ";
    moneyCount.SetText(money_str);
    moneyCount.LoadFromRenderText(font_time, g_screen);
    player_money.SetPos(moneyCount.GetWidth() + 390, 8);
    moneyCount.RenderText(g_screen, 400, 15);

}

void HandleMouse(SDL_Event e, SDL_Rect rect, TextObject &text, SDL_Color default_color, SDL_Color change_color) {
    if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEMOTION) {
        int x_pos, y_pos;
        SDL_GetMouseState(&x_pos, &y_pos);
        bool inside = true;

        //Mouse is left of the button
        if( x_pos < rect.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x_pos > rect.x + rect.w )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y_pos< rect.y)
        {
            inside = false;
        }
        //Mouse below the button
        else if( y_pos > rect.y + rect.h )
        {
            inside = false;
        }
        if(!inside) {
            text.setType(BUTTON_SPRITE_MOUSE_OUT);
            text.SetColor(default_color);
        }
        else {
            text.setType(BUTTON_SPRITE_MOUSE_OVER_MOTION);
            text.SetColor(change_color);
            if(e.button.button == SDL_BUTTON_LEFT) {
                if(e.type == SDL_MOUSEBUTTONDOWN)text.setType(BUTTON_SPRITE_MOUSE_DOWN);
                else text.setType(BUTTON_SPRITE_MOUSE_OVER_MOTION);
            }
        }
    }
}



void showBox() {
    SDL_Delay(200);
    SDL_Event e;
    TextObject text[4];
    text[0].SetText("Congratulation!You Win");
    text[1].SetText("You Lost!");
    text[2].SetText("Press to SPACE to replay ");
    text[3].SetText("or ESC to exit game!");
    for(int i = 0; i < 4; i++) {
        int leng = text[i].GetText().length() * 30;
        text[i].SetRect((SCREEN_WIDTH - leng) / 2, 150 + 60*i);
        text[i].SetColor(BLACKCOLOR);
    }
    bool is_quit = false;
    while(!is_quit) {

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
            case SDL_QUIT:
                close();
                exit(1);
                break;

            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_SPACE) {
                    isQuit = false;
                    is_quit = true;
                }
                else if(e.key.keysym.sym == SDLK_ESCAPE) {
                    close();
                    exit(1);
                }
                break;
            }
        }
        //background_menu.Render(g_screen, NULL);
        for(int i = 0; i < 4; i++) {
            if((fatal && i == 0) || (!fatal && i == 1) || i >= 2) {
                text[i].LoadFromRenderText(font_menu, g_screen);
                text[i].RenderText(g_screen, text[i].getPosX(), text[i].getPosY());
            }
        }
        SDL_RenderPresent(g_screen);
        SDL_Delay(100);
    }
    time_over = SDL_GetTicks()/ 1000;
}

void showOptions() {
    Mix_PauseMusic();
    background_menu.LoadImg(".//img//menu.png", g_screen);
    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 0);
    SDL_RenderClear(g_screen);
    SDL_Event e;
    bool is_quit = false;
    TextObject text[4];
    text[0].SetText("Remuse");
    text[1].SetText("Replay");
    text[2].SetText("Tutorial");
    text[3].SetText("Exit");
    for(int i = 0; i < 4; i++) {
        int leng = text[i].GetText().length() * 30;
        text[i].SetRect((SCREEN_WIDTH - leng) / 2, 200 + 80*i);
        text[i].SetColor(BLACKCOLOR);
    }
    while(!is_quit) {

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
            case SDL_QUIT:
                close();
                exit(1);
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE) is_quit = true;
            }
            for(int i = 0; i < 4; i++) {
                HandleMouse(e, text[i].getRect(), text[i]);
            }

        }
        background_menu.Render(g_screen, NULL);
        for(int i = 0; i < 4; i++) {
            text[i].LoadFromRenderText(font_menu, g_screen);
            text[i].RenderText(g_screen, text[i].getPosX(), text[i].getPosY());
        }
        for(int i = 0; i < 4; i++) {
            if(text[i].getType() == BUTTON_SPRITE_MOUSE_DOWN) {
                loadChunk(".\\img\\game resource\\beep_.wav", g_chunk);
                Mix_PlayChannel(-1, g_chunk, 0);
                //SDL_Delay(200);
                if(text[i].GetText() == "Exit") {
                    SDL_Delay(200);
                    close();
                    exit(1);
                }
                else if(text[i].GetText() == "Remuse")is_quit = true;
                else if(text[i].GetText() == "Tutorial") {
                    showTutorial();
                }
                else if(text[i].GetText() == "Replay") {
                    isQuit = true;
                    is_quit = true;
                    replay = true;
                }
            }
        }

        SDL_RenderPresent(g_screen);
        SDL_Delay(100);
    }
    time_over = SDL_GetTicks()/ 1000 - 300 + val_time;

    Mix_ResumeMusic();
}

void playGame() {
    ImpTimer fps_timer;
    time_over = SDL_GetTicks()/1000;
    if(firstStep) {

        if(!InitData()) {
            logSDLError(cout, "init", true);
            close();
        }
        if(loadBackground() == false) {
            logSDLError(cout, "load background", true);
        }
        firstStep = false;
        showMenu();
    }
    mark_value = 0;
    GameMap game_map;
    game_map.loadMap(".\\map2\\map01.dat");
    game_map.loadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg(".\\img\\player_right.png", g_screen);
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);
    vector<ThreatsObject*> threats_list = MakeThreadList();

    vector<Plane*> Planes_list = MakePlaneList();
    p_boss = makeBoss();

    ExplosionObject exp_threat;
    ExplosionObject exp_main;
    bool tRet = exp_threat.LoadImg(".\\img\\game resource\\exp3.png", g_screen);
    bool mRet = exp_main.LoadImg(".\\img\\game resource\\exp3.png", g_screen);
    if(!tRet || !mRet) {
        logSDLError(cout, "Explosion image", true);
    }
    exp_threat.set_clip();
    exp_main.set_clip();

    Mix_PlayMusic(g_music, -1);
    Mix_VolumeMusic(20);
    Mix_VolumeChunk(g_chunk, 10);
    while(!isQuit) {
        fps_timer.start();
        while(SDL_PollEvent(&g_event) != 0) {
            switch(g_event.type) {
            case SDL_QUIT:
                close();
                exit(1);
                break;
            case SDL_KEYDOWN:
                if(g_event.key.keysym.sym == SDLK_ESCAPE)
                    showOptions();
                break;
            }

            p_player.HandelInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen, NULL);


        Map map_data =  game_map.getMap();

        HandlePlayer(p_player, map_data, player_power);
        game_map.setMap(map_data);
        game_map.DrawMap(g_screen);



        HandleBulletThreats(p_player, threats_list, exp_main, map_data, player_power);
        HandleBulletThreats(p_player, Planes_list, exp_main, map_data, player_power);
        HandleConflict(p_player, threats_list, exp_threat);
        HandleConflict(p_player, Planes_list, exp_threat);
        if(p_boss->get_alive()) {
            HandleBoss(p_boss, p_player, map_data, exp_main, player_power);
            HandleConflictWithBoss(p_player, p_boss, exp_threat);
        }


        drawTextShow(p_player, player_money);
        player_power.Show(g_screen);
        player_money.Show(g_screen);


        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND;
        if(real_imp_time < time_one_frame) {
            int delay_time = time_one_frame - real_imp_time;
            SDL_Delay(delay_time);
        }
        //this_thread::sleep_for(15ms);
    }
    for(int i = 0; i < threats_list.size(); i++) {
        ThreatsObject* p_threat = threats_list[i];
        if(p_threat != NULL) {
            p_threat->Free();
        }
    }

    for(int i = 0; i < Planes_list.size(); i++) {
        Plane* p_plane = Planes_list[i];
        Planes_list.erase(Planes_list.begin() + i);
        if(p_plane != NULL) {
            p_plane->Free();
        }
    }
    p_player.reset();
    if(!replay)showBox();
    else {
        isQuit = false;
        replay = false;
    }
}
