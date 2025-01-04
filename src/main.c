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

    const int fps = 60;
    const double frame_time = 1.0 / fps;
    
    clock_t last_time = clock();

	struct World world;
	world.camera._w = w;
	world.camera.fov = 70.0f;
	set_camera_position(&world.camera, 0.0f, 0.5f, -2.0f);
	set_camera_rotation(&world.camera, 0.0f, 0.0f, 0.0f);

	world.objects[0] = (struct Object){ 
		{0.0f, -1.0f, -1.0f}, // pos
		{1.0f, 1.0f, 1.0f}, // size
		{0.0f, 0.0f, 0.0f}, // rot
		0, // type
		1 // uh i forgot
	};

	// TARGET BALL!! RED YIPPE!!!
	// world.objects[1] = (struct Object){ 
	// 	{0.0f, -1.0f, -1.0f},
	// 	{0.5f, 1.0f, 0.5f},
	// 	{0.0f, 0.0f, 0.0f},
	// 	1,
	// 	1
	// };

	world.object_count = 1;

	printf("\x1b[2J");
    float curr = 0;
	while (1) {
        clock_t current_time = clock();
        double delta_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;

        if (delta_time >= frame_time) {
            last_time = current_time;
			curr = curr + delta_time * 10;
            printf("\x1b[1H");

			if ((float)curr >= 360) { // modulus operator is like not working so idk
				curr = 0;
			}

			// set_object_position(&world.objects[0], -curr / 64.0f, -1.0f, -1.0f);
			set_object_rotation(&world.objects[0], curr / -2.5f, 0.0f, curr); // skirt go spinny :3333333333333333333
			
			set_camera_rotation(&world	.camera, 235.0f, 0.0f, 0.0f);
			// Vec3 direction = point(&world.camera.pos, &world.objects[0].pos);
			// world.camera.rot = direction; THIS FUCKING THING DOESNT WANNA WORK AGRFSHUGEGIDK QG IDMGKDS QEGMJUTGD KMS

			render_world(&world); // erm

            printf("\x1b[1H");
			printf("\x1b[0mCLK: %.6f\n", curr);
			printf("\x1b[0mFPS: %.6f\n", 1.0 / delta_time);
			printf("%f, %f, %f\n", world.camera.rot.x, world.camera.rot.y, world.camera.rot.z);

            fflush(stdout); // so this makes the thing run like faster
		}
	}

	// please no memory leaks 🥺🥺🥺🙏
	return 0;
}