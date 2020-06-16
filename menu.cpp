#include "menu.h"

void menu_object::load_pos(ifstream& in)
{
  int first, second;
  in >> first >> second;
  pos = a_coordinates(first, second);
}
void menu_object::load_size(ifstream& in)
{
  int first, second;
  in >> first >> second;
  size = a_coordinates(first, second);
}
void menu_object::_load_colorset(ifstream& in, colorset& C)
{
  unsigned long long colorCode;
  in >> hex >> colorCode >> dec;
  C = colorset(colorCode / 256 / 256 % 256, colorCode / 256 % 256, colorCode % 256, 255);
}
menu_object::menu_object(m_Type id)
{
  switch (id)
  {
  case _M_RECT:
    entry = new rect;
    break;
  case _M_TEXT:
    entry = new text;
    break;
  case _M_BUTTON:
    entry = new button;
    break;
  default:
    assert(false);
    break;
  }
}

void rect::load(ifstream &in)
{
  int i = 0;
  string data;
  while (i >= 0)
  {
    in >> data;
    for (i = 0; i < sizeof(PARSER_OBJECT_FEATURES); i++)
    {
      if (PARSER_OBJECT_FEATURES[i] == data)
        break;
    }
    switch (i)
    {
    case _F_POSITION: //Pos
      load_pos(in);
      break;
    case _F_SIZE: //Size
      load_size(in);
      break;
    case _F_COLOR: //Color
      load_color(in);
      if (color_highlighted == COLORSET_NONE)
        color_highlighted = color;
      break;
    case _F_COLOR_H: //Color Highlighted
      load_color_h(in);
      break;
    case _F_END:
      i = -1;
      break;
    default:
      cerr << "Incorrect input";
      assert(false);
    }
  }
}
scene_object* rect::init_image(menu *head)
{
  scene_object* pic = new rectangle(r_coordinates(pos, head->Size) + head->Pos,
    (r_coordinates(size, head->Size) + r_coordinates(1, -1)),
    color, color_highlighted);
  image = pic;
  return pic;
}
scene_object* rect::get_image(void)
{
  return image;
}
void rect::set_image(scene_object* imageToSet)
{
  image = imageToSet;
}

void text::load_text(ifstream& in)
{
  string input;
  in >> input;
  while (input != PARSER_OBJECT_FEATURES[_F_END])
  {
    sign += " ";
    sign += input;
    in >> input;
  }
}
void text::load(ifstream &in)
{
  int i = 0;
  string data;
  while (i >= 0)
  {
    in >> data;
    for (i = 0; i < sizeof(PARSER_OBJECT_FEATURES); i++)
    {
      if (PARSER_OBJECT_FEATURES[i] == data)
        break;
    }
    switch (i)
    {
    case _F_POSITION:
      load_pos(in);
      break;
    case _F_SIZE:
      load_size(in);
      break;
    case _F_COLOR:
      load_color(in);
      if (color_highlighted == COLORSET_NONE)
        color_highlighted = color;
      break;
    case _F_COLOR_H:
      load_color_h(in);
      break;
    case _F_TEXT_COLOR:
      load_tcolor(in);
      if (text_color_highlighted == COLORSET_NONE)
        text_color_highlighted = text_color;
      break;
    case _F_TEXT_COLOR_H:
      load_tcolor_h(in);
      break;
    case _F_TEXT:
      load_text(in);
      break;
    case _F_END:
      i = -1;
      break;
    default:
      cerr << "Incorrect input" << endl;
      assert(false);
    }
  }
}
scene_object* text::init_image(menu *head)
{
  scene_object* pic = new rectangle_with_text(r_coordinates(pos, head->Size) + head->Pos,
    r_coordinates(size, head->Size) + r_coordinates(1, -1),
    color, color_highlighted,
    sign, text_color, text_color_highlighted);
  image = pic;
  return pic;
}
scene_object* text::get_image(void)
{
  return image;
}
void text::set_image(scene_object* imageToSet)
{
  image = imageToSet;
}

