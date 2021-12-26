#pragma once

#include "Terrain.hpp"

//#define DEBUG

struct scene_environment
{
	vcl::camera_head camera;
	vcl::mat4 projection;
	vcl::vec3 light;
	float fog_deph =.01;
	float spotlight_falloff = 0.5;
	float time;
	bool god_mode;
};


static float max_speed=1.;

static vcl::timer_basic timer;
extern scene_environment scene;