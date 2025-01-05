#include "camera.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float dot(Vec3 *v1, Vec3 *v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}
float dist(Vec3 *v1, Vec3 *v2) {
    float dx = v1->x - v2->x;
    float dy = v1->y - v2->y;
    float dz = v1->z - v2->z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}
Vec3 normalize(Vec3 v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    Vec3 result = {v.x / length, v.y / length, v.z / length};
    return result;
}
Vec3 subtract(Vec3 *v1, Vec3 *v2) {
    Vec3 result = {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z};
    return result;
}
Vec3 inverse(Vec3 *v) {
    Vec3 result = {-v->x, -v->y , -v->z};
    return result;
}
Vec3 rotate_vector(Vec3 v, Vec3 rotation) {
    float cos_x = cosf(rotation.x);
    float sin_x = sinf(rotation.x);
    float cos_y = cosf(rotation.y);
    float sin_y = sinf(rotation.y);
    float cos_z = cosf(rotation.z);
    float sin_z = sinf(rotation.z);
    
    // x axis (pitch)
    float y1 = v.y * cos_x - v.z * sin_x;
    float z1 = v.y * sin_x + v.z * cos_x;
    
    // y axis (yaw)
    float x2 = v.x * cos_y + z1 * sin_y;
    float z2 = -v.x * sin_y + z1 * cos_y;
    
    // z axis (roll)
    float x3 = x2 * cos_z - y1 * sin_z;
    float y3 = x2 * sin_z + y1 * cos_z;
    
    return (Vec3){x3, y3, z2};
}
Vec3 point(Vec3 *origin, Vec3 *to) { // this does NOT fucking work with our stupid system
    Vec3 direction = {
        to->x - origin->x,
        to->y - origin->y,
        to->z - origin->z
    };
    
    Vec3 normalized_direction = normalize(direction);

    float yaw = atan2(normalized_direction.z, normalized_direction.x) * (180 / M_PI);
    float pitch = asin(normalized_direction.y) * (180 / M_PI);
    
    Vec3 result = {yaw, pitch, 0};

    return result;
}

// char gpt
int ray_intersect_object(const struct Ray *_ray, const struct Object *obj) {
	if (obj->type == 0) { // cube
		Vec3 local_origin = subtract(&_ray->origin, &obj->pos);
		Vec3 inv_rotation = inverse(&obj->rot);

		struct Ray local_ray = {
			rotate_vector(local_origin, inv_rotation),
			rotate_vector(_ray->direction, inv_rotation)
		};
		
		Vec3 min = {
			-obj->size.x / 2,
			-obj->size.y / 2,
			-obj->size.z / 2
		};
		Vec3 max = {
			obj->size.x / 2,
			obj->size.y / 2,
			obj->size.z / 2
		};

		float tx1 = local_ray.direction.x != 0 ? (min.x - local_ray.origin.x) / local_ray.direction.x : 1e30f;
		float tx2 = local_ray.direction.x != 0 ? (max.x - local_ray.origin.x) / local_ray.direction.x : 1e30f;
		float tmin = fminf(tx1, tx2);
		float tmax = fmaxf(tx1, tx2);

		float ty1 = local_ray.direction.y != 0 ? (min.y - local_ray.origin.y) / local_ray.direction.y : 1e30f;
		float ty2 = local_ray.direction.y != 0 ? (max.y - local_ray.origin.y) / local_ray.direction.y : 1e30f;
		tmin = fmaxf(tmin, fminf(ty1, ty2));
		tmax = fminf(tmax, fmaxf(ty1, ty2));

		float tz1 = local_ray.direction.z != 0 ? (min.z - local_ray.origin.z) / local_ray.direction.z : 1e30f;
		float tz2 = local_ray.direction.z != 0 ? (max.z - local_ray.origin.z) / local_ray.direction.z : 1e30f;
		tmin = fmaxf(tmin, fminf(tz1, tz2));
		tmax = fminf(tmax, fmaxf(tz1, tz2));

		if (tmax < tmin || tmax <= 0) {
			return 0; // no fucking intersection
		}

		Vec3 intersection_point = {
			local_ray.origin.x + tmin * local_ray.direction.x,
			local_ray.origin.y + tmin * local_ray.direction.y,
			local_ray.origin.z + tmin * local_ray.direction.z
		};

		if (fabsf(intersection_point.x - min.x) < 1e-4) { // left face | "missing texture"
            int int1 = 13; // checkerboard 1
            int int2 = 0; // checkerboard 2

            float x = intersection_point.z - min.z;
            float y = intersection_point.y - min.y;
            int checker = (int)(floorf(x * 3.0f) + floorf(y * 3.0f)) % 2;

            if (checker == 1) {
                return int1;
            } else {
                return int2;
            }
		} else if (fabsf(intersection_point.x - max.x) < 1e-4) { // right face | rainbow flag :3
            float gradient = (intersection_point.z - min.z) / (max.z - min.z); // normalize to 0-1
            gradient = fmaxf(0.0f, fminf(gradient, 1.0f)); // clamp to [0, 1]

            int ansi_colors[] = {196, 208, 227, 154, 27, 57, 213, 0, 0};
            int color_count = sizeof(ansi_colors) / sizeof(ansi_colors[0]);

            int index = (int)(gradient * (color_count - 1));
            return ansi_colors[index];
		} else if (fabsf(intersection_point.y - min.y) < 1e-4) { // bottom face? | transgender flag :3
            float gradient = (intersection_point.z - min.z) / (max.z - min.z); // normalize to 0-1
            gradient = fmaxf(0.0f, fminf(gradient, 1.0f)); // clamp to [0, 1]

            int ansi_colors[] = {75, 219, 255, 219, 75, 0};
            int color_count = sizeof(ansi_colors) / sizeof(ansi_colors[0]);

            int index = (int)(gradient * (color_count - 1));
            return ansi_colors[index];
		} else if (fabsf(intersection_point.y - max.y) < 1e-4) { // top face? | simple gradient
            int int1 = 232; // ansi start 1
            int int2 = 251; // ansi end 2

            float gradient = (intersection_point.z - min.z) / (max.z - min.z); // normalize y to 0-1
            gradient = fmaxf(0.0f, fminf(gradient, 1.0f)); // clamp to [0, 1]

            return (int)((int2 - int1) * gradient) + int1;
		} else if (fabsf(intersection_point.z - min.z) < 1e-4) {
			return 7; // back face
		} else if (fabsf(intersection_point.z - max.z) < 1e-4) {
            return 7; // front face
		}

        return 0;
	}
	if (obj->type == 1) {

        // spheroid intersection
        Vec3 local_origin = {
            _ray->origin.x - obj->pos.x,
            _ray->origin.y - obj->pos.y,
            _ray->origin.z - obj->pos.z
        };

        Vec3 direction = _ray->direction;

        Vec3 scaled_origin = {
            local_origin.x / obj->size.x,
            local_origin.y / obj->size.y,
            local_origin.z / obj->size.z
        };

        Vec3 scaled_direction = {
            direction.x / obj->size.x,
            direction.y / obj->size.y,
            direction.z / obj->size.z
        };

        // quadratic eq t^2 * (d . d) + 2t * (o . d) + (o . o) - 1 = 0
        float a = scaled_direction.x * scaled_direction.x +
                  scaled_direction.y * scaled_direction.y +
                  scaled_direction.z * scaled_direction.z;

        float b = 2 * (scaled_origin.x * scaled_direction.x +
                       scaled_origin.y * scaled_direction.y +
                       scaled_origin.z * scaled_direction.z);

        float c = scaled_origin.x * scaled_origin.x +
                  scaled_origin.y * scaled_origin.y +
                  scaled_origin.z * scaled_origin.z - 1;

        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return 0; // no touching wait pause
        }

        float sqrt_discriminant = sqrtf(discriminant);
        float t1 = (-b - sqrt_discriminant) / (2 * a);
        float t2 = (-b + sqrt_discriminant) / (2 * a);

        if (t2 < 0) {
            return 0;
        }

        return 1;
	}
	return 0;
}
struct Ray get_ray_for_pixel(const struct Camera *camera, int pixel_x, int pixel_y) {
    struct Ray ray;
    
