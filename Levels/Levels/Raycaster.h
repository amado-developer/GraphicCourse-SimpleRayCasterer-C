//
//  Raycaster.h
//  Levels
//
//  Created by Amado Garcia on 10/26/20.
//

#ifndef Raycaster_h
#define Raycaster_h
using namespace::std;
#include <stdio.h>
#include <vector>

class Raycaster
{
    private:
        int width;
        int height;
        int block_size;
        double ray_distance;
        char wall_position;
        vector<double> player;
        vector<tuple<double, double, vector<double>>> points{};
        vector<vector<char>> map{};
        SDL_Window *window;
        SDL_Renderer *s;
        vector<vector<double>> colors{};
    
  
    public:
        Raycaster(int width, int height, char* window_name);
        void point(int x, int y, vector<double> color);
        void draw_rectangle(int x, int y, int width, int height, vector<double> color);
        void load_map(char* filename);
        void render();
        void clear();
        void cast_ray(double angle);
        void draw_strake(double x, double h, vector<double> color);
};

#endif /* Raycaster_h */
