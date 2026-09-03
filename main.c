#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define FPS_STR_BUF 32
#define RUNS_STR_BUF 32

#define JAVID_GREEN (Color){0x1f, 0x28, 0x1f, 0xFF}
#define JAVID_PURPLE (Color){0xaf, 0x81, 0xcf, 0xFF}

#define PARTICLE_RADIUS (float)10
#define PARTICLE_CENTER_DEFAULT                                                \
  (Vector2){(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2}

#define MAX_VEL_X 500  // px/s
#define MAX_VEL_Y 500  // px/s
#define GRAV_ACCEL 400 // px/s^2

#define MAX_SOUND_VARIATION 5

#define SHAKE_MULTIPLIER 10

struct Particle {
  Vector2 center;
  float radius;
  Vector2 velocity;
};

uint32_t rng_state = 123456789u;
int runs = 0;
float shake_time = 0;

static inline uint32_t fast_rand(void) {
  uint32_t x = rng_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rng_state = x;
  return x;
}

static void apply_gravity(struct Particle *particles, const int particle_count,
                          const double dt) {
  for (int prtcl = 0; prtcl < particle_count; prtcl++) {
    particles[prtcl].velocity.y += GRAV_ACCEL * dt;
  }
  return;
}

void reset_particles(struct Particle *particles, const int particle_count,
                     const Vector2 center) {
  memset(particles, 0, particle_count * sizeof(struct Particle));
  for (int prtcl = 0; prtcl < particle_count; prtcl++) {
    particles[prtcl].center = center;
    particles[prtcl].radius = PARTICLE_RADIUS;
    particles[prtcl].velocity.x =
        (((fast_rand() >> 8) / 16777216.0f) * 2.0f - 1.0f) * MAX_VEL_X;
    particles[prtcl].velocity.y =
        (((fast_rand() >> 8) / 16777216.0f) * 2.0f - 1.0f) * MAX_VEL_Y;
  }
  runs++;
  printf("Initialized %d particles (Run #%d)\n", particle_count, runs);
}

char *randomize_sound(const char *sound_name) {
  static char path[64];
  int ran_num = fast_rand() % MAX_SOUND_VARIATION;

  snprintf(path, sizeof(path), "%s%d.mp3", sound_name, ran_num);

  return path;
}

void shake_camera(struct Camera2D *camera) {
  int x_shake = fast_rand() % SHAKE_MULTIPLIER;
  int y_shake = fast_rand() % SHAKE_MULTIPLIER;
  camera->offset.x += x_shake;
  camera->offset.y += y_shake;
  shake_time -= 0.01;
}

int main(int argc, char *argv[]) {
  SetTraceLogLevel(LOG_NONE);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raylibbing");
  InitAudioDevice();

  int refresh_rate = GetMonitorRefreshRate(GetCurrentMonitor());
  printf("Refresh rate is %d Hz\n", refresh_rate);
  SetTargetFPS(refresh_rate);

  int particle_count = atoi(argv[1]);
  struct Particle particles[particle_count];
  reset_particles(particles, particle_count, PARTICLE_CENTER_DEFAULT);

  Camera2D camera;
  camera.offset = (Vector2){0, 0};
  camera.target = (Vector2){0, 0};
  camera.rotation = 0;
  camera.zoom = 1;

  while (!WindowShouldClose()) {
    char fps_str[FPS_STR_BUF];
    char runs_str[RUNS_STR_BUF];
    snprintf(fps_str, sizeof(fps_str), "%d", GetFPS());
    snprintf(runs_str, sizeof(runs_str), "%d", runs);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Sound particle_sound = LoadSound(randomize_sound("particle"));
      reset_particles(particles, particle_count, GetMousePosition());
      shake_time = 0.2f;
      PlaySound(particle_sound);
    }
    if (shake_time > 0) {
      shake_camera(&camera);
    }

    if (IsKeyPressed(KEY_Q)) {
      CloseWindow();
      return 0;
    }
    if (IsKeyPressed(KEY_MINUS)) {
      camera.zoom -= 0.1f;
    }
    double dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(JAVID_GREEN);
    BeginMode2D(camera);
    apply_gravity(particles, particle_count, dt);
    for (int prtcl = 0; prtcl < particle_count; prtcl++) {
      particles[prtcl].center.x += particles[prtcl].velocity.x * dt;
      particles[prtcl].center.y += particles[prtcl].velocity.y * dt;
      DrawCircleGradient(particles[prtcl].center, PARTICLE_RADIUS, JAVID_GREEN,
                         JAVID_PURPLE);
    }
    DrawText(fps_str, 0, 0, 40, JAVID_PURPLE);
    DrawText(runs_str, 0, 40, 40, JAVID_PURPLE);
    EndMode2D();
    EndDrawing();
  }
  return 1;
}
