// world.c
#include "world.h"
#include "camera.h"
#include <stdio.h>

void render_world(struct World *world) {
    struct Camera *camera = &world->camera;
    render_camera(camera, world);
}

void set_object_position(struct Object *object, float x, float y, float z) {
    object->pos.x = x;
    object->pos.y = y;
    object->pos.z = z;
}

void set_object_size(struct Object *object, float x, float y, float z) {
    object->size.x = x;
    object->size.y = y;
    object->size.z = z;
}

void set_object_rotation(struct Object *object, float x, float y, float z) {
    object->rot.x = x;
    object->rot.y = y;
    object->rot.z = z;
}