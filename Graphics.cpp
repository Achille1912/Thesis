#include "Graphics.h"
#include "Game.h"
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>


SDL_Window* Graphics::window = NULL;
SDL_Renderer* Graphics::renderer = NULL;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;
bool Graphics::debug = false;
float Graphics::scale = 1.0;
SDL_Texture* Graphics::sfondo_texture;

int Graphics::Width() {
    return windowWidth;
}

int Graphics::Height() {
    return windowHeight;
}

bool Graphics::OpenWindow(const char* t_title, int t_xpos, int t_ypos, int t_width, int t_height, bool t_fullscreen) {
    int flags = 0;
    windowWidth = t_width;
    windowHeight = t_height;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Initialized." << std::endl;

        window = SDL_CreateWindow(t_title, t_xpos, t_ypos, t_width, t_height, flags);
        if (window) {
            std::cout << "Window created." << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created." << std::endl;
        }
        sfondo_texture = IMG_LoadTexture(Graphics::renderer, "./gfx/background.jpg");

        return true;
    }
    else {
        return false;
    }
}

void Graphics::ClearScreen() {
    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);
    SDL_RenderClear(renderer);
}

void Graphics::RenderFrame() {
    SDL_RenderPresent(renderer);
}

void Graphics::RenderScale() {
    SDL_RenderSetScale(renderer, scale, scale);
}


void Graphics::CloseWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game Closed." << std::endl;
}

void Graphics::SDL_RenderDrawRect(SDL_Renderer* t_renderer, float t_xpos, float t_ypos, float t_width, float t_height, Color t_color) {
    SDL_SetRenderDrawColor(Graphics::renderer, t_color.r, t_color.g, t_color.b, t_color.a);
    SDL_FRect rect = { 100, 100, 50, 50 };
    SDL_RenderFillRectF(t_renderer, &rect);
}



void Graphics::SDL_RenderDrawCircle(SDL_Renderer* t_renderer, int t_x, int t_y, int t_radius) {
    for (int w = 0; w <= t_radius * 2; w++) {
        for (int h = 0; h <= t_radius * 2; h++) {
            int dx = w - t_radius;
            int dy = h - t_radius;
            if ((dx * dx + dy * dy) <= (t_radius * t_radius)) {
                SDL_RenderDrawPoint(t_renderer, t_x + dx, t_y + dy);
            }
        }
    }
}

void Graphics::DrawCenterPoint(GameObject* t_obj, int t_width, int t_height, Color t_color) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect center;

    center.w = t_width;
    center.h = t_height;
    
    center.x = (t_obj->getX() + t_obj->getW() / 2 - center.w / 2);
    center.y = (t_obj->getY() + t_obj->getH() / 2 - center.h / 2);

    SDL_SetRenderDrawColor(t_obj->getRenderer(), t_color.r, t_color.g, t_color.b, t_color.a);
    SDL_RenderFillRectF(t_obj->getRenderer(), &center);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Graphics::DrawVertices(SDL_Renderer* t_renderer, std::vector <Vector2f> t_vertices, Color t_color) {
    SDL_SetRenderDrawColor(Graphics::renderer, t_color.r, t_color.g, t_color.b, t_color.a);
    for (int i = 0; i < t_vertices.size(); i++) {
        Graphics::SDL_RenderDrawCircle(Graphics::renderer, t_vertices[i].x, t_vertices[i].y, 5);
    }
    SDL_SetRenderDrawColor(t_renderer, 255, 255, 255, 255);
}