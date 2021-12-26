#pragma once

#include "vcl/vcl.hpp"

#define N_OF_CORAILS 30

#define N_OF_CHEST 5

struct perlin_noise_parameters
{
	float persistency = 0.45f;
	float frequency_gain = 2.f;
	int octave = 6;
	float terrain_height = 4.f;
};




const struct perlin_noise_parameters parameters;

const vcl::vec3 X_ROTATION = {-1.,1.,1.};
const vcl::vec3 Y_ROTATION ={1.,-1.,1.};
const vcl::vec3 XY_ROTATION ={-1.,-1.,1.};
const vcl::vec3 NO_ROTATION = {1.,1.,1.};

//chest
vcl::mesh create_chest();

static constexpr float SIZE_OF_CHUNK = 20.;
class Chunk{

    
    private:
            // Number of samples of the terrain is N x N
        const unsigned int N = 100;
        vcl::vec3 position;
        vcl::vec3 scale;
        vcl::mesh chunk_mesh;
        std::vector<vcl::vec3> positions;
        std::vector<vcl::vec3> positions_seaweed;
        std::vector<vcl::vec3> positions_chest;
        std::vector<vcl::vec3> positions_purple;

    public:


        Chunk(vcl::vec3 _scale);
    
        vcl::vec3 get_position();
        vcl::vec3 get_scale();
        vcl::mesh get_chunk_mesh();
        std::vector<vcl::vec3> get_positions();
        std::vector<vcl::vec3> get_positions_seaweed();
        std::vector<vcl::vec3> get_positions_chest();
        std::vector<vcl::vec3> get_positions_purple();


        void set_position(vcl::vec3 _pos);
        void translate(vcl::vec3 _pos);

        vcl::vec3 evaluate_terrain(float u, float v);
        float compute_z(float x, float y);

        vcl::vec3 compute_normal(float x, float y);
};
