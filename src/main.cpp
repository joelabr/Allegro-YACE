#include <cstdio>
#include "../include/MainApp.h"
#include "../YACE/include/Chip8.h"

int main(int argc, char** argv)
{
  MainApp* app = 0;
  if (argc > 1)
  {
    app = new MainApp(argv[1]);

    if (argc > 2)
      app->read_config(argv[2]);
    else
    {
      std::string program_directory = argv[0];
      program_directory = program_directory.substr(0, program_directory.rfind("\\"));
      app->read_config(program_directory + "\\config.ini");
    }
  }
  else
  {
    app = new MainApp();
    std::string program_directory = argv[0];
    program_directory = program_directory.substr(0, program_directory.rfind("\\"));
    app->read_config(program_directory + "\\config.ini");
  }

  if (!app)
  {
    fprintf(stderr, "Failed to create main application!\n");
    return -1;
  }

  app->run();
  delete app;

  return 0;
}

//int main(int argc, char** argv)
//{
//  if (argc > 1)
//  {
//   Chip8::Chip8 chip8 = Chip8::Chip8();
//
//   try
//   {
//     chip8.load_game(argv[1]);
//     printf("Done loading!\n\n");
//     chip8.run();
//   }
//   catch (const char* text)
//   {
//     printf("%s\n", text);
//   }
//  }
//
//  return 0;
//}
