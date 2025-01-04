//types.h

#include <sys/ioctl.h>

#ifndef TYPES_H
#define TYPES_H

// this shit is NOT gonna run with 256 objects but ok
#define MAX_OBJECTS 256
#define M_PI 3.14159265358979323846

typedef unsigned char ObjectType;
typedef struct
{
	float x;
	float y;
	float z;
} Vec3;

struct Camera {
	struct winsize _w;
	float fov;
    Vec3 pos;
	Vec3 rot;
};

struct Ray {
	Vec3 origin;
	Vec3 direction;
};

struct Object {
	Vec3 pos;
	Vec3 size;
	Vec3 rot;
	ObjectType type;
	int exists;
};

struct World {
	struct Camera camera;
	struct Object objects[MAX_OBJECTS];
	int object_count;
};

#endif // TYPES_H