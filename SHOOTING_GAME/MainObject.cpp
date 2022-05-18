#include "MainObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10

#define PLAYER_JUMP_VAL 20

MainObject::MainObject() {
    reset();
}
void MainObject::reset() {
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_RIGHT;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
    money_count = 0;
    space_x = 0;
    space_y = 0;
    inf = -10;
    immortal = false;
    win = false;
    bullet_delay = 0;
    input_type_.up_ = 0;
    input_type_.down_ = 0;
    for(int i = 0; i < p_bullet_list_.size(); i++) {
        RemoveBullet(i);
    }
}

MainObject::~MainObject() {
    for(int i = 0; i < p_bullet_list_.size(); i++) {
        BulletObject* &p_bullet = p_bullet_list_.at(i);
        p_bullet_list_.erase(p_bullet_list_.begin() + i);
        if(p_bullet != NULL) {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

bool MainObject::LoadImg(string path, SDL_Renderer* screen) {
    bool ret = BaseObject::LoadImg(path, screen);

    if(ret == true) {
        width_frame_ = rect_.w/NUM_FRAME_MAIN;
        height_frame_ = rect_.h;
    }

    return ret;
}

SDL_Rect MainObject::GetRectFrame() {
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

void MainObject::set_clips() {
    if(width_frame_ > 0 && height_frame_ > 0) {
        for(int i = 0; i < NUM_FRAME_MAIN; i++) {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void MainObject::show(SDL_Renderer* des) {
    if(come_back_time_ > 0) return;
    UpdateImagePlayer(des);

    if(input_type_.left_ == 1 || input_type_.right_ == 1) {
        frame_++;
        frame_ %= NUM_FRAME_MAIN;
    }
    else {
        frame_ = 0;
    }

    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

    //SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen) {
    if(come_back_time_ > 0) return;
    if(events.type == SDL_KEYDOWN) {

        switch(events.key.keysym.sym) {
        case SDLK_RIGHT:
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            input_type_.left_ = 0;

            break;
        case SDLK_LEFT:
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0;
            break;
        default:
            break;
        }
        switch(events.key.keysym.sym) {
        case SDLK_UP:
            input_type_.up_ = 1;
            input_type_.down_ = 0;
            break;
        case SDLK_DOWN:
            input_type_.up_ = 0;
            input_type_.down_ = 1;
            break;
        default:
            break;
        }

        if(events.key.keysym.sym == SDLK_SPACE) {
            input_type_.jump_ = 1;
        }
        if(events.key.keysym.sym == SDLK_a) {
            input_type_.add_bullet_ = 1;
        }
    }
    else if(events.type == SDL_KEYUP) {
        switch(events.key.keysym.sym) {
        case SDLK_RIGHT:
            input_type_.right_ = 0;
            break;
        case SDLK_LEFT:
            input_type_.left_ = 0;
            break;
        default:
            break;
        }
        switch(events.key.keysym.sym) {
        case SDLK_UP:
            input_type_.up_ = 0;
            break;
        case SDLK_DOWN:
            input_type_.down_ = 0;
            break;
        default:
            break;
        }
        if(events.key.keysym.sym == SDLK_SPACE) {
            input_type_.jump_ = 0;
        }
        if(events.key.keysym.sym == SDLK_a) {
            input_type_.add_bullet_ = 0;
        }
    }

    if(input_type_.add_bullet_ == 1 && bullet_delay == 0) {
        bullet_delay = 2;
        loadChunk(".\\img\\game resource\\Fire.wav", g_chunk);
        Mix_PlayChannel(-1, g_chunk, 0);
        BulletObject* p_bullet = new BulletObject();
        p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
        p_bullet->loadImgBullet(screen);
        p_bullet->setBulletPlayer(true);
//        if(input_type_.right_ == 0 && input_type_.left_ == 0 && (input_type_.up_ == 1 || input_type_.down_ == 1) && status_ != WALK_NONE) {
//            old_status = status_;
//            status_ = WALK_NONE;
//        }
//        if(status_ == WALK_NONE && input_type_.up_ == 0 && input_type_.down_ == 0) status_ = old_status;
        if(status_ == WALK_LEFT) {
            p_bullet->SetRect(this->rect_.x, this->rect_.y + height_frame_*0.25);
            if(input_type_.down_ == 1) {
                p_bullet->set_bullet_dir(BulletObject::DIR_DOWN_LEFT);
            }
            else if(input_type_.up_ == 1){
                p_bullet->SetRect(this->rect_.x + 10, this->rect_.y + height_frame_*0.25);
                p_bullet->set_bullet_dir(BulletObject::DIR_UP_LEFT);
            }
            else {
                p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            }
        }
        else if(status_ == WALK_RIGHT){
            p_bullet->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_*0.25);
//            if(input_type_.up_ == 1 && input_type_.right_ == 1) {
//                p_bullet->set_bullet_dir(BulletObject::DIR_UP_RIGHT);
//            }
            if(input_type_.down_ == 1) {
                p_bullet->set_bullet_dir(BulletObject::DIR_DOWN_RIGHT);
            }
            else if(input_type_.up_ == 1) {
                p_bullet->set_bullet_dir(BulletObject::DIR_UP_RIGHT);
            }
            else {
                p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
            }
        }
        p_bullet->set_x_val(20);
        p_bullet->set_y_val(20);
        p_bullet->set_is_move(true);
        p_bullet_list_.push_back(p_bullet);
    }
    else if(bullet_delay > 0) --bullet_delay;
}

void MainObject::HandleBullet(SDL_Renderer* des, Map& map_data) {
    for(int i = 0; i < p_bullet_list_.size(); i++) {
        BulletObject* p_bullet = p_bullet_list_[i];
        if(p_bullet != NULL) {
            int pos_x = p_bullet->GetRect().x + map_data.start_x_;
            int pos_y = p_bullet->GetRect().y + map_data.start_y_;
            if(map_data.tile[pos_y/64][pos_x/64] != BLANK_TILE && map_data.tile[pos_y/64][pos_x/64] != STATE_MONEY) p_bullet->set_is_move(false);
            if(p_bullet->get_is_move()) {
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);
            }
            else {
                 p_bullet_list_.erase(p_bullet_list_.begin() + i);
                if(p_bullet != NULL) {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }

}

void MainObject::RemoveBullet(const int& idx) {
    int Size = p_bullet_list_.size();
    if(Size > 0 && idx < Size) {
        BulletObject* &p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);
        if(p_bullet != NULL) {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

void MainObject::DoPlayer(Map& map_data) {
    if(come_back_time_ == 0) {
        x_val_ = 0;
        y_val_ += 0.8;
        if(y_val_ >= MAX_FALL_SPEED) {
            y_val_ = MAX_FALL_SPEED;
        }

        if(input_type_.left_ == 1) {
            x_val_ -= PLAYER_SPEED;
        }
        else if(input_type_.right_ == 1) x_val_ += PLAYER_SPEED;
        if(input_type_.jump_ == 1) {
            if(on_ground_ == true) {
                y_val_ = - PLAYER_JUMP_VAL;
                on_ground_ = false;
                input_type_.jump_ = 0;
            }
        }
        checkToMap(map_data);
        CenterEntityOnMap(map_data);
    }
    else {
        come_back_time_--;
        if(come_back_time_ == 0) {
            loadChunk(".\\img\\game resource\\two_beep_.wav", g_chunk);
            Mix_PlayChannel(-1, g_chunk, 0);
            if(x_pos_ > 256) {
                x_pos_ -= 256;
            }
            else x_pos_ = 0;
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
            on_ground_ = false;
            inf = SDL_GetTicks() / 1000;
            immortal = false;
        }
    }
}

void MainObject::CenterEntityOnMap(Map& map_data) {
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/2);
    if(map_data.start_x_ < 0) map_data.start_x_ = 0;
    else if(map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT*0.75);
    if(map_data.start_y_ < 0) map_data.start_y_ = 0;
    else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_) {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

void MainObject::checkToMap(Map& map_data) {
    float old_pos_x = x_pos_;
    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;

    //check horizontal
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if(map_data.tile[y1][x1] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y1][x1] = 0;
        }
        if(map_data.tile[y1][x2] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y1][x2] = 0;
        }
        if(map_data.tile[y2][x1] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y2][x1] = 0;
        }
        if(map_data.tile[y2][x2] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y2][x2] = 0;
        }
        if(map_data.tile[y1][x2] == DESTINATION || map_data.tile[y2][x2] == DESTINATION || map_data.tile[y1][x1] == DESTINATION || map_data.tile[y2][x1] == DESTINATION) win = true;
        if(x_val_ > 0) {
            if((map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != DESTINATION) || (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != DESTINATION)) {
                x_pos_ = x2*TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            }
        }
        else if(x_val_ < 0) {
            if((map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != DESTINATION) || (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != DESTINATION)) {
                x_pos_ = (x1 + 1)*TILE_SIZE;
                x_val_ = 0;
             }
        }
    }
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min - 1)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;
    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if(map_data.tile[y1][x1] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y1][x1] = 0;
        }
        if(map_data.tile[y1][x2] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y1][x2] = 0;
        }
        if(map_data.tile[y2][x1] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y2][x1] = 0;
        }
        if(map_data.tile[y2][x2] == STATE_MONEY) {
            IncreaseMoney();
            map_data.tile[y2][x2] = 0;
        }
        if(map_data.tile[y1][x2] == DESTINATION || map_data.tile[y2][x2] == DESTINATION || map_data.tile[y1][x1] == DESTINATION || map_data.tile[y2][x1] == DESTINATION) win = true;
        if(y_val_ > 0) {
            if((map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != DESTINATION) || (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != DESTINATION)) {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                //if(status_ == WALK_NONE) status_ = WALK_RIGHT;
                on_ground_ = true;
            }

        }
        else if(y_val_ < 0) {
            if((map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != DESTINATION) || (map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != DESTINATION)) {
                y_pos_ = (y1 + 1) * TILE_SIZE ;
                y_val_ = 0;
            }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;
    if(x_pos_ < 0) {
        x_pos_ = 0;
    }
    else if(x_pos_ + width_frame_ > map_data.max_x_) {
        x_pos_ =  map_data.max_x_ - width_frame_ -1;
    }

    if(y_pos_ > map_data.max_y_) {
        come_back_time_ = 60;
        decrease = true;
    }
}

void MainObject::IncreaseMoney() {
    loadChunk(".\\img\\game resource\\beep_.wav", g_chunk);
    Mix_PlayChannel(-1, g_chunk, 0);
    money_count++;
}

void MainObject::UpdateImagePlayer(SDL_Renderer* des) {
    if(on_ground_) {
        if(status_ == WALK_LEFT) {
            if(input_type_.up_ == 1) {
                LoadImg(".\\img\\contra_up_left.png", des);
            }
            else if(input_type_.down_ == 1) {
                LoadImg(".\\img\\contra_down_left.png", des);
            }
            else LoadImg(".\\img\\contra_left.png", des);
        }
        else{
            if(input_type_.up_ == 1) {
                LoadImg(".\\img\\contra_up_right.png", des);
            }
            else if(input_type_.down_ == 1) {
                LoadImg(".\\img\\contra_down_right.png", des);
            }
            else LoadImg(".\\img\\contra_right.png", des);
        }
    }
    else {
        if(status_ == WALK_LEFT) {
            if(input_type_.up_ == 1) {
                LoadImg(".\\img\\jum_up_left.png", des);
            }
            else if(input_type_.down_ == 1) {
                LoadImg(".\\img\\jum_down_left.png", des);
            }
            else LoadImg(".\\img\\contra_jum_left.png", des);
        }
        else {
            if(input_type_.up_ == 1) LoadImg(".\\img\\jum_up_right.png", des);
            else if(input_type_.down_ == 1) LoadImg(".\\img\\jum_down_right.png", des);
            else LoadImg(".\\img\\contra_jum_right.png", des);
        }
    }
}
