#include "main.h"
#include "util.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int explosions = 0;
int fireworks = 0;

struct Particle particle_array[MAX_PARTICLES];
struct Firework fireworks_array[MAX_FIREWORKS];
struct Explosion explosion_array[MAX_FIREWORKS];

int main(void) {
  SetTraceLogLevel(LOG_NONE);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raylibbing");
  InitAudioDevice();

  int refresh_rate = GetMonitorRefreshRate(GetCurrentMonitor());
  printf("Refresh rate is %d Hz\n", refresh_rate);
  SetTargetFPS(refresh_rate);

  Camera2D camera;
  camera.offset = (Vector2){WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
  camera.target = (Vector2){WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
  camera.rotation = 0;
  camera.zoom = 1.0f;

  Texture2D firework_texture = LoadTexture(FIREWORK_TEXTURE);
  Texture2D particle_texture = LoadTexture(PARTICLE_TEXTURE);

  while (!WindowShouldClose()) {
    double dt = GetFrameTime();
    char fps_str[FPS_STR_BUF];
    char runs_str[RUNS_STR_BUF];
    char fwks_str[FWKS_STR_BUF];
    fireworks_expiration_string(fwks_str, FWKS_STR_BUF);
    snprintf(fps_str, sizeof(fps_str), "%d", GetFPS());
    snprintf(runs_str, sizeof(runs_str), "%d", runs);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      struct Firework new_firework;
      new_firework.pos = real_mouse_pos(camera);
      new_firework.velocity = (Vector2){0, 0};
      new_firework.expiration = FIREWORK_EXPIRATION;
      new_firework.alive = true;
      new_firework.active = true;
      summon_firework(new_firework);
    }

    if (shake_time > 0) {
      shake_camera(&camera, dt);
    }

    if (IsKeyPressed(KEY_Q)) {
      CloseWindow();
      return 0;
    }

    if (IsKeyDown(KEY_MINUS)) {
      camera.zoom -= 0.01f;
    }
    if (IsKeyDown(KEY_EQUAL)) {
      camera.zoom += 0.01f;
    }
    if (camera.zoom < 0.1f)
      camera.zoom = 0.1f;

    BeginDrawing();
    ClearBackground(MY_GREEN);
    BeginMode2D(camera);
    apply_gravity(particle_array, dt);
    for (int prtcl = 0; prtcl < MAX_PARTICLES; prtcl++) {
      handle_prtcl(prtcl, dt, particle_texture);
    }

    for (int fwk = 0; fwk < MAX_FIREWORKS; fwk++) {
      handle_fwk(fwk, camera, dt, firework_texture);
    }

    EndMode2D();

    DrawText(fps_str, 0, 0, 20, MY_PURPLE);
    DrawText(runs_str, 0, 20, 20, MY_PURPLE);
    DrawText(fwks_str, 0, 40, 20, MY_PURPLE);
    EndDrawing();
  }
  return 1;
}

void handle_fwk(int fwk, Camera2D camera, double dt,
                Texture2D firework_texture) {
  struct Firework *cur_fwk = &fireworks_array[fwk];

  if (cur_fwk->expiration < 0.0f) {
    cur_fwk->expiration = 0.0f;
    if (cur_fwk->alive || cur_fwk->active) {
      Sound particle_sound = LoadSound(randomize_sound("particle"));
      PlaySound(particle_sound);
      call_particle_array(particle_array, cur_fwk->pos);
      fireworks--;
    }
    cur_fwk->alive = false;
    cur_fwk->active = false;
  }

  Vector2 min = GetScreenToWorld2D((Vector2){0, 0}, camera);
  Vector2 max =
      GetScreenToWorld2D((Vector2){WINDOW_WIDTH, WINDOW_HEIGHT}, camera);

  if (cur_fwk->pos.x < min.x || cur_fwk->pos.x > max.x ||
      cur_fwk->pos.y < min.y || cur_fwk->pos.y > max.y) {
    Sound particle_sound = LoadSound(randomize_sound("particle"));
    PlaySound(particle_sound);
    call_particle_array(particle_array, cur_fwk->pos);
    reset_fwk(cur_fwk);
  }

  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && cur_fwk->active) {
    cur_fwk->active = false;
  }
  if (cur_fwk->alive) {
    cur_fwk->expiration -= FIREWORK_EXPIRATION_SPEED;
    if (cur_fwk->active) {
      cur_fwk->velocity.x =
          FIREWORK_VEL * (real_mouse_pos(camera).x - cur_fwk->pos.x);
      cur_fwk->velocity.y =
          FIREWORK_VEL * (real_mouse_pos(camera).y - cur_fwk->pos.y);
      cur_fwk->last_dir.x = cur_fwk->velocity.x;
      cur_fwk->last_dir.y = cur_fwk->velocity.y;
      cur_fwk->rot = face_mouse(cur_fwk->pos, real_mouse_pos(camera));
    } else {
      float vel_x, vel_y;
      if (cur_fwk->last_dir.x >= 0) {
        vel_x = FIREWORK_CONSTANT_VEL;
      } else {
        vel_x = (-1.0f) * FIREWORK_CONSTANT_VEL;
      }
      if (cur_fwk->last_dir.y >= 0) {
        vel_y = FIREWORK_CONSTANT_VEL;
      } else {
        vel_y = (-1.0f) * FIREWORK_CONSTANT_VEL;
      }
      cur_fwk->velocity.x = vel_x;
      cur_fwk->velocity.y = vel_y;
    }
    cur_fwk->pos.x += cur_fwk->velocity.x * dt;
    cur_fwk->pos.y += cur_fwk->velocity.y * dt;
    DrawTexturePro(firework_texture,
                   (Rectangle){0, 0, (float)firework_texture.width,
                               (float)firework_texture.height},
                   (Rectangle){cur_fwk->pos.x, cur_fwk->pos.y, FIREWORK_HEIGHT,
                               FIREWORK_WIDTH},
                   (Vector2){FIREWORK_HEIGHT / 2, FIREWORK_WIDTH / 2},
                   cur_fwk->rot, DEFAULT_TINT);
  }
}
void handle_prtcl(int prtcl, double dt, Texture2D particle_texture) {
  particle_array[prtcl].pos.x += particle_array[prtcl].velocity.x * dt;
  particle_array[prtcl].pos.y += particle_array[prtcl].velocity.y * dt;
  DrawTexturePro(
      particle_texture,
      (Rectangle){0, 0, (float)particle_texture.width,
                  (float)particle_texture.height},
      (Rectangle){particle_array[prtcl].pos.x, particle_array[prtcl].pos.y,
                  PARTICLE_SIZE, PARTICLE_SIZE},
      (Vector2){PARTICLE_SIZE / 2, PARTICLE_SIZE / 2}, 0.0f, DEFAULT_TINT);
}

