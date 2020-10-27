//
//  Raycaster.cpp
//  Levels
//
//  Created by Amado Garcia on 10/26/20.
//

#include <stdio.h>
#include <SDL.h>
#include "Raycaster.h"
#include <iomanip>
#include <fstream>

using namespace::std;
Raycaster::Raycaster(int width, int height, char* window_name)
{
    this->width = width;
    this->height = height;
    this->window = SDL_CreateWindow(window_name,
                SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width,
    height, SDL_WINDOW_SHOWN);
    this->s = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

//    clear();
    this->block_size = 50;
    vector<double>player{(static_cast<double>(block_size + 20)), (static_cast<double>(block_size + 20)), M_PI / 3.0, M_PI/3.0};
    this->player = player;
    vector<vector<double>> colors{{146, 60, 63}, {233, 70, 63}, {0, 130, 168}};

    SDL_RenderClear(s);
    this -> colors = colors;
}

void Raycaster::clear()
{
    double r{0.0};
    double g{0.0};
    double b{0.0};
    for(auto x{0}; x < width; x++)
    {
        for(auto y{0}; y < height; y++)
        {
            if(x/width < 1)
                r = int(((double)x/width)*255.0);
            else
                r = 1.0;
            if(y/height <1)
                g = int(((double)y/height) * 255.0);
            else
                g = 1.0;
            point(x, y, {static_cast<double>(r), static_cast<double>(g), static_cast<double>(b)});
        }
    }
}

void Raycaster::cast_ray(double angle)
{
    double distance{0.0};
    bool is_not_Wall{true};
    
    while(is_not_Wall)
    {
        double x{player[0] + distance * cos(angle)};
        double y{player[1] + distance * sin(angle)};
        auto i{int(x/50.0)};
        auto j{int(y/50.0)};
        if(map[j][i] != ' ')
        {
            is_not_Wall = false;
            wall_position = map[j][i] - 1;
            ray_distance = distance;
            
        }
        else
        {
            point(x, y, {255, 255, 255});
            distance+=1.0;
        }
    }
}

void Raycaster::draw_strake(double x, double h, vector<double> color)
{
    auto start{int(250.0 - h / 2.0)};
    auto end{int(250.0 + h / 2.0)};
    
    for(auto y{start}; y < end; y++)
    {
        point(x, y, color);
    }
    
}
void Raycaster::render()
{
    SDL_SetRenderDrawColor(s, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(s);
    for(auto x{0}; x < 500; x+=block_size)
    {
        for(auto y{0}; y < 500; y+=block_size)
        {
            int i{x/block_size};
            int j{y/block_size};

            if(this->map[j][i] != ' ')
                draw_rectangle(x, y, block_size, block_size, colors[(int) map[j][i] - 49]);
        }
    }
    point(player[0], player[1], {255, 255, 255});
    
    for(auto i{0}; i < 500; i++)
    {
        point(500, i, {0, 0, 0});
        point(501, i, {0, 0, 0});
        point(499, i, {0, 0, 0});
    }
    for(auto i{1}; i < 500; i++)
    {
        auto angle{player[2] - player[3]/2.0 + (player[3] * (double) i) / 500.0};
        cast_ray(angle);
        auto x{500.0 + (double) i};
        auto h{500.0/(ray_distance * cos(angle - player[2])) * 100};
        int wall_position = this -> wall_position - 48;
        draw_strake(x, h, colors[wall_position]);
    }
    
        bool quit = false;
        SDL_Event event;
    
        while (!quit) {
            //drawing particles
            //setting up objects
            //repeated over and over again
    
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                
                if(event.type == SDL_KEYDOWN)
                {
                    if(event.key.keysym.sym == SDLK_a)
                    {
                        player[2] -= M_PI/10.0;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_d)
                    {
                        player[2] += M_PI/10.0;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_RIGHT)
                    {
                        player[0] += 10.0;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_LEFT)
                    {
                        player[0] -= 10.0;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_UP)
                    {
                        player[1] += 10.0;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_DOWN)
                    {
                        player[1] -= 10.0;
                        render();
                    }
                }
            }
            SDL_RenderPresent(s);
        }
    
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(s);
        SDL_Quit();
}

void Raycaster::point(int x, int y, vector<double> color)
{
    SDL_SetRenderDrawColor(s, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(s, x, y);
}

void Raycaster::draw_rectangle(int x, int y, int width, int height, vector<double> color)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    
    SDL_SetRenderDrawColor(s, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(s, &rect);
    SDL_SetRenderDrawColor(s, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(s, &rect);
}

void Raycaster::load_map(char *filename)
{
    string text;
    ifstream MyReadFile("map.txt");
    while (getline (MyReadFile, text)) {
        vector<char> line{};
        for(auto character : text)
        {
            line.push_back(character);
        }
        map.push_back(line);
    }
    MyReadFile.close();
}