    float char_aspect = 0.5f;
    float screen_aspect = ((float)camera->_w.ws_col / camera->_w.ws_row) * char_aspect;
    
    float fov_rad = camera->fov * M_PI / 180.0f;
    
    float x = (2.0f * ((float)pixel_x + 0.5f) / camera->_w.ws_col - 1.0f) * screen_aspect * tanf(fov_rad / 2.0f);
    float y = (1.0f - 2.0f * ((float)pixel_y + 0.5f) / camera->_w.ws_row) * tanf(fov_rad / 2.0f);
    
    Vec3 direction = {x, y, -1.0f};
    
    Vec3 rotation_rad = {
        camera->rot.x * M_PI / 180.0f,
        camera->rot.y * M_PI / 180.0f,
        camera->rot.z * M_PI / 180.0f
    };
    
    direction = rotate_vector(direction, rotation_rad);
    
    direction = normalize(direction);
    
    ray.origin = camera->pos;
    ray.direction = direction;
    
    return ray;
}

void set_camera_position(struct Camera *camera, float x, float y, float z) {
    camera->pos.x = x;
    camera->pos.y = y;
    camera->pos.z = z;
}
void set_camera_rotation(struct Camera *camera, float pitch, float yaw, float roll) {
    camera->rot.x = pitch;
    camera->rot.y = yaw;
    camera->rot.z = roll;
}

void render_camera(const struct Camera *camera, const struct World *world) {

	// kill meeeeeee
    int x, y;
    for (int y = 0; y < camera->_w.ws_row; y++) {
        for (int x = 0; x < camera->_w.ws_col; x++) {
            struct Ray ray = get_ray_for_pixel(camera, x, y);

            int hit = 0;
            for (int i = 0; i < world->object_count; i++) { // Loop over objects in the world
                // I wonder how not optimized this is
				if (hit == 0) {
					hit = ray_intersect_object(&ray, &world->objects[i]);
				}
            }

            if (hit) {
                int col = hit;
				// int col = 201; // unk face
				// if (hit == 1) {
				// 	col = 196;
				// }
				// if (hit == 2) {
				// 	col = 214;
				// }
				// if (hit == 3) {
				// 	col = 220;
				// }
				// if (hit == 4) {
				// 	col = 154;
				// }
				// if (hit == 5) {
				// 	col = 21;
				// }
				// if (hit == 6) {
				// 	col = 13;
				// }
                printf("\x1b[38;5;%dm%d", col, 1); // RENDER COLOR!!! FUCK YOU!!
            } else {
                printf("\x1b[38;5;232m%d", 0); // no hit
            }
        }
        printf("\n");
    }
}