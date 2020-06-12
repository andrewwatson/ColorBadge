
#ifndef COLOR_H
#define COLOR_H

#include <stdlib.h>
#include <iostream>
#include <string.h>

class Color
{
  private:
    long long m_color;

  public:
    
    Color(long long color);
    Color(char* hex);


    void SetColor(long long color);
    long long FromString(char* hex);
    int Green();
    int Red();
    int Blue();
};

#endif
