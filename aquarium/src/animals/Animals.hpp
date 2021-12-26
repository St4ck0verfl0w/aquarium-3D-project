#pragma once

#include "vcl/vcl.hpp"
#include "Fish.hpp"
#include "School.hpp"

#define N_SCHOOL_MAX 3
class Animals{
    
    private:
        int n_school;
        int n_fish_per_school;
        School *schools[N_SCHOOL_MAX];

    public:

        Animals(int _n_school, int _n_of_fish_per_school);
        School *get_instance(int i);
        int get_n_school();
        int get_n_fish_per_school();

};