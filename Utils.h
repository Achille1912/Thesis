#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "GameObject.h"

struct Utils
{

    static void Print(std::string str)
    {
        std::cout << str << std::endl;
    }

    static float map_range(float value, float from_min, float from_max, float to_min, float to_max)
    {
        return (value - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
    }

    static bool isInstanceOf(GameObject *obj, const std::type_info &type)
    {
        return typeid(*obj) == type;
    }

    static void SetRectSizeAndPosition(SDL_FRect &t_dstRect, float t_x, float t_y, float t_w, float t_h)
    {
        t_dstRect.h = t_h;
        t_dstRect.w = t_w;
        t_dstRect.x = t_x;
        t_dstRect.y = t_y;
    }
    static SDL_Color RandomColor() {
        return { (unsigned char)(0 + (rand() % 254)),(unsigned char)(0 + (rand() % 254)),(unsigned char)(0 + (rand() % 254)),(unsigned char)(0 + (rand() % 254)) };
    }

};

struct Color
{
    Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    Color(int t_r, int t_g, int t_b, int t_a)
    {
        r = t_r;
        g = t_g;
        b = t_b;
        a = t_a;
    }
    int r;
    int g;
    int b;
    int a;
};




#endif
