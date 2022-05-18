#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class PlayerPower : public BaseObject {
public:
    PlayerPower();
    ~PlayerPower();

    void SetNum(const int& num) {number_ = num;}
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);

    int getPower() const {return pos_list.size();}
    void Increase();
    void Decrease();

private:
    int number_;
    vector<int> pos_list;

};

class PlayerMoney : public BaseObject {
public:
    PlayerMoney();
    ~PlayerMoney();

    void Init(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void SetPos(const int& x, const int& y) {x_pos = x, y_pos = y;}
private:
    int x_pos, y_pos;

};

#endif // PLAYER_POWER_H_
