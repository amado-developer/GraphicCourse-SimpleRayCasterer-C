//
//  Raycaster.cpp
//  Levels
//
//  Created by Amado Garcia on 10/26/20.
//
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Raycaster.h"
#include "LTimer.h"
#include <iomanip>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <tuple>
#include <iostream>
#include <sstream>
using namespace::std;
Raycaster::Raycaster(int width, int height, char* window_name)
{
    this->width = width;
    this->height = height;
    this->window = SDL_CreateWindow(window_name,
                SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width,
    height, SDL_WINDOW_SHOWN);
    this->s = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    this->block_size = 50;
    vector<double>player{70.0, 70.0, 0, 0.785398};
    this->player = player;
    this->step_size = 5;
    vector<vector<double>> colors{{146, 60, 63}, {233, 70, 63}, {0, 130, 168}};
    SDL_RenderClear(s);
    this -> colors = colors;
    SDL_Texture* texture1 = IMG_LoadTexture(s, "wall1.png");
    SDL_Texture* texture2 = IMG_LoadTexture(s, "wall2.png");
    SDL_Texture* texture3 = IMG_LoadTexture(s, "wall3.png");
    SDL_Texture* texture4 = IMG_LoadTexture(s, "wall4.png");
    SDL_Texture* texture5 = IMG_LoadTexture(s, "wall5.png");
    SDL_Texture* helText  = IMG_LoadTexture(s, "helicopter.png");
    
    SDL_Surface* txt1 = IMG_Load("wall1.png");
    SDL_Surface* txt2 = IMG_Load("wall2.png");
    SDL_Surface* txt3 = IMG_Load("wall3.png");
    SDL_Surface* txt4 = IMG_Load("wall4.png");
    SDL_Surface* txt5 = IMG_Load("wall5.png");
    

    SDL_Surface* sprite1 = IMG_Load("sprite1.png");
    SDL_Surface* sprite2 = IMG_Load("sprite2.png");
    SDL_Surface* sprite3 = IMG_Load("sprite3.png");
    SDL_Surface* sprite4 = IMG_Load("sprite4.png");
    SDL_Surface* helicopter = IMG_Load("helicopter.png");

    textures.push_back(texture1);
    textures.push_back(texture2);
    textures.push_back(texture3);
    textures.push_back(texture4);
    textures.push_back(texture5);
    textures.push_back(helText);
    
    surfaces.push_back(txt1);
    surfaces.push_back(txt2);
    surfaces.push_back(txt3);
    surfaces.push_back(txt4);
    surfaces.push_back(txt5);
    
   
    this->enemies.push_back(make_tuple(100.0, 200.0, sprite2));
    this->enemies.push_back(make_tuple(280.0, 190.0, sprite3));
    this->enemies.push_back(make_tuple(225.0, 340.0, sprite4));
    this->enemies.push_back(make_tuple(220.0, 425.0, sprite1));
    // this->enemies.push_back(make_tuple(320.0, 420.0, sprite2));
    this->other_objects.push_back(make_tuple(400.0, 425.0,helicopter));

    vector<double> zBuffer(500, -INFINITY);
    this -> zBuffer = zBuffer;


    memset(frametimes, 0, sizeof(frametimes));
    framecount = 0;
    framespersecond = 0;
    frametimelast = SDL_GetTicks();
}

/*
 * Prints out the whole screen with a predefined color
 */
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

/*
 * Actually draws something in the screen using raycasting
 */
double Raycaster::cast_ray(double angle)
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
            auto hitx = x - (double) i*50.0;
            auto hity = y - (double) j*50.0;
            
            double maxhit{0};
            
            if(hitx > 1.0 && hitx < 49.0)
                maxhit = hitx;
            else
                maxhit = hity;
            
            tx = int(maxhit * 128.0 / 50.0);
            return distance;
        }
        else
        {
           
            point(x, y, {255, 255, 255});
            distance+=1.0;
            
        }
    }
    return -INFINITY;
}

/*
 * Draws the ray corresponding to player vision
 */
void Raycaster::draw_strake(double x, double h, vector<double> color)
{
    auto start{int(250.0 - h / 2.0)};
    auto end{int(250.0 + h / 2.0)};
    auto txt = this -> current_surface;
    
    for(int y{start}; y < end; y++)
    {
        int ty = ((y - start)*128.0)/(end - start);
        uint32_t pixel = *( ( uint32_t * )txt->pixels + (int)ty * txt->w + int(tx));
        uint8_t r;
        uint8_t g;
        uint8_t b;

        SDL_GetRGB( pixel, txt->format , &r, &g, &b );
        point(x, y, {static_cast<double>(r), static_cast<double>(g), static_cast<double>(b)});
    }
}

