#ifndef MAIN_H
#define MAIN_H

#include "raylib.h"

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000
#define FPS_STR_BUF 32
#define FWKS_STR_BUF 256

#define MAX_PARTICLES 1000
#define MAX_FIREWORKS 10

#define MY_GREEN (Color){0x1f, 0x28, 0x1f, 0xFF}
#define MY_PURPLE (Color){0xaf, 0x81, 0xcf, 0xFF}

#define PARTICLE_SIZE (float)100.0f
#define PARTICLE_pos_DEFAULT                                                   \
  (Vector2){(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2}

#define FIREWORK_WIDTH (float)150.0f
#define FIREWORK_HEIGHT (float)300.0f
#define FIREWORK_EXPIRATION (float)5.0f
#define FIREWORK_EXPIRATION_SPEED (float)0.01f

#define MAX_VEL_X 1500 // px/s
#define MAX_VEL_Y 1500 // px/s
#define FIREWORK_VEL 2
#define FIREWORK_CONSTANT_VEL 4000
#define GRAV_ACCEL 400 // px/s^2

#define MAX_SOUND_VARIATION 5

#define MAX_SHAKE_TIME 0.6f
#define MAX_SHAKE_MULTIPLIER 100

#define FIREWORK_TEXTURE "media/firework.png"
#define PARTICLE_TEXTURE "media/explosion_particle.png"
#define DEFAULT_TINT (Color){255, 255, 255, 255}

struct Particle {
  Vector2 pos;
  Vector2 velocity;
  bool seen;
};

struct Explosion {
  struct Particle *particle_array;
};

struct Firework {
  Vector2 pos;
  Vector2 velocity;
  Vector2 last_dir;
  float rot;
  float expiration;
  bool alive;
  bool active;
};

extern int explosions;
extern int fireworks;

extern struct Explosion explosion_array[MAX_FIREWORKS];
extern struct Firework fireworks_array[MAX_FIREWORKS];
extern struct Particle particle_array[MAX_PARTICLES];

void call_particle_array(struct Particle *particle_array, const Vector2 pos);
int summon_firework(const struct Firework new_fwk);

void handle_fwk(int fwk, Camera2D *camera, double dt,
                Texture2D firework_texture);
void handle_prtcl(int prtcl, double dt, Texture2D particle_texture);
#endif
