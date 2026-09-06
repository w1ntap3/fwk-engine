#include "util.h"
#include "main.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

float shake_time = 0;

uint32_t rng_state = 123456789u;
uint32_t fast_rand(void) {
  uint32_t x = rng_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rng_state = x;
  return x;
}

void apply_gravity(struct Particle *particle_array, const double dt) {
  for (int prtcl = 0; prtcl < MAX_PARTICLES; prtcl++) {
    particle_array[prtcl].velocity.y += GRAV_ACCEL * dt;
  }
}

char *randomize_sound(const char *sound_name) {
  static char path[64];
  int ran_num = fast_rand() % MAX_SOUND_VARIATION;

  snprintf(path, sizeof(path), "media/%s%d.mp3", sound_name, ran_num);

  return path;
}

void shake_camera(struct Camera2D *camera, float dt) {
  shake_time -= dt;

  float strength = shake_time / MAX_SHAKE_TIME;

  float x = ((fast_rand() / (float)UINT32_MAX) * 2.0f - 1.0f);
  float y = ((fast_rand() / (float)UINT32_MAX) * 2.0f - 1.0f);

  int shake_mult_x = fast_rand() % MAX_SHAKE_MULTIPLIER;
  int shake_mult_y = fast_rand() % MAX_SHAKE_MULTIPLIER;
  camera->offset.x = WINDOW_WIDTH / 2.0f + x * shake_mult_x * strength;
  camera->offset.y = WINDOW_HEIGHT / 2.0f + y * shake_mult_y * strength;

  if (shake_time <= 0.0f) {
    shake_time = 0.0f;
    camera->offset = (Vector2){WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
  }
}

struct Color random_color() {
  struct Color generated_color;
  // generated_color.a = 100 + fast_rand() % 156;
  generated_color.a = 255;
  generated_color.r = 200 + fast_rand() % 55;
  generated_color.g = 50 + fast_rand() % 170;
  generated_color.b = fast_rand() % 40;
  return generated_color;
}

float face_mouse(const Vector2 pos, const Vector2 mouse) {
  return atan2(mouse.y - pos.y, mouse.x - pos.x) * 180.0f / PI;
}

Vector2 real_mouse_pos(Camera2D camera) {
  return GetScreenToWorld2D(GetMousePosition(), camera);
}
void fireworks_expiration_string(char *buffer, size_t buffer_size) {
  int offset = 0;

  offset += snprintf(buffer + offset, buffer_size - offset, "[");

  for (int i = MAX_FIREWORKS - 1; i >= 0; i--) {
    offset += snprintf(buffer + offset, buffer_size - offset, "%.2f%s",
                       fireworks_array[i].expiration, (i > 0) ? ", " : "");
  }

  snprintf(buffer + offset, buffer_size - offset, "]");
}

void reset_fwk(struct Firework *cur_fwk) {
  cur_fwk->pos = (Vector2){0, 0};
  cur_fwk->active = false;
  cur_fwk->alive = false;
  cur_fwk->expiration = FIREWORK_EXPIRATION;
  cur_fwk->velocity = (Vector2){0, 0};
  cur_fwk->last_dir = (Vector2){0, 0};
  cur_fwk->rot = 0.0f;
  fireworks--;
}