/*
 * Draws sprites objects (enemies, players, etc)
 */
void Raycaster::draw_sprite(tuple<double, double, SDL_Surface*> sprite)
{
    double sprite_a{atan2(get<1>(sprite) - player[1], get<0>(sprite) - player[0])};
    double sprite_d{pow(pow(player[0] - get<0>(sprite), 2) + pow(player[1] - get<1>(sprite),2), 0.5)};
    double sprite_size = (500.0/sprite_d) * 70.0;
    double sprite_x = 500.0 + (sprite_a - player[2])*500.0/player[3] + 250.0 - sprite_size/2.0;
    double sprite_y = 250.0 - sprite_size/2.0;
    sprite_x = int(sprite_x);
    sprite_y = int(sprite_y);
    sprite_size = int(sprite_size);
    
    for(auto x{sprite_x}; x < sprite_x + sprite_size; x++)
    {
        for(auto y{sprite_y}; y < sprite_y + sprite_size; y++)
        {
            if(x > 500 && x < 1000 && zBuffer[x - 500] >= sprite_d)
            {
                auto tx{int((x - sprite_x) * 128.0/sprite_size)};
                auto ty{int((y - sprite_y) * 128.0/sprite_size)};
                uint32_t pixel = *( ( uint32_t * ) current_sprite->pixels + ty * current_sprite->w + tx);
                uint8_t r;
                uint8_t g;
                uint8_t b;
                SDL_GetRGB( pixel, current_sprite->format , &r, &g, &b);
                if(r != 152 && g != 0 && b != 136)
                {
                    point(x, y, {static_cast<double>(r), static_cast<double>(g), static_cast<double>(b)});
                    zBuffer[x - 500] = sprite_d;
                }
            }
        }
    }
    
}

/*
 * Actually shows on screen all that was drawn in the buffer
 */
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
            {
                current_texture = textures[int(map[j][i] - 49)];
                draw_rectangle(x, y, block_size, block_size);
            }
        }
    }
    point(player[0], player[1], {255, 255, 255});
    this->current_texture = textures[5];
    point(400, 400, {0,0,0});
    current_sprite = surfaces[5];
    draw_rectangle(400, 400, block_size, block_size);
    
    for(auto i{0}; i < 500; i++)
    {
        point(500, i, {0, 0, 0});
        point(501, i, {0, 0, 0});
        point(499, i, {0, 0, 0});
    }


    
    for(auto i{0}; i < 500; i++)
    {
        auto angle{player[2] - player[3]/2.0 + (player[3] * (double) i) / 500.0};
        double distance{cast_ray(angle)};
        auto x{500.0 + (double) i};
        auto h{500.0/(ray_distance * cos(angle - player[2])) * 100};
        int wall_position = this -> wall_position - 48;
        current_texture = textures[wall_position - 1];
        current_surface = surfaces[wall_position];
        zBuffer[i] = distance;
        draw_strake(x, h, colors[wall_position]);
    }
    
    for(auto object: other_objects)
    {
        point(get<0>(object), get<1>(object), {0,0,0});
        current_sprite = get<2>(object);
        draw_sprite(object);
    }
    for(auto enemy : enemies)
    {
        point(get<0>(enemy), get<1>(enemy), {0,0,0});
        current_sprite = get<2>(enemy);
        draw_sprite(enemy);
    }
        bool quit = false;
        SDL_Event event;

        //Frames
    

        SDL_Rect text_rect;
        string x = "X: " + to_string((int) player[0]);
        set_text(x, {255, 0, 0, 255});
        SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
        text_rect.x = 500;
        text_rect.y = 450;
        SDL_RenderCopy(s, text_texture, nullptr, &text_rect);

        string y = "Y: " + to_string((int) player[1]);
        set_text(y, {255, 255, 50, 255});
        SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
        text_rect.x = 625;
        text_rect.y = 450;
        SDL_RenderCopy(s, text_texture, nullptr, &text_rect);
   
        fps_frames += 6;
        if (fps_lasttime < SDL_GetTicks() - 1000)
        {
            fps_lasttime = SDL_GetTicks();
            fps_current = fps_frames;
            fps_frames = 0;
        }

        string frames_per_second = "FPS: " + to_string((int) fps_current);
        set_text(frames_per_second, {50, 255, 50, 255});
        SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
        text_rect.x = 850;
        text_rect.y = 450;
        SDL_RenderCopy(s, text_texture, nullptr, &text_rect);

        SDL_RenderPresent(s);
   

        while (!quit) {
            while (SDL_PollEvent(&event)) 
            {
    
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                    break;
                }

                if(player[0] > 400.0 && player[0] < 450 && player[1] > 400 && player[1] < 450)
                {
                    win("./font.ttf", 40, "You have won! Press any key to start again!", {255, 255, 255, 255}, 120, 200);
                }
                
                if(event.type == SDL_KEYDOWN)
                {
                    if(event.key.keysym.sym == SDLK_a)
                    {
                        player[2] -= 0.5;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_d)
                    {
                        player[2] += 0.5;
        
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_RIGHT)
                    {
                        player[0] -= sin(player[2]) * 15;
                        player[1] += cos(player[2]) * 15;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_LEFT)
                    {
                        player[0] += sin(player[2]) * 15;
                        player[1] -= cos(player[2]) * 15;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_UP)
                    {
                        player[0] += cos(player[2]) * 15;
                        player[1] += sin(player[2]) * 15;
                        render();
                    }
                    
                    else if(event.key.keysym.sym == SDLK_DOWN)
                    {
                        player[0] -= cos(player[2]) * 15;
                        player[1] -= sin(player[2]) * 15;
                        render();
                    }
                }
            }
 
        }
    
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(s);
        SDL_Quit();
}

