#include <pthread.h>
#include <SDL2/SDL.h>

#ifdef __linux__
#ifdef USE_GLES1
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <SDL2/SDL.h>
#include <CoreAudio/CoreAudio.h>
#endif

#define PROJECTM_BUFSIZE 512
#include <projectM.hpp>
extern "C" {
#include "defs.h"
#include "map.h"
#include "oo.h"
#include "type.h"
#include "import.h"
#include "ugen.h"
#include "vm.h"
}

#include <limits.h>

typedef struct {
    projectM *pm;
    SDL_Window *win;
    SDL_Renderer *rend;
    bool done;
    projectM::Settings settings;
    short pcm_data[2][512];
	m_uint count;
} projectMApp;


const float FPS = 60;


/*
void ResizeWindow()

{

screen_width = event.resize.w;
screen_height = event.resize.h;

//SDL_SetVideoMode(screen_width, screen_height, bpp, SDL_OPENGL | SDL_RESIZABLE | SDL_DOUBLEBUF);

}
*/




void* renderFrame(void *data) {
	projectMApp* app = (projectMApp*)data;
    app->pm->pcm()->addPCM16(app->pcm_data);


//    glClearColor( 0.0, 0.5, 0.0, 0.0 );
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, app->settings.windowWidth, app->settings.windowHeight);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, app->settings.windowWidth, 0, app->settings.windowHeight, -1, 1);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();


    app->pm->renderFrame();
    glFlush();
    SDL_RenderPresent(app->rend);
}

void* projectm_run(void* data)
{
	projectMApp* app = (projectMApp*)data;
	int width  = 680, height = 680;

    app->settings.meshX = 1;
    app->settings.meshY = 1;
    app->settings.fps   = FPS;
    app->settings.textureSize = 2048;  // idk?
    app->settings.windowWidth = width;
    app->settings.windowHeight = height;
    app->settings.smoothPresetDuration = 3; // seconds
    app->settings.presetDuration = 5; // seconds
    app->settings.beatSensitivity = 0.8;
    app->settings.aspectCorrection = 1;
    app->settings.easterEgg = 0; // ???
    app->settings.shuffleEnabled = 1;
    app->settings.softCutRatingsEnabled = 1; // ???
    app->settings.presetURL = "presets/presets_milkdrop";
    app->settings.menuFontURL = "fonts/Vera.ttf";
    app->settings.titleFontURL = "fonts/Vera.ttf";

    SDL_Init(SDL_INIT_VIDEO);
    app->win = SDL_CreateWindow("Gwion projectM plugin", 0, 0, width, height, SDL_WINDOW_RESIZABLE);
    app->rend = SDL_CreateRenderer(app->win, 0, SDL_RENDERER_ACCELERATED);
    if (! app->rend) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetWindowTitle(app->win, "ProjectM for Gwion");

    // init projectM
    app->pm = new projectM(app->settings);
    app->pm->selectRandom(true);
    app->pm->projectM_resetGL(width, height);


    const Uint32 frame_delay = 1000/FPS;
    Uint32 last_time = SDL_GetTicks();

    while (! app->done) {
        renderFrame(app);
        Uint32 elapsed = SDL_GetTicks() - last_time;
        if (elapsed < frame_delay)
            SDL_Delay(frame_delay - elapsed);
        last_time = SDL_GetTicks();
    }
}

m_bool projectm_tick(UGen u)
{
  projectMApp* app = (projectMApp*)u->ug;
  u->channel[0]->ugen->out = u->channel[0]->ugen->in;
  u->channel[1]->ugen->out = u->channel[1]->ugen->in;
  short l = (u->channel[0]->ugen->in * SHRT_MAX);
  short r = (u->channel[1]->ugen->in * SHRT_MAX);
  app->pcm_data[0][app->count % PROJECTM_BUFSIZE] = l;
  app->pcm_data[1][app->count % PROJECTM_BUFSIZE] = r;
  u->out = u->last = (u->channel[0]->ugen->out + u->channel[1]->ugen->out)/2;
  app->count++;
}

extern "C"
{

CTOR(projectm_ctor) {
pthread_t t;
    projectMApp* app = new projectMApp();
    o->ugen->ug = app;
    o->ugen->n_in = 2;
    o->ugen->n_out = 2;
    o->ugen->tick = projectm_tick;
    o->ugen->ugen = new_Vector();
    o->ugen->channel = (M_Object*)calloc(2, sizeof(struct M_Object_));
    o->ugen->channel[0] = new_M_UGen();
    o->ugen->channel[0]->ugen->n_in = 1;
    o->ugen->channel[0]->ugen->n_out = 1;
    o->ugen->channel[0]->ugen->ugen = new_Vector();
    o->ugen->channel[0]->ugen->ref = o->ugen;
    o->ugen->channel[1] = new_M_UGen();
    o->ugen->channel[1]->ugen->n_in = 1;
    o->ugen->channel[1]->ugen->n_out = 1;
    o->ugen->channel[1]->ugen->ugen = new_Vector();
    o->ugen->channel[1]->ugen->ref = o->ugen;

	pthread_create(&t, NULL, projectm_run, app);
}

DTOR(projectm_dtor)
{
	delete (projectMApp*)o->ugen->ug;
//	SDL_Quit();
}

static struct Type_ t_projectm= { (m_str)"ProjectM", SZ_INT, &t_ugen };
m_bool import(Env env)
{
  CHECK_BB(add_global_type(env, &t_projectm))
  CHECK_BB(import_class_begin(env, &t_projectm, env->global_nspc, projectm_ctor, projectm_dtor))
  CHECK_BB(import_class_end(env));
  return 1;
}
}