void button::load(ifstream &in)
{
  int i = 0;
  string data;
  while (i >= 0)
  {
    in >> data;
    for (i = 0; i < sizeof(PARSER_OBJECT_FEATURES); i++)
    {
      if (PARSER_OBJECT_FEATURES[i] == data)
        break;
    }
    switch (i)
    {
    case _F_POSITION:
      load_pos(in);
      break;
    case _F_SIZE:
      load_size(in);
      break;
    case _F_COLOR:
      load_color(in);
      if (color_highlighted == COLORSET_NONE)
        color_highlighted = color;
      break;
    case _F_COLOR_H:
      load_color_h(in);
      break;
    case _F_TEXT_COLOR:
      load_tcolor(in);
      if (text_color_highlighted == COLORSET_NONE)
        text_color_highlighted = text_color;
      break;
    case _F_TEXT_COLOR_H:
      load_tcolor_h(in);
      break;
    case _F_TEXT:
      load_text(in);
      break;
    case _F_CODE:
      in >> code;
      break;
    case _F_END:
      i = -1;
      break;
    default:
      cerr << "Incorrect input";
      assert(false);
    }
  }
}
scene_object* button::init_image(menu *head)
{
  scene_object* pic = new button_image(r_coordinates(pos, head->Size)+head->Pos,
    r_coordinates(size, head->Size) + r_coordinates(1, -1),
    color, color_highlighted,
    sign, text_color, text_color_highlighted, 
    code, head->call_back_fun);
  image = pic;
  return pic;
}
scene_object* button::get_image(void)
{
  return image;
}
void button::set_image(scene_object* imageToSet)
{
  image = imageToSet;
}


// menu

void menu::clear(void)
{
  Size = a_coordinates();
  Pos = r_coordinates();
  for (auto obj : objects)
  {
    env->PopObject(obj->get_image());
    delete obj;
  }
  objects.clear();
}
void menu::load(string fileName)
{
  ifstream file;
  file.open(fileName, ifstream::in);
  clear();
  int i = 0;
  string data;

  file >> data;
  while (data != PARSER_OBJECT_NAMES[_M_MENU])
  {
    file >> data;
    if (file.eof())
    {
      cerr << "incorrect menu input file. no menu header";
      assert(false);
    }
  }

  while (i >= 0)
  {
    file >> data;
    for (i = 0; i < sizeof(PARSER_OBJECT_NAMES); i++)
    {
      if (PARSER_OBJECT_NAMES[i] == data)
        break;
    }
    if (i == sizeof(PARSER_OBJECT_NAMES))
    {
      if (data == PARSER_OBJECT_FEATURES[_F_POSITION])
      {
        double posX, posY;
        file >> posX >> posY;
        Pos = r_coordinates(posX, posY);
      }
      if (data == PARSER_OBJECT_FEATURES[_F_SIZE])
      {
        int W, H;
        file >> W >> H;
        Size = a_coordinates(W, H);

      }
    }
    else
    {
      menu_object* n = nullptr;
      switch (i)
      {
      case _M_RECT:
        n = new menu_object(_M_RECT);
        goto inited;
      case _M_TEXT:
        n = new menu_object(_M_TEXT);
        goto inited;
      case _M_BUTTON:
        n = new menu_object(_M_BUTTON);
        goto inited;
      case _M_END:
        i = -1; // breaking the while
        break;
      default:
        cerr << "Incorrect input. Unable to distinguish menu element.";
        assert(false);
      inited:
        n->load(file);
        objects.push_back(n);
        if (env != nullptr)
        {
          env->PushObject(n->init_image(this));
        }
      }

    }
  }
  file.close();
}
void menu::connect_environment(scene* s)
{
  env = s;
  for (auto obj : objects)
  {
    env->PushObject(obj->init_image(this));
  }
}
void menu::disconnect_environment(void)
{
  for (auto obj : objects)
  {
    env->PopObject(obj->get_image());
    obj->set_image(nullptr);
  }
  env = nullptr;
}
void menu::set_callback_fun(void(*f)(int menu_state_code))
{
  call_back_fun = f;
}
menu::menu()
{
  Size = a_coordinates();
  Pos = r_coordinates();
  env = nullptr;
}
menu::~menu(void)
{
  clear();
}
menu::menu(string fileName) { load(fileName); env = nullptr; }
