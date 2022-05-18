#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define NUM_FRAME_MAIN 8

class MainObject : public BaseObject {

public:
    MainObject();
    ~MainObject();

    enum WalkType {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
        WALK_RIGHT_UP = 3,
        WALK_LEFT_UP = 4,
        WALK_RIGHT_DOWN = 5,
        WALK_LEFT_DOWN = 6,
        WALK_UP = 7,
        WALK_DOWN = 8
    };
    virtual bool LoadImg(string path, SDL_Renderer* screen);
    void show(SDL_Renderer* des);
    void HandelInputAction(SDL_Event event, SDL_Renderer* screen);
    void set_clips();

    SDL_Rect GetRectFrame();

    void DoPlayer(Map& map_data);
    void checkToMap(Map& map_data);
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap(Map& map_data);
    void UpdateImagePlayer(SDL_Renderer* des);

    void set_bullet_list(vector<BulletObject*> bullet_list) {
        p_bullet_list_ = bullet_list;
    }
    vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des, Map& map_data);
    void RemoveBullet(const int& idx);
    void IncreaseMoney();
    int get_frame_width() const {return width_frame_;}
    int get_frame_height() const {return height_frame_;}
    int set_come_back_time(int time_) {come_back_time_ = time_;}
    int get_money_count() const {return money_count;}
    bool get_decrease() const {return decrease;}
    void set_decrease(const bool& fall) {decrease = fall;}
    Input get_input_type() const {return input_type_;}
    float get_space_x() const {return space_x;}
    float get_space_y() const {return space_y;}
    int get_inf() const {return inf;}
    void set_inf(int inf_) {inf = inf_;}
    void set_immortal(bool immortal_) {immortal = immortal_;}
    bool get_immortal() const {return immortal;}
    float set_space_x(float spaceX) {space_x = spaceX;}
    float set_space_y(float spaceY) {space_y = spaceY;}
    int get_map_x() const {return map_x_;}
    int get_map_y() const {return map_y_;}
    bool get_win() const {return win;}
    void reset();
private:

    int money_count;
    vector<BulletObject*> p_bullet_list_;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[15];
    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;

    int map_x_;
    int map_y_;

    int come_back_time_;
    bool decrease = false;
    float space_x, space_y;
    int inf;
    bool immortal;
    bool win;
    int old_status;
    int bullet_delay;
};

#endif // MAIN_OBJECT_H_
