#include "game.hxx"
#include <SDL.h>

#include <iostream>

// extern "C"
// {
//    int SDL_main(int argc, char* argv[])
//    {
//        std::cout << " ------------- SDL MAIN ------------ ";
//                SDL_Log("SDL init failed");

//       return 0;
//    }
// }

int protected_main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  bool skipMenu = false;

  // add commandline parameter to skipMenu
  for (int i = 1; i < argc; ++i)
  {

    if (std::string(argv[i]) == "--skipMenu")
    {
      skipMenu = true;
    }
  }
  Game game;

  if (!game.initialize())
  {
    return EXIT_FAILURE;
  }

  if (!skipMenu)
  {
    game.mainMenu();
  }
  game.run();
  game.shutdown();

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    Game game;
   if (!game.initialize())
  {
    return EXIT_FAILURE;
  }
    game.mainMenu();
  game.shutdown();

}
