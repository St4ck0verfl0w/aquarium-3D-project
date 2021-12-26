#pragma once

#include "Chunk.hpp"
#include "vcl/vcl.hpp"

//#define DEBUG 


static float SoC= SIZE_OF_CHUNK;
static vcl::vec2 translation_grid[16] = {
    vcl::vec2(-SoC, 2*SoC),  vcl::vec2(0, 2*SoC),  vcl::vec2(SoC, 2*SoC),  vcl::vec2(2*SoC, 2*SoC),  //
        vcl::vec2(-SoC, SoC),  vcl::vec2(0, SoC),  vcl::vec2(SoC, SoC),  vcl::vec2(2*SoC, SoC),  //
        vcl::vec2(-SoC, 0),  vcl::vec2(0, 0),  vcl::vec2(SoC, 0),  vcl::vec2(2*SoC, 0),  //
        vcl::vec2(-SoC, -SoC), vcl::vec2(0, -SoC), vcl::vec2(SoC, -SoC) ,  vcl::vec2(2*SoC, -SoC) //
    };;

#define NUM_OF_LINE 4

static vcl::vec3 mirror_grid[NUM_OF_LINE*NUM_OF_LINE] = {
        X_ROTATION, NO_ROTATION, X_ROTATION, NO_ROTATION,
        XY_ROTATION, Y_ROTATION, XY_ROTATION,Y_ROTATION,
        X_ROTATION, NO_ROTATION, X_ROTATION, NO_ROTATION,
        XY_ROTATION, Y_ROTATION, XY_ROTATION, Y_ROTATION
    };


class Terrain{

    static Terrain *singleton;

    Terrain();

    private:
        Chunk *grid[16];
        int center;
        int first_col; // used to move from North to South, this will give us the shift. Works like modulo in fact
        int first_line; // ssame for East-West direction
    public:
        Chunk get_instance(int i);

        int get_center();


        float compute_z(float x, float y);
        vcl::vec3 compute_normal(float x, float y);

        void user_move_up();
        void user_move_down();
        void user_move_left();
        void user_move_right();
        
        void update(vcl::vec3 position_of_user);


        static Terrain * get_terrain(){
            if (!singleton){
                singleton = new Terrain;
            }
            return singleton;
        }
};
