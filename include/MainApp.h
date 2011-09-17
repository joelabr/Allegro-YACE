#ifndef MAINAPP_H
#define MAINAPP_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include "../YACE/include/Chip8.h"

class MainApp
{
  public:
    MainApp();
    MainApp(char* filename);
    ~MainApp();

    void read_config(std::string filename);
    void run();
    void save_config(std::string filename);

  private:
    bool game_loaded;
    std::map<int, YACE::Chip8::EMU_KEYS> key_map;
    bool running;
    ALLEGRO_COLOR color_black;
    ALLEGRO_COLOR color_white;
    ALLEGRO_BITMAP* buffer;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;

    YACE::Chip8 chip8;

    void clear_screen();
    void destroy_resources();
    void handle_key_presses(ALLEGRO_EVENT& event);
    void handle_key_releases(ALLEGRO_EVENT& event);
    void init_allegro();
    void loop();
    const char* open_file();
    void process_events();
    void render();
    void update_buffer_bitmap();
};

#endif
