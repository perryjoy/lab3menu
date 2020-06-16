#pragma once
#include "menu.h"
#include <string>
#include <iostream>
#include <fstream>
#include "my_graphics.h"
#include <set>
#include <cassert>
#include <vector>
using namespace std;

typedef enum m_Type
{
  _M_RECT = 0,
  _M_BUTTON,
  _M_TEXT,
  _M_CHOICE,
  _M_OPTION,
  _M_MENU,
  _M_END
} m_Type;

typedef enum m_Feat
{
  _F_POSITION = 0,
  _F_SIZE,
  _F_TEXT,
  _F_COLOR,
  _F_COLOR_H,
  _F_TEXT_COLOR,
  _F_TEXT_COLOR_H,
  _F_CODE,
  _F_END
} m_Feat;

typedef enum menu_reply
{
  _MENU_CLOSED = 0,
  _MENU_BUTTON = 512,
  _MENU_CHOICE = 1024
} menu_reply;



const string PARSER_OBJECT_NAMES[] = { "Rectangle", "Button", "Text", "Choice", "Option", "Menu", "End" };

const string PARSER_OBJECT_FEATURES[] = { "Position", "Size", "Text", "Color", "Color(H)", "TColor", "TColor(H)", "Code", "End" };


class menu;

class menu_object
{
private:
  menu_object* entry;
protected:
  a_coordinates pos;
  a_coordinates size;
  scene_object* image;
public:

  menu_object() : entry(0)
  {
    pos = a_coordinates();
    size = a_coordinates();
    image = nullptr;
  }
  menu_object(m_Type t);
  virtual scene_object* init_image(menu *head) { return entry->init_image(head); }
  virtual scene_object* get_image(void) { return entry->get_image(); }
  virtual void set_image(scene_object* imageToSet) { entry->set_image(imageToSet); }
  virtual void load(ifstream &in) { return entry->load(in); }

  virtual ~menu_object(void) { delete entry; entry = nullptr; }


  void load_pos(ifstream& in);
  void load_size(ifstream& in);
  void _load_colorset(ifstream& in, colorset& C);

};


class rect : public menu_object
{
protected:
  colorset color;
  colorset color_highlighted;

public:
  void load_color(ifstream& in) { _load_colorset(in, color); }
  void load_color_h(ifstream& in) { _load_colorset(in, color_highlighted); }
  virtual void load(ifstream &in) override;
  virtual scene_object* init_image(menu *head) override;
  virtual scene_object* get_image(void) override;
  virtual void set_image(scene_object* imageToSet) override;
private:
  rect() : menu_object() { color = COLORSET_NONE; color_highlighted = COLORSET_NONE; }
  friend menu_object;
  friend class text;
  friend class button;
};

class text : public rect
{
protected:
  string sign;
  colorset text_color;
  colorset text_color_highlighted;

public:
  void load_tcolor(ifstream& in) { _load_colorset(in, text_color); }
  void load_tcolor_h(ifstream& in) { _load_colorset(in, text_color_highlighted); }
  void load_text(ifstream& in);
  virtual void load(ifstream &in) override;
  virtual scene_object* init_image(menu *head) override;
  virtual scene_object* get_image(void) override;
  virtual void set_image(scene_object* imageToSet) override;
private:
  friend menu_object;
  friend class button;
  text() : rect() { text_color = COLORSET_NONE; text_color_highlighted = COLORSET_NONE; sign = ""; }
};

class button : public text
{
protected:
  int code;

public:
  virtual void load(ifstream &in) override;
  virtual scene_object* init_image(menu *head) override;
  virtual scene_object* get_image(void) override;
  virtual void set_image(scene_object* imageToSet) override;
private:
  button() : text() { code = 0; }
  friend menu_object;
  friend button;
};



class menu
{
private:
  a_coordinates Size;
  r_coordinates Pos;
  vector <menu_object *> objects;
  scene *env;
  void(*call_back_fun)(int code);
  friend scene_object* menu_object::init_image(menu *head);
  friend scene_object* rect::init_image(menu *head);
  friend scene_object* text::init_image(menu *head);
  friend scene_object* button::init_image(menu *head);
public:
  void clear(void);
  void load(string fileName);
  void connect_environment(scene* s);
  void disconnect_environment(void);
  void set_callback_fun(void(*f)(int menu_state_code));
  menu();
  ~menu(void);
  menu(string fileName);
  
};