#include "vcl/vcl.hpp"
#include <iostream>
#include "main.hpp"
#include "Terrain.hpp"
#include "Chunk.hpp"
#include "user.hpp"
#include "animals/Animals.hpp"
#include "animals/School.hpp"
#include "animals/Fish.hpp"

using namespace vcl;


scene_environment scene;


void window_size_callback(GLFWwindow* window, int width, int height);

void initialize_data();
void display_scene();
void display_interface();


Animals *all_animals = new Animals(3,100);

mesh_drawable fish;
mesh_drawable billboard_coral;
mesh_drawable billboard_seaweed;
mesh_drawable billboard_purple;
mesh_drawable chunks[16];
mesh_drawable chest;
mesh_drawable  eyes;


void opengl_uniform(GLuint shader, scene_environment const& current_scene)
{
	opengl_uniform(shader, "projection", current_scene.projection);
	opengl_uniform(shader, "time", current_scene.time,false);
	opengl_uniform(shader, "view", current_scene.camera.matrix_view());

	/** Note: Here we use the raw OpenGL call to glUniform3fv allowing us to pass a vector of data (here an array of 5 positions and 5 colors) */
	opengl_uniform(shader, "fog_deph", current_scene.fog_deph);
	opengl_uniform(shader, "god_mode", current_scene.god_mode);

}


