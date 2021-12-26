#include "Animals.hpp"
#include "vcl/vcl.hpp"


Animals::Animals(int _n_school, int _n_of_fish){
    n_school=_n_school;
    n_fish_per_school=_n_of_fish;
    for(int i = 0; i< n_school;i++){
        schools[i]=new School(_n_of_fish);
        for(int j =0;j<_n_of_fish;j++){
            schools[i]->get_instance(j)->translate(vcl::vec3(i,i,i));
        }

    }

}

School *Animals::get_instance(int i){
    return schools[i];
}

int Animals::get_n_school(){
    return n_school;
}
int Animals::get_n_fish_per_school(){
    return n_fish_per_school;
}
