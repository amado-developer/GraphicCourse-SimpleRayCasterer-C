//
//  Raycaster.h
//  Levels
//
//  Created by Amado Garcia on 10/26/20.
//

#ifndef Raycaster_h
#define Raycaster_h
#include <stdio.h>
#include <vector>
#include <tuple>
using namespace::std;
class Raycaster
{
    private:
        int step_size;
        int width;
        int height;
        int block_size;
        double ray_distance;
        char wall_position;
        vector<double> player;
        vector<tuple<double, double, vector<double> > > points{};
        vector<vector<char>> map{};
        SDL_Window *window;
        SDL_Renderer *s;
        vector<vector<double>> colors{};
        SDL_Texture* current_texture = NULL;
        SDL_Surface* current_surface = NULL;
        SDL_Surface* current_sprite = NULL;
        vector<SDL_Texture*> textures{};
        vector<SDL_Surface*> surfaces{};
        vector<tuple<double, double, SDL_Surface*>> enemies{};
        vector<Uint8*> pixels{};
        double tx;
        vector<double> current_color{};
        SDL_Rect screen = {0, 0, 1000, 500};
        vector<double> zBuffer{};
        SDL_Texture* text_texture = nullptr;
        SDL_Rect text_rect;
        
    public:
        Raycaster(int width, int height, char* window_name);
        void point(int x, int y, vector<double> color);
        void draw_rectangle(int x, int y, int width, int height, vector<double> color);
        void draw_rectangle(int x, int y, int width, int height);
        void load_map(char* filename);
        void render();
        void render_main_menu(string path, int font_size, string message, SDL_Color color, int x, int y);
        void clear();
        double cast_ray(double angle);
        void draw_strake(double x, double h, vector<double> color);
        void draw_sprite(tuple<double, double, SDL_Surface*> sprite);
        SDL_Texture* set_text(string path, int font_size, string message, SDL_Color color);
        
};

#endif /* Raycaster_h */
