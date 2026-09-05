#ifndef UTIL_H
#define UTIL_H

#include "main.h"
#include "raylib.h"
#include <stdint.h>
#include <stdio.h>

extern int runs;
extern float shake_time;

uint32_t fast_rand(void);
char *randomize_sound(const char *sound_name);
void apply_gravity(struct Particle *particle_array, const double dt);
void shake_camera(struct Camera2D *camera, float dt);
struct Color random_color();
float face_mouse(const Vector2 pos, const Vector2 mouse);
Vector2 real_mouse_pos(Camera2D camera);
void fireworks_expiration_string(char *buffer, size_t buffer_size);
void reset_fwk(struct Firework *cur_fwk);
#endif
