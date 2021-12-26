#pragma once

#include "vcl/vcl.hpp"
#include "Fish.hpp"

#define NMAX 300
#define XRANGE 5.
#define YRANGE 5.
#define SCHOOL_RANGE 2.
#define NROT 5



class School{
    
    private:
        Fish *fishes[NMAX];
        int n_fish;
        vcl::vec3 center;
    public:

        School(int _n_fish);
        Fish *get_instance(int i);
        int get_n_fish();
        void compute_boid();
        void update_all(float dt);

};