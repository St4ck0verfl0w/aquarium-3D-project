#include "School.hpp"
#include "Terrain.hpp"
#include "cmath"

using namespace vcl;

School::School(int _n_fish){
    n_fish=_n_fish;

    for(int i=0;i<n_fish;i++){
        fishes[i]= new Fish();
        float x=XRANGE*((float)( (rand()%n_fish)/(float) n_fish));
        float y=YRANGE*((float)( (rand()%n_fish)/(float) n_fish));
		fishes[i]->set_position(vec3(x, y, Terrain::get_terrain()->compute_z(x,y)+1) );
    }

}
Fish *School::get_instance(int i){
    return fishes[i];
}
int School::get_n_fish(){
    return n_fish;
}

void School::compute_boid(){
        //make them go toward the center
    vec3 new_center=vec3(0.,0.,0.);
    for(int i=0;i<n_fish;i++){
        new_center+=fishes[i]->get_position();
    }
    new_center = new_center/n_fish;

    vec3 global_mov = new_center-center;
    if(norm(global_mov)>0.01){
        center=new_center;
    }
    for(int i = 0; i<n_fish;i++){
        vec3 diff = fishes[i]->get_position()-center;
        float dist = norm(diff);
        if (dist<0.01)
            continue;

        float coef = 0.1*(1-std::exp(-dist*dist/10.));
        if (dist>SCHOOL_RANGE){
            fishes[i]->rotate_towards_direction(-diff,coef);
        }else{
            if(norm(global_mov)>0.01){
                coef = 0.2*(1-std::exp(-1./dist*dist));
                fishes[i]->rotate_towards_direction(global_mov,coef);
            }
        }


    }

}



void School::update_all(float dt){
    compute_boid();
    for(int i=0;i<n_fish;i++){
        fishes[i]->update(dt);
        fishes[i]->update_movement();
    }
}