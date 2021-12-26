#include "Fish.hpp"
#include "Terrain.hpp"
#include "main.hpp"
#include <math.h>  

using namespace vcl;


Fish::Fish(){
    old_orientation=vec3(1.,0.,0.);
    orientation=vec3(1.,0.,0.);
    position=vec3(0.,0.,0.);
    speed=max_speed;

    fish_rotation = rotation();
    fish_mesh=mesh_primitive_ellipsoid(vec3(0.1,0.03,0.1), vec3(0.,0.,0.), 30, 30);
    eyes_mesh = mesh_primitive_ellipsoid(vec3(0.035, 0.02, 0.035), vec3(0., 0., 0.), 30, 30);
    //sph.position += vec3(0.05,0.,0.);
    // fish_mesh.push_back(sph);
    mesh tail = mesh_primitive_triangle({ 0,0,0 }, { -0.1,0,0.1}, { -0.1,0,-0.1});
    tail.position += vec3(-0.065, 0., 0.);
    fish_mesh.push_back(tail);
   // mesh eye1 = mesh_primitive_disc(0.01f, { 0.08,0.1,0.05 }, { 0,1,0 });
}

vec3 Fish::get_position(){
    return position;
}

vec3 Fish::get_orientation(){
    return orientation;
}

float Fish::get_speed(){
    return speed;
}
mesh Fish::get_mesh(){
    return fish_mesh;
}
mesh Fish::get_eye(){
    return  eyes_mesh;
}


rotation Fish::get_rotation(){
    return fish_rotation;
}



void Fish::set_position(vec3 _position){
    position=_position;
}

void Fish::set_orientation(vec3 _orientation){
    orientation=_orientation;
}

void Fish::set_speed(float _speed){
    speed=_speed;
}



void Fish::translate(vec3 translation){
    position+=translation;
}


void Fish::rotate(vec3 axis, float angle){
    vec3 axis_normalized=normalize(axis);
    mat3 r =rotation::axis_angle_to_matrix(axis_normalized,angle);
    orientation=r*orientation;
    fish_rotation=rotation(r)*fish_rotation;
}


void Fish::rotate_towards_direction(vec3 direction, float coef){
    float scal = dot(orientation,normalize(direction));
    scal=(scal>0)?scal:-scal;
    if (scal<0.001){
        return;
    }
    vec3 axis_of_rotation = cross(orientation,direction);
    if (norm(axis_of_rotation)<0.005)
        return;
        //assuming orientation's norm is one
    float angle = coef*std::acos(dot(normalize(axis_of_rotation),normalize(orientation)));
    if (angle>0.03){
        rotate(axis_of_rotation,angle);
    }
}

void Fish::update(float dt){
        //update orientation to make it avoid ground if close and go toward ground if far
    {
        vec3 normal = Terrain::get_terrain()->compute_normal(position.x,position.y);

        float z_terrain = Terrain::get_terrain()->compute_z(position.x,position.y);
        float diff = 10*(position.z - z_terrain);    
        if (diff>0.1){
                //avoid
            float coef = 0.5*(1-std::exp(-1./(diff*diff)));
            rotate_towards_direction(normal,coef);
                //go back
            coef = 0.1*std::exp(-100./(diff*diff));
            rotate_towards_direction(vec3(orientation.x,orientation.y,-0.1),coef);
        }else  {
            position.z=Terrain::get_terrain()->compute_z(position.x,position.y)+0.01;
        }
    }

        //update orientation to make them go toward the player if they are far away or avoid them if they are close

    {
        vec3 diff= position-scene.camera.position_camera;
        float dist = norm(diff);
        if (dist>0.01){
            diff=normalize(diff);
                //avoid the player
            float coef = (1-std::exp(-.5/(dist*dist)));
            rotate_towards_direction(diff,coef);
                //go toward him
            coef = 0.1*(1-std::exp(-dist*dist/500.));
            rotate_towards_direction(-diff,coef);
        }
    }

    position+=dt*speed*orientation;
}

void Fish::update_movement(){
    orientation=normalize(orientation);

    float angle = dot(orientation,old_orientation);
    angle=sqrt(1-angle*angle);

    if (angle >0)
        speed=max_speed*(4-2*angle);

    old_orientation=normalize(orientation);

}


bool Fish::collision_with_fish(Fish *fish2){
     return (norm(fish2->get_position()-position)<RADIUS_FISH);
}
bool Fish::collision_with_pos(vec3 *position2){
     return (norm(*position2-position)<RADIUS_FISH);
}