int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

	int const width = 1280, height = 1024;
	GLFWwindow* window = create_window(width, height);
	window_size_callback(window, width, height);
	std::cout << opengl_info_display() << std::endl;;

	imgui_init(window);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, keyboard_callback);

	std::cout<<"Initialize data ..."<<std::endl;
	initialize_data();



	user.fps_record.start();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		scene.god_mode=user.gui.god_mod;
		float dt =timer.update();
		scene.time=timer.t;
		scene.light = scene.camera.position();
		user.fps_record.update();

		glClearColor(0.030f,0.30f,0.57f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		imgui_create_frame();
		if(user.fps_record.event) {
			std::string const title = "VCL Display - "+str(user.fps_record.fps)+" fps";
			glfwSetWindowTitle(window, title.c_str());
		}

		ImGui::Begin("GUI",NULL,ImGuiWindowFlags_AlwaysAutoResize);
		user.cursor_on_gui = ImGui::IsAnyWindowFocused();

		if(user.gui.display_frame) draw(user.global_frame, scene);

		Terrain::get_terrain()->update(scene.camera.position_camera);
		
		//update the positions of the fishes
		for(int i=0;i<all_animals->get_n_school();i++)
			all_animals->get_instance(i)->update_all(dt);

		// Handle camera fly-through
		update_camera_position(dt);


		display_interface();
		display_scene();

		ImGui::End();
		imgui_render_frame(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



void initialize_data()
{

	// Load a new custom shader that take into account spotlights (note the new shader file in shader/ directory)
	GLuint const shader_mesh = opengl_create_shader_program(read_text_file("shaders/vertex_shader.glsl"),read_text_file("shaders/fragment_shader.glsl"));

	// set it as default shader
	mesh_drawable::default_shader = shader_mesh;
	mesh_drawable::default_texture = opengl_texture_to_gpu(image_raw{1,1,image_color_type::rgba,{255,255,255,255}});

	GLuint const shader_uniform_color = opengl_create_shader_program(opengl_shader_preset("single_color_vertex"), opengl_shader_preset("single_color_fragment"));
	curve_drawable::default_shader = shader_uniform_color;
	segments_drawable::default_shader = shader_uniform_color;
	
	user.global_frame = mesh_drawable(mesh_primitive_frame());
	
	scene.camera.manipulator_rotate_roll_pitch_yaw(0,pi/2.0f,0.);

	
    // Create visual terrain surface
    //terrain = mesh_drawable(chunk.get_chunk_mesh());
	for(int i=0;i<16;i++){
		chunks[i] = mesh_drawable(Terrain::get_terrain()->get_instance(i).get_chunk_mesh());
	}
	
    //Terrain::get_terrain()->shading.color = {0.6f,0.85f,0.5f};
    //Terrain::get_terrain()->shading.phong.specular = 0.0f; // non-specular terrain material
	


	//add sand texture to the terrain ground
	image_raw const ground = image_load_png("img/texture/uniform_gravel_texture.png");	
	GLuint const texture_image_id = opengl_texture_to_gpu(ground, 
		GL_REPEAT /**GL_TEXTURE_WRAP_S*/, 
		GL_REPEAT /**GL_TEXTURE_WRAP_T*/);
	for(int i=0;i<16;i++){
		chunks[i].texture = texture_image_id;
	}

	image_raw const ecailles = image_load_png("img/texture/ecailles.png");
		GLuint const texture_image_id_2 = opengl_texture_to_gpu(ecailles,
			GL_REPEAT /**GL_TEXTURE_WRAP_S*/,
			GL_REPEAT /**GL_TEXTURE_WRAP_T*/);

		
	fish=mesh_drawable(all_animals->get_instance(0)->get_instance(0)->get_mesh());
	fish.texture = texture_image_id_2;
	

	eyes=mesh_drawable(all_animals->get_instance(0)->get_instance(0)->get_eye());
	//coral billboards

	billboard_coral = mesh_drawable(mesh_primitive_quadrangle({ -1,0,0 }, { 1,0,0 }, { 1,0,2 }, { -1,0,2 }));
	billboard_coral.transform.scale = 0.4f;
	billboard_coral.texture = opengl_texture_to_gpu(image_load_png("img/texture/coral.png"));

	//seaweed billboards

	billboard_seaweed = mesh_drawable(mesh_primitive_quadrangle({ -1,0,0 }, { 1,0,0 }, { 1,0,2 }, { -1,0,2 }));
	billboard_seaweed.transform.scale = 0.4f;
	billboard_seaweed.texture = opengl_texture_to_gpu(image_load_png("img/texture/algue.png"));


	//chest 

	chest = mesh_drawable(create_chest());
	image_raw const im2 = image_load_png("img/texture/wood.png");
	GLuint const texture_image_id_chest = opengl_texture_to_gpu(im2,
		GL_MIRRORED_REPEAT /**GL_TEXTURE_WRAP_S*/,
		GL_MIRRORED_REPEAT /**GL_TEXTURE_WRAP_T*/);
	chest.texture = texture_image_id_chest;
	chest.transform.scale = 0.5;

	//purple coral
	billboard_purple = mesh_drawable(mesh_primitive_quadrangle({ -1,0,0 }, { 1,0,0 }, { 1,0,2 }, { -1,0,2 }));
	billboard_purple.transform.scale = 0.4f;
	billboard_purple.texture = opengl_texture_to_gpu(image_load_png("img/texture/purple.png"));


	
}


void display_scene()
{	
	if(user.gui.god_mod )
		chunks[Terrain::get_terrain()->get_center()].shading.color = {0.6f,0.85f,0.5f};
		
	for(int i=0;i<16;i++){
		vec3 chunk_position=Terrain::get_terrain()->get_instance(i).get_position();
		if (user.gui.god_mod || norm(chunk_position- scene.camera.position_camera)<=(1-0.9*scene.fog_deph)*SIZE_OF_CHUNK+SIZE_OF_CHUNK*0.6){
			chunks[i].transform.translate=chunk_position;
			if(user.gui.god_mod  && i != Terrain::get_terrain()->get_center())
				chunks[i].shading.color={1.,1.,1.}; 
			//std::cout << i << ": " << Terrain::get_terrain()->get_instance(i).get_position().x << " " <<Terrain::get_terrain()->get_instance(i).get_position().y << " " << Terrain::get_terrain()->get_instance(i).get_position().z << "\n";
			draw(chunks[i], scene);
		}
	}

	//move the mesh_drawable of the fishes to theirs new locations
	for(int s=0;s<all_animals->get_n_school();s++){
		for(int i=0;i<all_animals->get_instance(s)->get_n_fish();i++){
			Fish *actual_fish = all_animals->get_instance(s)->get_instance(i);
			fish.transform.translate=actual_fish->get_position();
			fish.transform.rotate = actual_fish->get_rotation();
			draw(fish,scene);
			eyes.transform.translate=actual_fish->get_position()+actual_fish->get_rotation()*vec3(0.05,0,0.02);
			draw(eyes,scene);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// coral

	for(int i=0;i<16;i++){
		std::vector<vec3> billboard_position = Terrain::get_terrain()->get_instance(i).get_positions();
		std::vector<vec3> billboard_position_seaweed = Terrain::get_terrain()->get_instance(i).get_positions_seaweed();
		std::vector<vec3> billboard_position_chest = Terrain::get_terrain()->get_instance(i).get_positions_chest();
		std::vector<vec3> billboard_position_purple = Terrain::get_terrain()->get_instance(i).get_positions_purple();
		vec3 terrain_position = Terrain::get_terrain()->get_instance(i).get_position();
		if (user.gui.god_mod || norm(terrain_position- scene.camera.position_camera)<=(1-0.9*scene.fog_deph)*SIZE_OF_CHUNK+SIZE_OF_CHUNK*0.6){
			//draw(chest, scene);
			for (size_t k = 0; k < billboard_position.size(); ++k)
			{
				billboard_coral.transform.translate = terrain_position + billboard_position[k];
				billboard_seaweed.transform.translate = terrain_position + billboard_position_seaweed[k];
				billboard_purple.transform.translate = terrain_position + billboard_position_purple[k];
				
				for (int i = 0; i < 5; i++) {
					billboard_coral.transform.rotate = rotation(vec3{ 0,0,1 }, 2 * 3.14f * i / 5);
					billboard_seaweed.transform.rotate = rotation(vec3{ 0,0,1 }, 2 * 3.14f * i / 5);
					billboard_purple.transform.rotate = rotation(vec3{ 0,0,1 }, 2 * 3.14f * i / 5);
					glDepthMask(false);
					draw(billboard_coral, scene);
					draw(billboard_seaweed, scene);
					draw(billboard_purple, scene);
					glDepthMask(true);
				}
			}
			for (size_t k = 0; k < billboard_position_chest.size(); ++k)
			{
				chest.transform.translate = terrain_position + billboard_position_chest[k];
				draw(chest, scene);
			}
		}


	}
}


void display_interface()
{
	ImGui::Checkbox("Frame", &user.gui.display_frame);
	ImGui::Checkbox("God Mod", &user.gui.god_mod);
	ImGui::SliderFloat("fog", &scene.fog_deph, 0, 0.1f, "%0.3f");
	//ImGui::SliderFloat("speed of fishes", &max_speed, 0., 5.f, "%0.1f");
	#ifdef DEBUG
	ImGui::Text("x value: %f",scene.camera.position_camera.x);
	ImGui::Text("y value: %f",scene.camera.position_camera.y);
	#endif
}


void window_size_callback(GLFWwindow* , int width, int height)
{
	glViewport(0, 0, width, height);
	float const aspect = width / static_cast<float>(height);
	scene.projection = projection_perspective(50.0f*pi/180.0f, aspect, 0.1f, 100.0f);
}




