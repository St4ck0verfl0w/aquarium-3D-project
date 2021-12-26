#pragma once

#include "vcl/vcl.hpp"


#include "main.hpp"
#include "animals/Fish.hpp"


// Available options on the Graphic user interface
struct gui_parameters {
	bool display_frame = false;
	bool god_mod = false;
	double fish_speed = max_speed;
	double fog = scene.fog_deph;
	#ifdef DEBUG
	double xval = 0.;
	double yval = 0.;
	#endif
};


// recognized keys
struct keyboard_state_parameters{
	bool left  = false; 
	bool right = false; 
	bool up    = false; 
	bool down  = false;
	bool space  = false;
};

//all interactive parameters
struct user_interaction_parameters {
	
    vcl::vec2 mouse_prev;
    vcl::timer_fps fps_record;
	vcl::mesh_drawable global_frame;
	bool cursor_on_gui;
   	gui_parameters gui; 
	keyboard_state_parameters keyboard_state;
	float speed_x = 3.f;
	float speed_z=0;
	bool jumping=false;

	float rotation = 3.f;
	float height = 0.8f;
};
extern user_interaction_parameters user;


// handle keyboard arrows
void keyboard_callback( GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);

void update_camera_position(float dt);
