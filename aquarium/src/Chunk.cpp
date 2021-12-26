#include "Chunk.hpp"

using namespace vcl;

const int SIZE_ARRAY=4;
float h[SIZE_ARRAY]={3,-1.5,1,2};
float sigma[SIZE_ARRAY]={0.5,0.15,0.2,0.2};
vec2 p[SIZE_ARRAY]={vec2(0.,0.),vec2(-0.5,0.5),vec2(-0.2,-0.4),vec2(-0.2,0.5)};

Chunk::Chunk(vcl::vec3 _scale){

            // INITIALIZE 
    position={0.0f, 0.0f, 0.0f};
    scale = _scale;

            // INITIALIZE chunk_mesh
    { 
        // Number of samples of the terrain is N x N

        chunk_mesh.position.resize(N*N);
        chunk_mesh.uv.resize(N*N);
        // Fill terrain geometry
        for(unsigned int ku=0; ku<N; ++ku)
        {
            for(unsigned int kv=0; kv<N; ++kv)
            {
                // Compute local parametric coordinates (u,v) \in [0,1]
                const float u = ku/(N-1.0f);
                const float v = kv/(N-1.0f);

                // Compute the local surface function
                float const x = SIZE_OF_CHUNK*(u-0.5f);
                float const y = SIZE_OF_CHUNK*(v-0.5f);

                float z = compute_z(x,y);
               
                vec3 const p = {x,y,z};

                // Store vertex coordinates
                chunk_mesh.position[kv+N*ku] = p;
                chunk_mesh.uv[kv+N*ku] = {N/5*u,N/5*v};
            }
        }



    // Generate triangle organization
        //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
        for(size_t ku=0; ku<N-1; ++ku)
        {
            for(size_t kv=0; kv<N-1; ++kv)
            {
                const unsigned int idx = kv + N*ku; // current vertex offset

                const uint3 triangle_1 = {idx, idx+1+N, idx+1};
                const uint3 triangle_2 = {idx, idx+N, idx+1+N};

                chunk_mesh.connectivity.push_back(triangle_1);
                chunk_mesh.connectivity.push_back(triangle_2);
            }
        }

        chunk_mesh.fill_empty_field(); // need to call this function to fill the other buffer with default values (normal, color, etc)
    }

    for (size_t k = 0; k < N_OF_CORAILS; ++k)
    {
        const float u = vcl::rand_interval(SIZE_OF_CHUNK)-SIZE_OF_CHUNK/2;
        const float v = vcl::rand_interval(SIZE_OF_CHUNK)-SIZE_OF_CHUNK/2;
        vcl::vec3 const p = {u, v, compute_z(u, v)-0.25};

        positions.push_back(p);
    }

    for (size_t k = 0; k < N_OF_CORAILS; ++k)
    {
        const float u = vcl::rand_interval(SIZE_OF_CHUNK) - SIZE_OF_CHUNK / 2;
        const float v = vcl::rand_interval(SIZE_OF_CHUNK) - SIZE_OF_CHUNK / 2;
        vcl::vec3 const p = { u, v, compute_z(u, v) - 0.25 };

        positions_seaweed.push_back(p);
    }

    for (size_t k = 0; k < N_OF_CHEST; ++k)
    {
        const float u = vcl::rand_interval(SIZE_OF_CHUNK) - SIZE_OF_CHUNK / 2;
        const float v = vcl::rand_interval(SIZE_OF_CHUNK) - SIZE_OF_CHUNK / 2;
        vcl::vec3 const p = { u, v, compute_z(u, v) };

        positions_chest.push_back(p);
    }

    for (size_t k = 0; k < N_OF_CORAILS; ++k)
    {
        const float u = vcl::rand_interval(SIZE_OF_CHUNK) - SIZE_OF_CHUNK / 2;
        const float v = vcl::rand_interval(SIZE_OF_CHUNK) - SIZE_OF_CHUNK / 2;
        vcl::vec3 const p = { u, v, compute_z(u, v) - 0.25 };

        positions_purple.push_back(p);
    }

}


vcl::vec3 Chunk::get_position(){
    return position;
}
vcl::vec3 Chunk::get_scale(){
    return scale;
}
mesh Chunk::get_chunk_mesh(){
    return chunk_mesh;
}

std::vector<vcl::vec3> Chunk::get_positions(){
    return positions;
}

std::vector<vcl::vec3> Chunk::get_positions_seaweed() {
    return positions_seaweed;
}

std::vector<vcl::vec3> Chunk::get_positions_chest() {
    return positions_chest;
}

std::vector<vcl::vec3> Chunk::get_positions_purple() {
    return positions_purple;
}

void Chunk::set_position(vcl::vec3 _pos){
    position = _pos;
}

void Chunk::translate(vcl::vec3 _pos){
    position += _pos;
}

float Chunk::compute_z(float _x,float _y){
    float u=_x/SIZE_OF_CHUNK;
    float v=_y/SIZE_OF_CHUNK;
    u=0.5+(u)*scale.x;
    v=0.5+(v)*scale.y;
    float z=0.0;

    vec2 u0;
    float sigma0;
    float d;

    for(int i=0;i<SIZE_ARRAY;i++){
        u0=p[i];
        sigma0=sigma[i];
        d = norm(vec2(u,v)-u0)/sigma0;

        z += h[i]*std::exp(-d*d);
    }
    float const noise = noise_perlin({u, v}, parameters.octave, parameters.persistency, parameters.frequency_gain);
    z += parameters.terrain_height*(noise-1);

    return z*scale.z;
}


vcl::vec3 Chunk::compute_normal(float x, float y){
    float u=x/SIZE_OF_CHUNK;
    float v=y/SIZE_OF_CHUNK;
    u=0.5+(u)*scale.x;
    v=0.5+(v)*scale.y;
    float z=Chunk::compute_z(x,y);
    vec3 normal=vec3(0.,0.,0.);
    
    for(int i=-1;i<=1;i++){
        for(int j =-1; j<=1;j++){
            if (i*i != j*j){
                float x2=x + i*(1./(2.*N));
                float y2=y + j*(1./(2.*N));
                float z2=compute_z(x2,y2);
                if(j==0){
                    normal+=normalize(vec3(i*(z-z2),0,i*(x2-x)));
                }else{
                    normal+=normalize(vec3(0,j*(z-z2),j*(y2-y)));
                }
            }
        }
    }

    return normalize(normal);

}

vcl::mesh create_chest() {
    float const H = 1.0f;
    mesh base = mesh_primitive_cube({ 0,0,0 }, H);
    base.color.fill({ 0.9f, 0.8f, 0.5f });

    mesh cyl = mesh_primitive_cylinder(H / 2, { 0,-H * 0.5f,H * 0.5f }, { 0,0.5f * H,0.5f * H }, 10, 20, true);
    base.color.fill({ 0.9f, 0.8f, 0.5f });

    mesh chest = base;
    chest.push_back(cyl);

    return chest;

}