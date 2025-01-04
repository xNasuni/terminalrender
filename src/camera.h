//camera.h

#include "types.h"

#ifndef CAMERA_H
#define CAMERA_H

// i only ever use point well actually i dont use point but i was gonna use point so i decided to like 'publicize' all these but i was only ever gonna use point but i cant even use point to POINT THE CAMERA TO LOOK AT THE THING AND IT JUST DOESNT BECAUSE MY ENTIRE CODEBASE IS FUCKED SO I JUST GIVE UP WHY ARE YOU READING THIS
float dot(Vec3 *v1, Vec3 *v2);
Vec3 normalize(Vec3 v);
Vec3 subtract(Vec3 *v1, Vec3 *v2);
Vec3 inverse(Vec3 *v);
Vec3 rotate_vector(Vec3 v, Vec3 rotation);
Vec3 point(Vec3 *origin, Vec3 *to);

void set_camera_position(struct Camera *camera, float x, float y, float z);
void set_camera_rotation(struct Camera *camera, float x, float y, float z);
void render_camera(const struct Camera *camera, const struct World *world);

#endif // CAMERA_H