void call_particle_array(struct Particle *particle_array, const Vector2 pos) {
  memset(particle_array, 0, MAX_PARTICLES * sizeof(struct Particle));
  for (int prtcl = 0; prtcl < MAX_PARTICLES; prtcl++) {
    particle_array[prtcl].pos = pos;
    int random_max_vel_x = fast_rand() % MAX_VEL_X;
    int random_max_vel_y = fast_rand() % MAX_VEL_X;
    particle_array[prtcl].velocity.x =
        (((fast_rand() >> 8) / 16777216.0f) * 2.0f - 1.0f) * random_max_vel_x;
    particle_array[prtcl].velocity.y =
        (((fast_rand() >> 8) / 16777216.0f) * 2.0f - 1.0f) * random_max_vel_y;
  }
  runs++;
  printf("Initialized %d particle_array (Run #%d)\n", MAX_PARTICLES, runs);
}

int summon_firework(const struct Firework new_fwk) {
  if (fireworks >= MAX_FIREWORKS) {
    fprintf(stderr, "Cannot summon a firework since there are too many\n");
    return -1;
  }
  int fwk_element;
  for (int element = 0; element < MAX_FIREWORKS; element++) {
    if (!fireworks_array[element].alive) {
      fwk_element = element;
    }
  }
  fireworks_array[fwk_element] = new_fwk;
  fireworks++;
  return fireworks;
}
