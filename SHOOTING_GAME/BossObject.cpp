#include "BossObject.h"
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3
BossObject::BossObject() {
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    on_ground_ = false;
    come_back_time_ = 0;
    frame_ = 0;
    animation_a_ = 0;
    animation_b_ = 0;
    input_type_.left_ = 0;
    type_move_ = STATIC_THREAT;
    delay = 0;
    alive = true;
    health = 50;
}

BossObject::~BossObject() {
    for(int i = 0; i < bullet_list_.size(); i++) {
        BulletObject* &p_bullet = bullet_list_.at(i);
        bullet_list_.erase(bullet_list_.begin() + i);
        if(p_bullet != NULL) {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

bool BossObject::LoadImg(string path, SDL_Renderer* screen) {
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret) {
        width_frame_ = rect_.w/BOSS_FRAME_NUM;
        height_frame_ = rect_.h;

    }
    return ret;
}

void BossObject::set_clips() {
    if(width_frame_ > 0 && height_frame_ > 0) {
        for(int i = 0; i < BOSS_FRAME_NUM; i++) {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }

}

void BossObject::Show(SDL_Renderer* des) {
    if(come_back_time_ == 0) {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        if(delay == 0) {
            frame_++;
            if(health >= BOSS_HEALTH / 3)delay = 3;
            else delay = 1;
        }
        else if(delay > 0) delay--;
        frame_ %= BOSS_FRAME_NUM;
        SDL_Rect* currentClip = &frame_clip_[frame_];
        int pos_x = rect_.x + (width_frame_ - BOSS_HEALTH*3)/2;
        int pos_y = rect_.y + 50;
        SDL_Rect outline = {pos_x, pos_y, BOSS_HEALTH*3 + 2, 12};
        SDL_Rect boss_health = {pos_x + 1, pos_y + 1, health*3, 10};
        SDL_SetRenderDrawColor(des, 255, 255, 255, 0);
        SDL_RenderDrawRect(des, &outline);
        SDL_SetRenderDrawColor(des, 255, 0, 0, 0);
        SDL_RenderFillRect(des, &boss_health);
        SDL_Rect rendQuad = {rect_.x , rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
    }
}

void BossObject::DoPlayer(Map& gMap, SDL_Renderer* screen) {
    if(come_back_time_ == 0) {
        x_val_ = 0;
        y_val_ += THREAT_GRAVITY_SPEED;

        if(y_val_ >= THREAT_MAX_FALL_SPEED) {
            y_val_ = THREAT_MAX_FALL_SPEED;
        }

        if(input_type_.left_ == 1) {
            x_val_ -= THREAT_SPEED;
        }
        else if(input_type_.right_ == 1)x_val_ += THREAT_SPEED;
        CheckToMap(gMap, screen);

    }
    else if(come_back_time_ > 0) {
        --come_back_time_;
        if(come_back_time_ == 0) {
            InitThreats();
        }
    }
}

void BossObject::InitThreats() {
    if(x_pos_ > 256) {
        x_pos_ -= 256;
        animation_a_ -= 256;
        animation_b_ -= 256;
    }
    else x_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    y_pos_ = 0;
    on_ground_ = false;
    input_type_.left_ = 1;

}

void BossObject::CheckToMap(Map& map_data, SDL_Renderer* screen) {
        int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;

    //check horizontal
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if(x_val_ > 0) {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];
            //check if the boss has collided any blocks
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY)) {
                x_pos_ = x2*TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            }
        }
        else if(x_val_ < 0) {
            int val1 = map_data.tile[y1][x1], val2 = map_data.tile[y2][x1];
            //check if the boss has collided any blocks
            if( (val1!= BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY)) {
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
        if(y_val_ > 0) {
            int val1 = map_data.tile[y2][x1], val2 = map_data.tile[y2][x2];
            //check if the boss has collided any blocks
            if( (val1!= BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY) ) {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                //if(status_ == WALK_NONE) status_ = WALK_RIGHT;
                on_ground_ = true;
            }
        }
        else if(y_val_ < 0) {
            int val1 = map_data.tile[y1][x1], val2 = map_data.tile[y1][x2];
            //check if the boss has collided any blocks
            if( (val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY)) {
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
    }
}

void BossObject::ImpMoveType(SDL_Renderer* screen) {
}

void BossObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen) {
    if(p_bullet != NULL) {
        p_bullet->set_bullet_type(BulletObject::FIRER);
        bool ret = p_bullet->loadImgBullet(screen);
        if(!ret) return;
        p_bullet->set_is_move(true);
        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        p_bullet->SetRect(rect_.x  - 50, rect_.y + height_frame_ - 30);
        p_bullet->set_x_val(15);
        bullet_list_.push_back(p_bullet);
    }
}

void BossObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& map_data, float space_x, float space_y) {
    for(int i = 0; i < bullet_list_.size(); i++) {
        BulletObject* p_bullet = bullet_list_[i];
        if(p_bullet != NULL) {

            p_bullet->SetRect(p_bullet->GetRect().x - space_x, p_bullet->GetRect().y - space_y);
            int pos_x = p_bullet->GetRect().x + map_data.start_x_;
            int pos_y = p_bullet->GetRect().y + map_data.start_y_;
            if(map_data.tile[pos_y/64][pos_x/64] != BLANK_TILE && map_data.tile[pos_y/64][pos_x/64] != STATE_MONEY && abs(pos_x - x_pos_) >= 300) p_bullet->set_is_move(false);
            if(p_bullet->get_is_move()) {
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;

                if(bullet_distance < 1400 && bullet_distance > 0) {
                    p_bullet->HandleMove(x_limit, y_limit);
                    p_bullet->Render(screen);
                }
                else{
                    bullet_list_.erase(bullet_list_.begin() + i);
                    if(p_bullet != NULL) {
                        delete p_bullet;
                        p_bullet = NULL;
                    }
                }
            }
            else {
                bullet_list_.erase(bullet_list_.begin() + i);
                if(p_bullet != NULL) {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
    if(frame_ == 22 && delay == 0) InitBullet(new BulletObject(), screen);
}

void BossObject::RemoveBullet(const int& idx) {
    int Size = bullet_list_.size();
    if(Size > 0 && idx < Size) {
        BulletObject* &p_bullet = bullet_list_[idx];
        p_bullet->set_is_move(false);
    }
}

SDL_Rect BossObject::GetRectFrame() {
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

