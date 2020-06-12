#include "colors.h"
#include <stdlib.h>
#include <iostream>

Color::Color(long long color)
{
  SetColor(color);
}

Color::Color(char* hexstring)
{
  long long number = strtoll( &hexstring[1], NULL, 16);
  SetColor(number);
}

void Color::SetColor(long long color)
{
  m_color = color;
}

int Color::Green()
{
  return (m_color >> 8) & 0xFF;
}

int Color::Blue()
{
  return m_color & 0xFF;
}

int Color::Red()
{
  return (m_color >> 16) & 0xFF;
}

long long Color::FromString(char* hexstring)
{
  long long number = strtoll( &hexstring[1], NULL, 16);
  return number;
}
