#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>
#include <array>

#include "Vector2.h"
#include "GameObject.h"
#include "Utils.h"


struct Graphics {
    static int windowWidth;
    static int windowHeight;
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static float scale;
    static SDL_Texture* background_texture;

    static int Width();
    static int Height();
    static bool OpenWindow(const char* t_title, int t_xpos, int t_ypos, int t_width, int t_height, bool t_fullscreen);
    static void CloseWindow();
    static void ClearScreen();
    static void RenderFrame();
    static void RenderScale();
    static void SDL_RenderDrawRect(SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width, float t_height, Color t_color);
    static void SDL_RenderDrawCircle(SDL_Renderer* t_renderer, int t_xpos, int t_ypos, int t_radius);
    static void DrawCenterPoint( GameObject* t_obj, int t_width, int t_height, Color t_color);
    static void DrawVertices(SDL_Renderer* t_renderer, std::array <Vector2f, 4> t_vertices, Color t_color);
};

#endif
