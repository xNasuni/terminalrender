//world.h

#include "types.h"
#include <math.h>

#ifndef WORLD_H
#define WORLD_H

void render_world(struct World *world);
void set_object_position(struct Object *object, float x, float y, float z);
void set_object_size(struct Object *object, float x, float y, float z);
void set_object_rotation(struct Object *object, float x, float y, float z);

#endif // WORLD_H