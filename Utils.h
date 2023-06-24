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
};

struct Color
{
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
