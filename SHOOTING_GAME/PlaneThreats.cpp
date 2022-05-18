#include "PlaneThreats.h"

Plane::Plane() {
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
    input_type_.left_ = 0;
}

Plane::~Plane() {
}

bool Plane::LoadImg(string path, SDL_Renderer* screen) {
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret) {
        width_frame_ = rect_.w/PLANE_FRAME_NUM;
        height_frame_ = rect_.h;

    }
    return ret;
}

void Plane::set_clips() {
    if(width_frame_ > 0 && height_frame_ > 0) {
        for(int i = 0; i < PLANE_FRAME_NUM; i++) {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void Plane::Show(SDL_Renderer* des) {
    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;
    if(rect_.x > 0 && rect_.x <= SCREEN_WIDTH && rect_.y >= 0 && rect_.y <= SCREEN_HEIGHT) {
        frame_++;
        frame_ %= PLANE_FRAME_NUM;
        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect rendQuad = {rect_.x , rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
    }
}

void Plane::DoPlayer(Map& gMap, SDL_Renderer* screen) {
    x_val_ = 0;
    y_val_ = 0;
    x_val_ += -PLANE_SPEED;
    if(rect_.x <= SCREEN_WIDTH) x_pos_ += x_val_;

}

void Plane::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen) {
    if(p_bullet != NULL) {
        p_bullet->set_bullet_type(BulletObject::BOMB);
        bool ret = p_bullet->loadImgBullet(screen);
        if(!ret) return;
        p_bullet->set_is_move(true);
        p_bullet->set_bullet_dir(BulletObject::DIR_DOWN);
        p_bullet->SetRect(rect_.x+ 10, rect_.y+ 30);
        p_bullet->set_x_val(PLANE_SPEED);
        p_bullet->set_y_val(10);
        bullet_list_.push_back(p_bullet);
    }
}

void Plane::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& map_data, float space_x, float space_y) {
    for(int i = 0; i < bullet_list_.size(); i++) {
        BulletObject* p_bullet = bullet_list_[i];
        if(p_bullet != NULL) {
            p_bullet->SetRect(p_bullet->GetRect().x -  space_x, p_bullet->GetRect().y - space_y);
            int pos_x = p_bullet->GetRect().x + map_data.start_x_;
            int pos_y = p_bullet->GetRect().y + map_data.start_y_;
            if(pos_y < 0 || pos_y > SCREEN_HEIGHT || (map_data.tile[pos_y/64][pos_x/64] != BLANK_TILE && map_data.tile[pos_y/64][pos_x/64] != STATE_MONEY) ) p_bullet->set_is_move(false);
            if(p_bullet->get_is_move()) {
                int bullet_distance = rect_.y + height_frame_ - p_bullet->GetRect().y;
                p_bullet->HandleMove(x_limit, y_limit);
                p_bullet->Render(screen);
            }
            else {

                p_bullet->SetRect(rect_.x + 10, rect_.y + 30);
                p_bullet->set_is_move(true);
                if(rect_.x >= 0 && rect_.x <= SCREEN_WIDTH) {
                    loadChunk(".\\img\\game resource\\Laser.wav", g_chunk);
                    Mix_PlayChannel(-1, g_chunk, 0);
                }
            }
        }
    }

}

void Plane::RemoveBullet(const int& idx) {
    int Size = bullet_list_.size();
    if(Size > 0 && idx < Size) {
        BulletObject* &p_bullet = bullet_list_[idx];
        p_bullet->set_is_move(false);
//        bullet_list_.erase(bullet_list_.begin() + idx);
//        if(p_bullet != NULL) {
//            delete p_bullet;
//            p_bullet = NULL;
//        }
    }
}

SDL_Rect Plane::GetRectFrame() {
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

void Plane::ImpMoveType(SDL_Renderer* screen) {
}
