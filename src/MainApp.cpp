#include "../include/MainApp.h"

MainApp::MainApp() : game_loaded(false), key_map(std::map<int, YACE::Chip8::EMU_KEYS>()), running(true),
                     buffer(0), display(0), event_queue(0), timer(0), chip8()
{
  init_allegro();

  // Clear screen after initialization
  clear_screen();

  // Start timer
  al_start_timer(timer);
}

MainApp::MainApp(char* filename) : game_loaded(false), key_map(std::map<int, YACE::Chip8::EMU_KEYS>()), running(true), buffer(0), display(0), event_queue(0),
                                   timer(0), chip8()
{
  init_allegro();

  try
  {
    chip8.load_game(filename);
    game_loaded = true;
  }
  catch (const char* text)
  {
    fprintf(stderr, "Caught exception! %s\n", text);
  }

  // Clear screen after initialization
  clear_screen();

  // Start timer
  al_start_timer(timer);
}

MainApp::~MainApp()
{
  destroy_resources();
}

/*
 *  Private methods
 */
void MainApp::clear_screen()
{
  al_clear_to_color(color_white);
  al_flip_display();
}

void MainApp::destroy_resources()
{
  al_destroy_bitmap(buffer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  al_destroy_timer(timer);
  al_uninstall_keyboard();
  al_shutdown_primitives_addon();
}

void MainApp::update_buffer_bitmap()
{
  al_set_target_bitmap(buffer);
  al_clear_to_color(color_white);

  const char* video = chip8.get_video();

  int x, y;
  for (int i = 0; i < 0x800; i++)
  {
    x = i % 64;
    y = i / 64;

    if (video[i])
      al_draw_filled_rectangle(x * 10, y * 10, x * 10 + 10, y * 10 + 10, color_black);
  }

  // Reset target to backbuffer
  al_set_target_backbuffer(display);
}

void MainApp::handle_key_presses(ALLEGRO_EVENT& event)
{
  if (key_map[event.keyboard.keycode])
    chip8.set_key(key_map[event.keyboard.keycode], true);
  else
  {
    switch (event.keyboard.keycode)
    {
      case ALLEGRO_KEY_ESCAPE:
        running = false;
        break;
      case ALLEGRO_KEY_F1:
        if (game_loaded)
        {
          game_loaded = false;
          chip8.reset();
        }
        const char* filename = open_file();

        if (strcmp(filename, "") != 0)
        {
          chip8.load_game(filename);
          game_loaded = true;
        }
        delete filename;
        break;
    }
  }
}

void MainApp::handle_key_releases(ALLEGRO_EVENT& event)
{
  if (key_map[event.keyboard.keycode])
    chip8.set_key(key_map[event.keyboard.keycode], false);
}

void MainApp::init_allegro()
{
  if (!al_init())
  {
    fprintf(stderr, "Failed to initialize Allegro!\n");
    exit(-1);
  }

  if (!al_init_primitives_addon())
  {
    fprintf(stderr, "Failed to initialize Allegro primitives addon!\n");
    exit(-1);
  }

  if (!al_install_keyboard())
  {
    fprintf(stderr, "Failed to install keyboard!\n");
    exit(-1);
  }

  display = al_create_display(640, 320);
  if (!display)
  {
    fprintf(stderr, "Failed to create display!\n");
    exit(-1);
  }

  buffer = al_create_bitmap(640, 320);
  if (!buffer)
  {
    fprintf(stderr, "Failed to create buffer!\n");
    exit(-1);
  }

  event_queue = al_create_event_queue();
  if (!event_queue)
  {
    fprintf(stderr, "Failed to create event queue!\n");
    exit(-1);
  }

  timer = al_create_timer(1.0 / 60.0);
  if (!timer)
  {
    fprintf(stderr, "Failed to create timer!\n");
    exit(-1);
  }

  // Register event sources to event queue
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  color_black = al_map_rgb(0, 0, 0);
  color_white = al_map_rgb(255, 255, 255);
}

void MainApp::loop()
{
  chip8.step();
}

const char* MainApp::open_file()
{
  ALLEGRO_FILECHOOSER* filechooser = 0;
  filechooser = al_create_native_file_dialog("", "Load ROM", "*.*", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
  if (!filechooser)
  {
    fprintf(stderr, "Failed to create dialog!\n");
    return "";
  }

  if (!al_show_native_file_dialog(display, filechooser))
  {
    fprintf(stderr, "Failed to open dialog!\n");
    al_destroy_native_file_dialog(filechooser);
    return "";
  }

  char* file = new char[strlen(al_get_native_file_dialog_path(filechooser, 0))];
  strcpy(file, al_get_native_file_dialog_path(filechooser, 0));

  al_destroy_native_file_dialog(filechooser);

  return file;
}

void MainApp::process_events()
{
  ALLEGRO_EVENT event;
  al_wait_for_event(event_queue, &event);

  switch (event.type)
  {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      running = false;
      break;
    case ALLEGRO_EVENT_KEY_DOWN:
      handle_key_presses(event);
      break;
    case ALLEGRO_EVENT_KEY_UP:
      handle_key_releases(event);
      break;
    case ALLEGRO_EVENT_TIMER:
      if (game_loaded)
      {
        loop();
        render();
      }
      break;
  }
}

void MainApp::render()
{
  update_buffer_bitmap();
  al_draw_bitmap(buffer, 0, 0, 0);

  al_flip_display();
}

/*
 *  Public methods
 */
void MainApp::read_config(std::string filename)
{
  ALLEGRO_CONFIG* config_file = 0;
  config_file = al_load_config_file(filename.c_str());

  if (!config_file)
  {
    fprintf(stderr, "Failed to read configuration file: %s\n", filename.c_str());
    return;
  }

  printf("BG-Color: %s\n", al_get_config_value(config_file, "General", "BGColor"));
  printf("FG-Color: %s\n", al_get_config_value(config_file, "General", "FGColor"));

  chip8.set_cpu_cycles(atoi(al_get_config_value(config_file, "Emulation", "Cycles")));

  key_map[atoi(al_get_config_value(config_file, "Keys", "Key0"))] = chip8.KEY_0;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key1"))] = chip8.KEY_1;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key2"))] = chip8.KEY_2;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key3"))] = chip8.KEY_3;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key4"))] = chip8.KEY_4;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key5"))] = chip8.KEY_5;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key6"))] = chip8.KEY_6;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key7"))] = chip8.KEY_7;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key8"))] = chip8.KEY_8;
  key_map[atoi(al_get_config_value(config_file, "Keys", "Key9"))] = chip8.KEY_9;
  key_map[atoi(al_get_config_value(config_file, "Keys", "KeyA"))] = chip8.KEY_A;
  key_map[atoi(al_get_config_value(config_file, "Keys", "KeyB"))] = chip8.KEY_B;
  key_map[atoi(al_get_config_value(config_file, "Keys", "KeyC"))] = chip8.KEY_C;
  key_map[atoi(al_get_config_value(config_file, "Keys", "KeyD"))] = chip8.KEY_D;
  key_map[atoi(al_get_config_value(config_file, "Keys", "KeyE"))] = chip8.KEY_E;
  key_map[atoi(al_get_config_value(config_file, "Keys", "KeyF"))] = chip8.KEY_F;

  al_destroy_config(config_file);
}

void MainApp::run()
{
  while (running)
  {
    process_events();
  }
}

void MainApp::save_config(std::string filename)
{

}
