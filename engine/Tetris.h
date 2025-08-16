
#ifndef TETRIS_H
#define TETRIS_H
#include <cstdio>
#include <vector>
#include <bits/fs_fwd.h>
#include <glm/glm.hpp>

#include "clock.h"
#include "Mesh.h"


//I need to be able to render multiple squares
//and put them at specific spots on the screen

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

struct Position
{
    float x;
    float y;
};

//were using this as there are no decimal values in grids
struct Grid_Position
{
    int x;
    int y;
};

struct Tetris_Grid
{
    int column;
    int row;
    std::vector<Grid_Position> grid;
    //std::vector<int> occupied_positions;
};

enum Tetromino_Type
{
    I,
    O,
    T,
    S,
    Z,
    J,
    L,
};



constexpr int GRID_ROW = 12; //2 rows are the boundaries
constexpr int GRID_COLUMN = 22; //1 columns are the boundaries
constexpr float BLOCK_SCALE = 0.041f;
constexpr float CELL_SIZE = 0.085f;
constexpr float XOFFSET = -0.4f;
constexpr float YOFFSET = -0.85f;

constexpr int CENTER_OFFSET = 5;
constexpr float SPAWN_CENTER_OFFSET = CENTER_OFFSET * CELL_SIZE;


inline Tetris_Grid create_grid(VERTEX_DYNAMIC_INFO& vertex_info, int column, int row)
{
    Tetris_Grid new_grid{};
    new_grid.column = column;
    new_grid.row = row;
    new_grid.grid.reserve(GRID_ROW * GRID_COLUMN);

    for (int i = 0; i < GRID_ROW; i++)
    {
        for (int j = 0; j < GRID_COLUMN; j++)
        {
            new_grid.grid.emplace_back(0);


            float x = XOFFSET + i * CELL_SIZE;  // Start from -0.45 for 10 columns
            float y = YOFFSET + j * CELL_SIZE;   // Start from -0.9 for 20 rows

            glm::vec3 white{1.0f, 1.0f, 1.0f};
            glm::vec3 grey{0.5, 0.5, 0.5};
            glm::vec3 color = white;
            //color the edges
            if (i == 0 || i == GRID_ROW-1|| j == 0 || j == GRID_COLUMN-1)
            {
                color = grey;
            }

            add_quad(glm::vec2{x, y}, glm::vec3{color},  BLOCK_SCALE, vertex_info);
            printf("X: %f, Y:%f\n", x, y);
        }
    }

    return new_grid;
}


struct Tetromino
{
    //this is a vector for offsetting each group of vertices but this is very much not ideal,
    //it would be better to just store the offset into the vertices data
    std::vector<int> id;
    glm::vec3 color = {1.0,0.0,0.0}; //red for now
    std::vector<Grid_Position> grid_position; // the position of each item on the grid, which should get updated, each move
    int rotation_state = 0; //3 max
    Tetromino_Type type;
};


inline void spawn_block(Tetromino& Tetromino, glm::vec3 color, VERTEX_DYNAMIC_INFO& vertex_info)
{

    for (auto grid_position : Tetromino.grid_position)
    {
        Tetromino.id.emplace_back(add_quad(glm::vec2{XOFFSET + (SPAWN_CENTER_OFFSET + (CELL_SIZE*grid_position.x)), YOFFSET + (CELL_SIZE*grid_position.y)}, color, BLOCK_SCALE, vertex_info));
    }
}

inline Tetromino create_block(VERTEX_DYNAMIC_INFO& vertex_info, Tetromino_Type tetromino_type)
{
    Tetromino new_block{};
    switch (tetromino_type)
    {
        case I:
            new_block.color = glm::vec3{0.0,1.0,1.0}; // LIGHTER BLUE
            new_block.grid_position = {Grid_Position(0, 0), Grid_Position(1, 0),Grid_Position(2, 0),Grid_Position(3, 0)};
            break;
        case O:
            new_block.color = glm::vec3{1.0,1.0,0.0}; //Yellow
            new_block.grid_position = {Grid_Position(0, 0), Grid_Position(0, 1),Grid_Position(1, 1),Grid_Position(1, 0)};
            break;
        case T:
            new_block.color = glm::vec3{0.5,0.0,1.0}; // PURPLE
            new_block.grid_position = {Grid_Position(0, 1), Grid_Position(1, 1),Grid_Position(2, 1),Grid_Position(1, 0)};
            break;
        case S:
            new_block.color = glm::vec3{0.0,1.0,0.0}; //GREEN
            new_block.grid_position = {Grid_Position(0, 1), Grid_Position(1, 0),Grid_Position(1, 1),Grid_Position(2, 0)};
            break;
        case Z:
            new_block.color = glm::vec3{1.0,0.0,0.0}; //RED
            new_block.grid_position = {Grid_Position(0, 0), Grid_Position(1, 0),Grid_Position(1, 1),Grid_Position(2, 1)};
            break;
        case J:
            new_block.color = glm::vec3{1.0,0.5,0.0}; // ORANGE
            new_block.grid_position = {Grid_Position(3, 0), Grid_Position(0, 1),Grid_Position(1, 1),Grid_Position(2, 1), Grid_Position(3, 1)};
            break;
        case L:
            new_block.color = glm::vec3{0.0,0.0,1.0}; // DARKER BLUE
            new_block.grid_position = {Grid_Position(0, 0), Grid_Position(0, 1),Grid_Position(1, 1),Grid_Position(2, 1), Grid_Position(3, 1)};
            break;
    }

    new_block.type = tetromino_type;
    spawn_block(new_block, new_block.color, vertex_info);


    return new_block;
}





