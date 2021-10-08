#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include <iostream>

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}
  
  Snake(const Snake& other)
  {
      // copy constructor
      std::cout << "Snake copy constructor" << std::endl;
      direction = other.direction;
      speed = other.speed;
      size = other.size;
      alive = other.alive;
      head_x = other.head_x;
      head_y = other.head_y;
      whichFood = other.whichFood;
      body = other.body;
      growing = other.growing;
      grid_width = other.grid_width;
      grid_height = other.grid_height;
  }

  Snake &operator=(const Snake& other)
  {
      // copy assignment constructor
      std::cout << "Snake copy assignment constructor" << std::endl;
      if (this != &other)
      {
          direction = other.direction;
          speed = other.speed;
          size = other.size;
          alive = other.alive;
          head_x = other.head_x;
          head_y = other.head_y;
          whichFood = other.whichFood;
          body = other.body;
          growing = other.growing;
          grid_width = other.grid_width;
          grid_height = other.grid_height;
      }
      return *this;
  }

  Snake(Snake &&other)
  {
      // move constructor
      std::cout << "Snake move constructor" << std::endl;
      direction = other.direction;
      speed = other.speed;
      size = other.size;
      alive = other.alive;
      head_x = other.head_x;
      head_y = other.head_y;
      whichFood = other.whichFood;
      body = other.body;
      growing = other.growing;
      grid_width = other.grid_width;
      grid_height = other.grid_height;

      // reset other object to initial state
      other.direction = Direction::kUp;
      other.speed = 0.05f;
      other.size = 1;
      other.alive = true;
      other.body.clear();
      other.growing = false;
      other.whichFood = 0;
  }

  Snake &operator=(Snake&& other)
  {
      // move assignment constructor
      std::cout << "Snake move assignment constructor" << std::endl;
      if (this != &other)
      {
          direction = other.direction;
          speed = other.speed;
          size = other.size;
          alive = other.alive;
          head_x = other.head_x;
          head_y = other.head_y;
          whichFood = other.whichFood;
          body = other.body;
          growing = other.growing;
          grid_width = other.grid_width;
          grid_height = other.grid_height;

          // reset other object to initial state
          other.direction = Direction::kUp;
          other.speed = 0.05f;
          other.size = 1;
          other.alive = true;
          other.body.clear();
          other.growing = false;
          other.whichFood = 0;
      }
      return *this;

  }
  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.05f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  int whichFood {0};
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif