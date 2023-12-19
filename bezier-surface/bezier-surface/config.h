#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include <array>
#include <math.h>
#include <chrono>
# define M_PI           3.14159265358979323846
typedef unsigned int uint;

// settings
const uint SCR_WIDTH = 1200;
const uint SCR_HEIGHT = 1200;
const uint DOUBLE_CLICK_TIME_SPAN = 700;
const uint DEFAULT_TRIANGLES_DENSITY = 3;
const uint MAX_TRIANGLES_DENSITY = 30;

//template<typename T = int>
//struct point
//{
//public:
//    T x = -1, y = -1;
//
//    void reset()
//    {
//        x = y = -1;
//    }
//    bool operator == (const point<T>& p) const
//    {
//        if (x == p.x && y == p.y)
//            return true;
//        return false;
//    }
//    bool operator != (const point<T>& p) const
//    {
//        return !(*this == p);
//    }
//    bool close_to(const point<T>& p) const
//    {
//        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)) <= CLOSE_DISTANCE;
//    }
//    point<T> operator - (const point<T>& p) const
//    {
//        return point<T>(x - p.x, y - p.y);
//    }
//    point<T> operator + (const point<T>& p) const
//    {
//        point<T> new_p = point<T>(x + p.x, y + p.y);
//        return new_p;
//    }
//    point<T> operator += (const point<T>& p) const
//    {
//        point<T> new_p = point<T>(x + p.x, y + p.y);
//        return new_p;
//    }
//    point<T> operator -= (const point<T>& p) const
//    {
//        point<T> new_p = *this - p;
//        return new_p;
//    }
//};
//
//template<typename T>
//using points_t = std::vector<point<T>>;
//
//struct pix_info_t {
//    uint figure_id = 0;
//    uint line_id = 0;
//    uint point_id = 0;
//};



