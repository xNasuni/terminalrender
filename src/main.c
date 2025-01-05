#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#include "camera.h"
#include "world.h"
#include "types.h"

int main()	
{
	struct winsize w;

	// what the fuck is a ticgownsnz
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
		perror("ioctl");
		return 1;
	}

    const int fps = 30;
    const double frame_time = 1.0 / fps;
    
	const int scene = 1;

    clock_t last_time = clock();
    srand(time(NULL));

	struct World world;
	world.camera._w = w;
	world.camera.fov = 70.0f;

	if (scene == 1) {
		set_camera_position(&world.camera, 0.0f, 0.5f, -2.0f);
		set_camera_rotation(&world.camera, 0.0f, 0.0f, 0.0f);
		world.objects[0] = (struct Object){ 
			{0.0f, -2.0f, -1.0f}, // pos
			{4.0f, 0.5f, 1.0f}, // size
			{0.0f, 0.0f, 0.0f}, // rot
			0, // type
			1 // uh i forgot
		};

		world.objects[1] = (struct Object){
			{0.0f, 2.0f, -1.0f}, // pos
			{4.0f, 0.5f, 1.0f}, // size
			{0.0f, 0.0f, 0.0f}, // rot
			0, // type
			1 // uh i forgot
		};

		world.objects[2] = (struct Object){
			{2.0f, 0.0f, -1.0f}, // pos
			{0.5f, 4.0f, 1.0f}, // size
			{0.0f, 0.0f, 0.0f}, // rot
			0, // type
			1 // uh i forgot
		};

		world.objects[3] = (struct Object){
			{-2.0f, 0.0f, -1.0f}, // pos
			{0.5f, 4.0f, 1.0f}, // size
			{0.0f, 0.0f, 0.0f}, // rot
			0, // type
			1 // uh i forgot
		};

		world.object_count = 4;
			
	} else {
		set_camera_position(&world.camera, 0.0f, -1.0f, 1.0f);
		set_camera_rotation(&world.camera, 235.0f / 365.0f, 0.0f, 0.0f);
		world.objects[0] = (struct Object){ 
			{0.0f, -1.0f, -1.0f}, // pos
			{1.0f, 1.0f, 1.0f}, // size
			{0.0f, 0.0f, 0.0f}, // rot
			0, // type
			1 // uh i forgot
		};
		world.object_count = 1;
	}

	printf("\x1b[2J");
    float curr = 0;
	while (1) {
        clock_t current_time = clock();
        double delta_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;

        if (delta_time >= frame_time) {
            last_time = current_time;
			curr = curr + delta_time * 60;
            printf("\x1b[1H");

			if ((float)curr >= 365) { // modulus operator is like not working so idk
				curr = 0;
			}

			// set_object_position(&world.objects[0], -curr / 64.0f, -1.0f, -1.0f);
			
			if (scene == 1) {
				set_camera_rotation(&world	.camera, 235.0f, 0.0f, curr);
			} else {
				set_object_rotation(&world.objects[0], curr / -11.5f, 0.0f, curr / 16.0f); // skirt go spinny :3333333333333333333
			}
			render_world(&world); // erm

            printf("\x1b[1H");
			printf("\x1b[0mCLK: %.6f\n", curr);
			printf("\x1b[0mFPS: %.6f\n", 1.0 / delta_time);
			printf("%f, %f, %f\n", world.camera.rot.x, world.camera.rot.y, world.camera.rot.z);
			printf("\x1b[1H");
            fflush(stdout); // so this makes the thing run like faster
		}
	}

	// please no memory leaks 🥺🥺🥺🙏
	return 0;
}
