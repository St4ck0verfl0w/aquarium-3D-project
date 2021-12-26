

#include "vcl/vcl.hpp"
#include <iostream>
#include <list>

#include "user.hpp"
#include "Terrain.hpp"

#include "main.hpp"

// Store keyboard state
// left-right / up-down key

using namespace vcl;

user_interaction_parameters user;

void keyboard_callback( GLFWwindow* , int key, int , int action, int )
{
	if(key == GLFW_KEY_UP){
		if(action == GLFW_PRESS) user.keyboard_state.up = true;
		if(action == GLFW_RELEASE) user.keyboard_state.up = false;
	}

	if(key == GLFW_KEY_DOWN){
		if(action == GLFW_PRESS) user.keyboard_state.down = true;
		if(action == GLFW_RELEASE) user.keyboard_state.down = false;
	}

	if(key == GLFW_KEY_LEFT){
		if(action == GLFW_PRESS) user.keyboard_state.left = true;
		if(action == GLFW_RELEASE) user.keyboard_state.left = false;
	}

	if(key == GLFW_KEY_RIGHT){
		if(action == GLFW_PRESS) user.keyboard_state.right = true;
		if(action == GLFW_RELEASE) user.keyboard_state.right = false;
	}
	if(key == GLFW_KEY_SPACE){
		if(action == GLFW_PRESS) user.keyboard_state.space = true;
		if(action == GLFW_RELEASE) user.keyboard_state.space = false;
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	vec2 const  p1 = glfw_get_mouse_cursor(window, xpos, ypos);
	vec2 const& p0 = user.mouse_prev;
	glfw_state state = glfw_current_state(window);

	//auto& camera = scene.camera;
	if(!user.cursor_on_gui){
		if(state.mouse_click_left && !state.key_ctrl)
			scene.camera.manipulator_rotate_trackball(p0, p1);
		// if(state.mouse_click_left && state.key_ctrl)
		// 	camera.manipulator_translate_in_plane(p1-p0);
		// if(state.mouse_click_right)
			//camera.manipulator_scale_distance_to_center( (p1-p0).y );
	}

	user.mouse_prev = p1;
}

void update_camera_position(float dt){
	
	float speed=user.speed_x;
	float rotation_speed=user.rotation;
	if (user.gui.god_mod){
		speed*=3;
		rotation_speed*=2;
	}
	float mult= speed*dt;
	if(user.keyboard_state.up)
		scene.camera.position_camera += mult*scene.camera.front();
	if(user.keyboard_state.down)
		scene.camera.position_camera -= mult*scene.camera.front();
	if(user.keyboard_state.left)
		scene.camera.manipulator_rotate_roll_pitch_yaw(0.,0.,rotation_speed*dt);
	if(user.keyboard_state.right)
		scene.camera.manipulator_rotate_roll_pitch_yaw(0.,0.,-rotation_speed*dt);


	vec3 pos_cam = scene.camera.position_camera;
	float min_z =Terrain::get_terrain()->compute_z(pos_cam.x,pos_cam.y)+user.height;

	//handle jumping

	if (scene.camera.position_camera.z<min_z && user.speed_z<=0){
		scene.camera.position_camera.z=min_z;
		user.jumping=false;
		user.speed_z=0;
	}

	if(!user.jumping){

		if(!user.gui.god_mod && scene.camera.position_camera.z>min_z){
			user.jumping=true;
			user.speed_z=0;
		}

		if (user.keyboard_state.space){
			user.speed_z=10.;
			user.jumping =true;
			scene.camera.position_camera.z+=0.2;
		}
	}else{
		float abs_speed = user.speed_z > 0 ? user.speed_z : -user.speed_z;
		user.speed_z+=-7*dt-0.5*user.speed_z*abs_speed*dt;
		scene.camera.position_camera.z+=user.speed_z*dt;
	
	}
}
