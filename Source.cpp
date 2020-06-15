#include <iostream>
#include "menu.h"

int menu_state = -1;

void set_menu_state(int code)
{
  menu_state = code;
}

int main(int argc, char* args[]) // end me pls
{
 
  scene S(900, 900);
  menu test;
  test.load("main_menu.txt");
  test.set_callback_fun(set_menu_state);
  test.connect_environment(&S);

  SDL_Event event;
  
  while (menu_state == -1)
  {
    S.DrawScene();
    if (menu_state == 0 || S.event_flags&_EVENT_WINDOWS_CLOSED)
    {
      menu_state = -1;
      test.load("sure.txt");
      
      test.set_callback_fun(set_menu_state);
    }
    if (menu_state == 11)
    {
      test.load("main_menu.txt");
      menu_state = -1;
    }
    if (menu_state == 10)
    {
      test.disconnect_environment();
      break;
    }
  }

  return 0;
}