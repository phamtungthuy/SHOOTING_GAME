#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunc.h"

class TextObject {
public:
    TextObject();
    ~TextObject();

    enum TextColor{
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };

    bool LoadFromFile(string path);
    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);
    void SetColor(SDL_Color color) {text_color_ = color;}

    void RenderText(SDL_Renderer* screen, int xp , int yp, SDL_Rect* clip = NULL, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int GetWidth() const {return rect.w;}
    int GetHeight() const {return rect.h;}

    void SetText(const string& text) {str_val_ = text;}
    string GetText() const {return str_val_;}
    void SetRect(int xpos, int ypos) { rect.x = xpos; rect.y = ypos;}
    SDL_Rect getRect() const {return rect;}
    int getPosX() const {return rect.x;}
    int getPosY() const {return rect.y;}
    void setType(int type_) {type = type_;}
    int getType() const {return type;}
private:
    string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    SDL_Rect rect;
    int type;
//    int width_;
//    int height_;
//    int xp, yp;

};

#endif // TEXT_OBJECT_H_
