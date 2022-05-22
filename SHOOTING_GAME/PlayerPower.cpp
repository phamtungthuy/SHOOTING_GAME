#include "PlayerPower.h"

PlayerPower::PlayerPower() {
    number_ = 0;
}

PlayerPower::~PlayerPower() {

}

void PlayerPower::AddPos(const int& xPos) {
    pos_list.push_back(xPos);
}

void PlayerPower::Init(SDL_Renderer* screen) {
    LoadImg(".\\img\\game resource\\contra_pw.png", screen);
    number_ = 3;
    if(pos_list.size() > 0 ) {
        pos_list.clear();
    }
    for(int i = 0; i < 3; i++) {
        AddPos(15 + i*(rect_.w + 20));
    }
}

void PlayerPower::Show(SDL_Renderer* screen) {
    for(int i = 0; i < pos_list.size(); i++) {
        rect_.x = pos_list[i];
        rect_.y = 0;
        Render(screen);
    }
}

void PlayerPower::Decrease() {
    number_--;
    pos_list.pop_back();
}

void PlayerPower::Increase() {
    number_--;
    int last_pos = pos_list.back();
    last_pos += rect_.w + 20;
    pos_list.push_back(last_pos);
}

PlayerMoney::PlayerMoney() {
    x_pos = 0;
    y_pos = 0;
}

PlayerMoney::~PlayerMoney() {

}

void PlayerMoney::Init(SDL_Renderer* screen) {
    LoadImg(".\\img\\game resource\\money_img.png", screen);
}

void PlayerMoney::Show(SDL_Renderer* screen) {
    rect_.x = x_pos;
    rect_.y = y_pos;
    Render(screen);
}
