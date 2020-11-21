#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Raycaster.h"
#include <iomanip>
#include <fstream>

using namespace std;
int main()
{
    Raycaster r(1000, 500, (char*)"Game");
    r.load_map((char *) "../maps/map.txt");
    r.render_main_menu("../fonts/font.ttf", 40, "Press any key to enter!", {255, 255, 255, 255}, 320, 350);
    cout<<"Hasta la proxima"<<endl;
    return 0;
}