void Raycaster::set_text(string path, int font_size, string message, SDL_Color color)
{
    TTF_Init();
    this->font = TTF_OpenFont( path.c_str(), font_size);
    if(!font)
        cout<< "Failed to load font!\n";
    
    auto text_surface{TTF_RenderText_Solid(font, message.c_str(), color)};
    if(!text_surface)
        cout<< "Failed to create surface!\n";
    
    this->text_texture =  SDL_CreateTextureFromSurface(s, text_surface);
    if(!text_texture)
        cout<< "Failed to create text texture!\n";
    
    SDL_FreeSurface(text_surface);
}

void Raycaster::set_text(string message, SDL_Color color)
{ 
    TTF_Init();
    auto text_surface{TTF_RenderText_Solid(font, message.c_str(), color)};
    if(!text_surface)
        cout<< "Failed to create surface!\n";
    this->text_texture =  SDL_CreateTextureFromSurface(s, text_surface);
    if(!text_texture)
        cout<< "Failed to create text texture!\n";
    SDL_FreeSurface(text_surface);
}

void Raycaster::render_main_menu(string path, int font_size, string message, SDL_Color color, 
int x, int y)
{
    bool show{true};
    bool click{false};
    int current_button{0};
    SDL_Texture* background = IMG_LoadTexture(s, "background.png");
    
    SDL_SetRenderDrawColor(s, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(s);
    SDL_RenderCopy(s, background, NULL, NULL);
    set_text(path, font_size, message, color);
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
    this -> text_rect.x = x;
    this -> text_rect.y = y;
    SDL_RenderCopy(s, text_texture, nullptr, &text_rect);
    SDL_RenderPresent(s);
    SDL_Event event;
    while (show)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                show = false;
            }
            
            if(event.type == SDL_KEYDOWN)
            {
                show = false;
                render();
            }
        }
    }
}

void Raycaster::win(string path, int font_size, string message, SDL_Color color, int x, int y)
{
    bool show{true};
    bool click{false};
    int current_button{0};
    SDL_Texture* background = IMG_LoadTexture(s, "win_background.jpg");
    
    SDL_SetRenderDrawColor(s, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(s);
    SDL_RenderCopy(s, background, NULL, NULL);
    set_text(path, font_size, message, color);
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
    this -> text_rect.x = x;
    this -> text_rect.y = y;
    SDL_RenderCopy(s, text_texture, nullptr, &text_rect);
    SDL_RenderPresent(s);
    SDL_Event event;
    while (show)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                show = false;
            }
            
            if(event.type == SDL_KEYDOWN)
            {
                show = false;
                player[0] = 70;
                player[1] = 70;
                player[2] = 0;
                render_main_menu("./font.ttf", 40, "Press any key to enter!", {255, 255, 255, 255}, 320, 200);
            }
        }
    }
}

/*
 * Draws a point (kind of obvious though)
 */
void Raycaster::point(int x, int y, vector<double> color)
{
    SDL_SetRenderDrawColor(s, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(s, x, y);
}

/*
 * Draws a rectangle with a specific color
 */
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

/*
 * Draws a rectangle with a texture
 */
void Raycaster::draw_rectangle(int x, int y, int width, int height)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    
    SDL_RenderCopy(s, current_texture, &screen, &rect);
}

/*
 * Reads a txt with the map to use
 */
void Raycaster::load_map(char *filename)
{
    string text;
    ifstream MyReadFile("map.txt");
    SDL_SetRenderDrawColor(s, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(s);
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