inline std::vector<Grid_Position> L_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 2), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2)};
    }
    if (position == 1)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 1), Grid_Position(2, 2)};
    }
    if (position == 2)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 0)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 0), Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 1)};
    };
}

inline std::vector<Grid_Position> J_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 0), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2)};
    }
    if (position == 1)
    {
        return {Grid_Position(0, 1), Grid_Position(0, 2), Grid_Position(1, 1), Grid_Position(2, 1)};
    }
    if (position == 2)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 2)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 0), Grid_Position(2, 1)};
    };

}
inline std::vector<Grid_Position> O_Block(int position)
{
    return {Grid_Position(0, 0), Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1)};
}

inline std::vector<Grid_Position> S_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 1), Grid_Position(0, 2), Grid_Position(1, 0), Grid_Position(1, 1)};
    }
    if (position == 1)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 2)};
    }
    if (position == 2)
    {
        return {Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 0), Grid_Position(2, 1)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 0), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 1)};
    };

}
inline std::vector<Grid_Position> T_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2)};
    }
    if (position == 1)
    {
        return{Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 1)};
    }
    if (position == 2)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 1)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 1)};
    };
}
inline std::vector<Grid_Position> Z_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 0), Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(1, 2)};
    }
    if (position == 1)
    {
        return{Grid_Position(0, 2), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 1)};
    }
    if (position == 2)
    {
        return{Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 1), Grid_Position(2, 2)};
    }
    if (position == 3)
    {
        return{Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 0)};
    };
}


inline void rotate_block(Tetromino& tetromino, VERTEX_DYNAMIC_INFO& vertex_info)
{
    //testing using Z
    tetromino.rotation_state++;
    if (tetromino.rotation_state > 3) tetromino.rotation_state = 0;

    //change grid representation
    for (auto& grid_position : tetromino.grid_position)
    {
        for (auto z_block : Z_Block(tetromino.rotation_state))
        {
            //want to set new position
            grid_position.x -= z_block.x;
            grid_position.y -= z_block.y;
        }
    }

    //update position
    for (auto& grid_position : tetromino.grid_position)
    {
        for (int id : tetromino.id)
        {
            for (int i = id*4; i < (id*4)+4; i++)
            {
                vertex_info.dynamic_vertices[i].pos.x += CELL_SIZE;
                vertex_info.dynamic_vertices[i].pos.y += CELL_SIZE;
            }
        }
    }

}




inline void move_block(Tetromino& tetromino, Direction direction, VERTEX_DYNAMIC_INFO& vertex_info)
{

    //we know that each cube is 4 vertices, and that the vertices are cube only
    switch (direction)
    {
        case UP:
            rotate_block(tetromino, vertex_info);
            break;
        case DOWN:
            for (int id : tetromino.id)
            {
                for (int i = id*4; i < (id*4)+4; i++)
                {
                    vertex_info.dynamic_vertices[i].pos.y += CELL_SIZE;
                }
            }
            break;
        case RIGHT:
            for (int id : tetromino.id)
            {
                for (int i = id*4; i < (id*4)+4; i++)
                {
                    vertex_info.dynamic_vertices[i].pos.x += CELL_SIZE;
                }
            }
            break;
        case LEFT:
            for (int id : tetromino.id)
            {
                for (int i = id*4; i < (id*4)+4; i++)
                {
                    vertex_info.dynamic_vertices[i].pos.x -= CELL_SIZE;
                }
            }
            break;
    }

    vertex_info.vertex_buffer_should_update = true;

}


struct Tetris_Clock
{
    float accumulated_time; //adding time
    float move_block_trigger = SECONDS(5.0f); //when we want to move the block down // in seconds
};

inline void tetris_clock_init(Tetris_Clock& tetris_clock)
{
    tetris_clock.accumulated_time =  tetris_clock.move_block_trigger;
};
inline void tetris_clock_update(Tetris_Clock& tetris_clock, float delta_time)
{
    tetris_clock.accumulated_time -= 1.0f * delta_time;
    //printf("%f\n", tetris_clock.accumulated_time);
};
inline bool should_move_block(Tetris_Clock& tetris_clock)
{
    if (tetris_clock.accumulated_time <= 0)
    {
        tetris_clock.accumulated_time = tetris_clock.move_block_trigger;
        return true;
    }
    return false;
};

struct Game_State
{
    Tetris_Clock tetris_clock;
    Tetris_Grid tetris_grid;
    Tetromino current_tetromino;
    float move_timer = 5.0;
    float move_trigger = 0.0;
};


inline void update_game(Game_State* game_state, VERTEX_DYNAMIC_INFO& vertex_dynamic_info, float dt)
{
    tetris_clock_update(game_state->tetris_clock, dt);
    if (should_move_block(game_state->tetris_clock))
    {
        printf("move_block\n");
        move_block(game_state->current_tetromino, DOWN, vertex_dynamic_info);

    }
};



#endif //TETRIS_H
