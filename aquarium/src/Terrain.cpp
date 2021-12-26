
// #include "terrain.hpp"
// // vcl
// #include "vcl/gtx/string_cast.hpp"
// #include <vcl/ext.hpp>
// #include <vcl/vcl.hpp>
// #include <vcl/gtc/noise.hpp>
// #include <vcl/gtx/matrix_operation.hpp>
// #include <vcl/gtx/rotate_vector.hpp>
// using namespace vcl;
// const int SIZE_ARRAY=4;
// float h[SIZE_ARRAY]={.6,0.2,0.2,0.1};
// float sigma[SIZE_ARRAY]={0.1,0.15,0.1,0.1};
// vec2 p[SIZE_ARRAY]={vec2(0.2,0.1),vec2(0.5,0.5),vec2(0.2,0.7),vec2(0.8,0.7)};
// struct perlin_noise_parameters parameters;

#include "Terrain.hpp"
#include "vcl/vcl.hpp"

Terrain *Terrain::singleton = nullptr;




Terrain::Terrain(){
    first_line=0;
    first_col=0;
    center=2*NUM_OF_LINE+1;
    for(int i=0;i<16;i++){
        grid[i]=new Chunk(mirror_grid[i]);
        vcl::vec2 pos = translation_grid[i];
        grid[i]->set_position(vcl::vec3(pos.x,pos.y,0.));
        //std::cout << i<<  ": " << pos.x << " " << pos.y << "\n";
       // grid->mirror(mirror_grid[i]);
    }
}


Chunk Terrain::get_instance(int i){
    return *grid[i];
}

int Terrain::get_center(){
    return ((first_line+2)%NUM_OF_LINE)*NUM_OF_LINE+(first_col+1)%NUM_OF_LINE;
}

float Terrain::compute_z(float x, float y){
    int posx_chunk=floor((x+(SIZE_OF_CHUNK/2))/SIZE_OF_CHUNK);
    int posy_chunk=floor((y+(SIZE_OF_CHUNK/2))/SIZE_OF_CHUNK);
    x-=SIZE_OF_CHUNK*posx_chunk;
    y-=SIZE_OF_CHUNK*posy_chunk;

    int pos_in_grid=(((posy_chunk)%2+2+NUM_OF_LINE)%NUM_OF_LINE)*NUM_OF_LINE+((posx_chunk)%2+1+NUM_OF_LINE)%NUM_OF_LINE;
    return grid[pos_in_grid]->compute_z(x,y);
}

vcl::vec3 Terrain::compute_normal(float x, float y){

    int posx_chunk=floor((x+(SIZE_OF_CHUNK/2))/SIZE_OF_CHUNK);
    int posy_chunk=floor((y+(SIZE_OF_CHUNK/2))/SIZE_OF_CHUNK);
    x-=SIZE_OF_CHUNK*posx_chunk;
    y-=SIZE_OF_CHUNK*posy_chunk;

    int pos_in_grid=(((posy_chunk%3+3)%3+2)%NUM_OF_LINE)*NUM_OF_LINE+((posx_chunk%3+3)%3+1)%NUM_OF_LINE;
    return grid[pos_in_grid]->compute_normal(x,y);
}

void Terrain::user_move_up(){
    #ifdef DEBUG
    std::cout << "up" << "\n";
    #endif
    
    for(int i=0;i<NUM_OF_LINE;i++){
        grid[first_line*NUM_OF_LINE+i]->translate(vcl::vec3(0.,-(NUM_OF_LINE)*SIZE_OF_CHUNK,0.));
    }

    first_line++;
    if (first_line>=NUM_OF_LINE)
        first_line-=NUM_OF_LINE;

}
void Terrain::user_move_down(){
    #ifdef DEBUG
    std::cout << "down" << "\n";
    #endif
     
    first_line--;
    if (first_line<0)
        first_line+=NUM_OF_LINE;


    for(int i=0;i<NUM_OF_LINE;i++){
        grid[first_line*NUM_OF_LINE+i]->translate(vcl::vec3(0.,(NUM_OF_LINE)*SIZE_OF_CHUNK,0.));
    }



}
void Terrain::user_move_left(){
    

    first_col--;
    if (first_col<0)
        first_col+=NUM_OF_LINE;

    for(int i=0;i<NUM_OF_LINE;i++){
        grid[i*NUM_OF_LINE+first_col]->translate(vcl::vec3(-(NUM_OF_LINE)*SIZE_OF_CHUNK,0.,0.));
    }

    #ifdef DEBUG
    std::cout << "left" << "\n";
    #endif
}
void Terrain::user_move_right(){
    #ifdef DEBUG
    std::cout << "right" << "\n";
    #endif
    
    for(int i=0;i<NUM_OF_LINE;i++){
     
        grid[i*NUM_OF_LINE+first_col]->translate(vcl::vec3((NUM_OF_LINE)*SIZE_OF_CHUNK,0.,0.));
    }
    first_col++;
    if (first_col>=NUM_OF_LINE)
        first_col-=NUM_OF_LINE;
}

void Terrain::update(vcl::vec3 position_of_user){
    if (position_of_user.y>grid[get_center()]->get_position().y+SIZE_OF_CHUNK/2.)
        user_move_down();
    if (position_of_user.y<grid[get_center()]->get_position().y-SIZE_OF_CHUNK/2.)
        user_move_up();
    if (position_of_user.x<grid[get_center()]->get_position().x-SIZE_OF_CHUNK/2.)
        user_move_left();
    if (position_of_user.x>grid[get_center()]->get_position().x+SIZE_OF_CHUNK/2.)
        user_move_right();

}



