#ifndef PLANE_THREATS_H_
#define PLANE_THREATS_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#define PLANE_FRAME_NUM 1
#define PLANE_SPEED 10

class Plane : public BaseObject{
public:
    Plane();
    ~Plane();
    void set_x_val(const float& xVal) { x_val_ = xVal;}
    void set_y_val(const float& yVal) { y_val_ = yVal;}
    void set_x_pos(const float& xp) { x_pos_ = xp;}
    void set_y_pos(const float& yp) {y_pos_ = yp;}

    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}
    void SetMapXY(const int& mp_x, const int& mp_y) {map_x_ = mp_x; map_y_ = mp_y;}

    void set_clips();
    virtual bool LoadImg(string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    void DoPlayer(Map& gMap, SDL_Renderer* screen);
    void CheckToMap(Map& gMap, SDL_Renderer* screen);
    void InitThreats();
    void set_type_move(const int& type_move) {type_move_ = type_move;}
    int get_type_move() const {return type_move_;}
    void set_input_left(const int& ipLeft) {input_type_.left_ = ipLeft;}
    void ImpMoveType(SDL_Renderer* screen);

    vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
    void set_bullet_list(const vector<BulletObject*>& bl_list) {bullet_list_ = bl_list;}

    void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& map_data, float space_x, float space_y);
    void RemoveBullet(const int& idx);
    SDL_Rect GetRectFrame();
private:
    float y_val_;
    float x_val_;
    float x_pos_;
    float y_pos_;
    bool on_ground_;
    int come_back_time_;

    SDL_Rect frame_clip_[20];
    int width_frame_;
    int height_frame_;
    int frame_;

    int type_move_;
    Input input_type_;

    vector<BulletObject*> bullet_list_;
    int map_x_;
    int map_y_;
};

#endif // PLANE_THREATS_H_
