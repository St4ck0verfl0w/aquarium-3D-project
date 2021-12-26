#pragma once

#include "vcl/vcl.hpp"

#define RADIUS_FISH 3.

class Fish{
    private:
        vcl::rotation fish_rotation;
        vcl::vec3 orientation;
        vcl::vec3 old_orientation;
        vcl::vec3 position;
        float speed;
        vcl::mesh fish_mesh;
        vcl::mesh eyes_mesh;

    public:


        static bool collision(vcl::vec3 *position1, vcl::vec3 *position2);

        Fish();
        vcl::vec3 get_position();
        vcl::vec3 get_orientation();
        float get_speed();
        vcl::mesh get_mesh();
        vcl::mesh get_eye();
        vcl::rotation get_rotation();

        
        void set_position(vcl::vec3 _position);
        void set_orientation(vcl::vec3 _orientation);
        void set_speed(float _speed);

        void translate(vcl::vec3 translation);
        void rotate_towards_direction(vcl::vec3 direction, float coef);
        void rotate(vcl::vec3 axis, float angle);
        void update(float dt);
        
        //orientation + speed
        void update_movement();

        bool collision_with_fish(Fish *fish2);
        bool collision_with_pos(vcl::vec3 *position2);


};