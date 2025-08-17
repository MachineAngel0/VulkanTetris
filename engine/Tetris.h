
#ifndef TETRIS_H
#define TETRIS_H
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <bits/fs_fwd.h>
#include <glm/glm.hpp>

#include "clock.h"
#include "Mesh.h"


//I need to be able to render multiple squares
//and put them at specific spots on the screen




constexpr int GRID_ROW = 12; //2 rows are the boundaries
constexpr int GRID_COLUMN = 22; //1 columns are the boundaries
constexpr float BLOCK_SCALE = 0.041f;
constexpr float CELL_SIZE = 0.085f;
constexpr float XOFFSET = -0.4f;
constexpr float YOFFSET = -0.85f;

constexpr float SPAWN_CENTER_OFFSET = 5;


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

enum Tetromino_Type
{
    I,
    O,
    T,
    S,
    Z,
    J,
    L,
    COUNT,
};

enum COLOR: int8_t
{
    WHITE,
    BLUE_LIGHT,
    YELLOW,
    PURPLE,
    GREEN,
    RED,
    ORANGE,
    BLUE_DARK,
    GREY,
};


struct Tetris_Grid
{
    int column;
    int row;
    //tetromino type represent the color of the grid
    COLOR grid_color [GRID_ROW][GRID_COLUMN];
};


static std::unordered_map<COLOR, glm::vec3> color_look_up_table{
    {WHITE,glm::vec3{1.0, 1.0, 1.0}},
    {BLUE_LIGHT,glm::vec3{0.0, 1.0, 1.0}},
    {YELLOW, glm::vec3{1.0,1.0,0.0}},
    {PURPLE, glm::vec3{0.5,0.0,1.0}},
    {GREEN,glm::vec3{0.0,1.0,0.0}},
    {RED, glm::vec3{1.0,0.0,0.0}},
    {ORANGE,glm::vec3{1.0,0.5,0.0}},
    {BLUE_DARK,glm::vec3{0.0,0.0,1.0}},
    {GREY,glm::vec3{0.5, 0.5, 0.5}},
};

static std::unordered_map<Tetromino_Type, COLOR> TETROMINO_COLOR_LOOKUP{
    {I, BLUE_LIGHT},
    {O, YELLOW},
    {T, PURPLE},
    {S, GREEN},
    {Z, RED},
    {J, ORANGE},
    {L, BLUE_DARK},
};

inline glm::vec3 get_color(COLOR color)
{
    return color_look_up_table[color];
}

inline void debug_print_grid(const Tetris_Grid& tetris_grid)
{
    for (auto grid : tetris_grid.grid_color)
    {
        printf("%d\n", grid);
    };

}

inline Tetris_Grid create_grid(VERTEX_DYNAMIC_INFO& vertex_info, int column, int row)
{
    Tetris_Grid new_grid{};
    new_grid.column = column;
    new_grid.row = row;

    for (int i = 0; i < GRID_ROW; i++)
    {
        for (int j = 0; j < GRID_COLUMN; j++)
        {

            float x = XOFFSET + i * CELL_SIZE;  // Start from -0.45 for 10 columns
            float y = YOFFSET + j * CELL_SIZE;   // Start from -0.9 for 20 rows

            glm::vec3 white{1.0f, 1.0f, 1.0f};
            glm::vec3 grey{0.5, 0.5, 0.5};
            glm::vec3 color = white;
            //color the edges
            if (i == 0 || i == GRID_ROW-1|| j == 0 || j == GRID_COLUMN-1)
            {
                color = grey;
                new_grid.grid_color[i][j] = GREY; //give the edge a custom color
            }
            else
            {
                new_grid.grid_color[i][j] = WHITE;
            }


            add_quad(glm::vec2{x, y}, color_look_up_table[new_grid.grid_color[i][j]],  BLOCK_SCALE, vertex_info);
        }
    }

    debug_print_grid(new_grid);

    return new_grid;
}

inline void refresh_grid(Tetris_Grid tetris_grid, VERTEX_DYNAMIC_INFO& vertex_info)
{

    vertex_info.dynamic_vertices.clear();
    vertex_info.dynamic_indices.clear();
    vertex_info.mesh_id = 0;
    vertex_info.vertex_buffer_should_update = true;



    for (int i = 0; i < GRID_ROW; i++)
    {
        for (int j = 0; j < GRID_COLUMN; j++)
        {

            float x = XOFFSET + i * CELL_SIZE;  // Start from -0.45 for 10 columns
            float y = YOFFSET + j * CELL_SIZE;   // Start from -0.9 for 20 rows

            //TODO: make get color function
            //tetris_grid.grid_color[i][j];

            add_quad(glm::vec2{x, y}, color_look_up_table[tetris_grid.grid_color[i][j]],  BLOCK_SCALE, vertex_info);
        }
    }

}





struct Tetromino
{
    //this is a vector for offsetting each group of vertices but this is very much not ideal,
    //it would be better to just store the offset into the vertices data
    std::vector<int> id;
    Tetromino_Type type;
    glm::vec3 color;
    Grid_Position grid_position; // the offset from (0,0)
    std::vector<Grid_Position> tetromino_default_position; // the position of each block relative to (0,0)
    int rotation_state = 0; //3 max

};



inline std::vector<Grid_Position> L_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 0), Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 1)};
    }
    if (position == 1)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 0)};
    }
    if (position == 2)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 1), Grid_Position(2, 2)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 2), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2)};
    };
    return {Grid_Position(0, 0)};

}

inline std::vector<Grid_Position> J_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 0), Grid_Position(2, 1)};
    }
    if (position == 1)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 2)};
    }
    if (position == 2)
    {
        return {Grid_Position(0, 1), Grid_Position(0, 2), Grid_Position(1, 1), Grid_Position(2, 1)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 0), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2)};
    };
    return {Grid_Position(0, 0)};


}
inline std::vector<Grid_Position> O_Block()
{
    return {Grid_Position(0, 0), Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1)};
}

inline std::vector<Grid_Position> S_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 0), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 1)};
    }
    if (position == 1)
    {
        return {Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 0), Grid_Position(2, 1)};
    } if (position == 2)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 2)};
    };
    if (position == 3)
    {
        return {Grid_Position(0, 1), Grid_Position(0, 2), Grid_Position(1, 0), Grid_Position(1, 1)};
    }
    return {Grid_Position(0, 0)};

}



inline std::vector<Grid_Position> T_Block(int position)
{
    if (position == 0)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 1)};
    }
    if (position == 1)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 1)};
    }
    if (position == 2)
    {
        return{Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 1)};
    }
    if (position == 3)
    {
        return {Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2)};
    };
    return {Grid_Position(0, 0)};

}
inline std::vector<Grid_Position> Z_Block(int position)
{
    if (position == 0)
    {
        return{Grid_Position(0, 2), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(2, 1)};
    }
    if (position == 1)
    {
        return {Grid_Position(0, 0), Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(1, 2)};
    }
    if (position == 2)
    {
        return{Grid_Position(0, 1), Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 0)};
    }
    if (position == 3)
    {
        return{Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(2, 1), Grid_Position(2, 2)};
    };

    return {Grid_Position(0, 0)};
}

inline std::vector<Grid_Position> I_Block(int position)
{
    if (position == 0)
    {
        return{Grid_Position(0, 1), Grid_Position(1, 1), Grid_Position(2, 1), Grid_Position(3, 1)};
    }
    if (position == 1)
    {
        return{Grid_Position(2, 0), Grid_Position(2, 1), Grid_Position(2, 2), Grid_Position(2, 3)};
    }
    if (position == 2)
    {
        return{Grid_Position(0, 2), Grid_Position(1, 2), Grid_Position(2, 2), Grid_Position(3, 2)};
    }
    if (position == 3)
    {
        return {Grid_Position(1, 0), Grid_Position(1, 1), Grid_Position(1, 2), Grid_Position(1, 3)}  ;
    };

    return {Grid_Position(0, 0)};
}


inline void spawn_block(Tetromino& Tetromino, glm::vec3 color, VERTEX_DYNAMIC_INFO& vertex_info)
{
    Tetromino.grid_position.x += SPAWN_CENTER_OFFSET; // give it a 5 offset
    for (auto default_position : Tetromino.tetromino_default_position)
    {
        //Tetromino.id.emplace_back(add_quad(glm::vec2{XOFFSET + (SPAWN_CENTER_OFFSET + (CELL_SIZE*grid_position.x)), YOFFSET + (CELL_SIZE*grid_position.y)}, color, BLOCK_SCALE, vertex_info));
        Tetromino.id.emplace_back(add_quad(glm::vec2{XOFFSET + ((default_position.x + Tetromino.grid_position.x) * CELL_SIZE), YOFFSET + (CELL_SIZE * (default_position.y + Tetromino.grid_position.y))}, color, BLOCK_SCALE, vertex_info));
    }
}

inline Tetromino create_new_tetromino(VERTEX_DYNAMIC_INFO& vertex_info, Tetromino_Type tetromino_type)
{
    Tetromino new_block{};
    switch (tetromino_type)
    {
        case I:
            new_block.tetromino_default_position = I_Block(0);
            break;
        case O:
            new_block.tetromino_default_position = O_Block();
            break;
        case T:
            new_block.tetromino_default_position = T_Block(0);
            break;
        case S:
            new_block.tetromino_default_position = S_Block(0);
            break;
        case Z:
            new_block.tetromino_default_position = Z_Block(0);
            break;
        case J:
            new_block.tetromino_default_position = J_Block(0);
            break;
        case L:
            new_block.tetromino_default_position = L_Block(0);
            break;
        default: break;
    }

    new_block.color =  color_look_up_table[TETROMINO_COLOR_LOOKUP[tetromino_type]];
    new_block.type = tetromino_type;
    spawn_block(new_block, new_block.color, vertex_info);

    return new_block;
}


inline void tetromino_update(Tetromino& tetromino, VERTEX_DYNAMIC_INFO& vertex_info)
{

    for (int i = 0; i < tetromino.id.size(); i++)
    {

        Grid_Position position = tetromino.grid_position;
        Grid_Position default_position = tetromino.tetromino_default_position[i];

        int id = tetromino.id[i] * 4;

        vertex_info.dynamic_vertices[id].pos.x =  XOFFSET + ((position.x + default_position.x) * CELL_SIZE) - BLOCK_SCALE;
        vertex_info.dynamic_vertices[id].pos.y = YOFFSET + ((position.y+ default_position.y) * CELL_SIZE) - BLOCK_SCALE;

        vertex_info.dynamic_vertices[id+1].pos.x =  XOFFSET + ((position.x + default_position.x) * CELL_SIZE) + BLOCK_SCALE;
        vertex_info.dynamic_vertices[id+1].pos.y = YOFFSET + ((position.y+ default_position.y) * CELL_SIZE) - BLOCK_SCALE;

        vertex_info.dynamic_vertices[id+2].pos.x =  XOFFSET + ((position.x + default_position.x) * CELL_SIZE)+ BLOCK_SCALE;
        vertex_info.dynamic_vertices[id+2].pos.y = YOFFSET + ((position.y+ default_position.y) * CELL_SIZE) + BLOCK_SCALE;

        vertex_info.dynamic_vertices[id+3].pos.x =  XOFFSET + ((position.x + default_position.x) * CELL_SIZE) - BLOCK_SCALE;
        vertex_info.dynamic_vertices[id+3].pos.y = YOFFSET + ((position.y+ default_position.y) * CELL_SIZE) + BLOCK_SCALE;



        /*
        vertex_info.dynamic_vertices[id].pos.x =  XOFFSET + (position.x * CELL_SIZE) - BLOCK_SCALE;
        vertex_info.dynamic_vertices[id].pos.y = YOFFSET + (position.y * CELL_SIZE) - BLOCK_SCALE;

        vertex_info.dynamic_vertices[id+1].pos.x =  XOFFSET + (position.x * CELL_SIZE) + BLOCK_SCALE;
        vertex_info.dynamic_vertices[id+1].pos.y = YOFFSET + (position.y * CELL_SIZE) - BLOCK_SCALE;

        vertex_info.dynamic_vertices[id+2].pos.x =  XOFFSET + (position.x * CELL_SIZE) + BLOCK_SCALE;
        vertex_info.dynamic_vertices[id+2].pos.y = YOFFSET + (position.y * CELL_SIZE) + BLOCK_SCALE;

        vertex_info.dynamic_vertices[id+3].pos.x =  XOFFSET + (position.x * CELL_SIZE) - BLOCK_SCALE;
        vertex_info.dynamic_vertices[id+3].pos.y = YOFFSET + (position.y * CELL_SIZE) + BLOCK_SCALE;*/

    }

    vertex_info.vertex_buffer_should_update = true;

}


inline void rotate_block(Tetromino& tetromino, VERTEX_DYNAMIC_INFO& vertex_info)
{
    //testing using Z
    tetromino.rotation_state++;
    if (tetromino.rotation_state > 3) tetromino.rotation_state = 0;

    std::vector<Grid_Position> rotation;

    switch (tetromino.type)
    {
        case I:
            rotation = I_Block(tetromino.rotation_state);
            break;
        case O:
            rotation = O_Block();
            break;
        case T:
            rotation = T_Block(tetromino.rotation_state);
            break;
        case S:
            rotation = S_Block(tetromino.rotation_state);
            break;
        case Z:
            rotation = Z_Block(tetromino.rotation_state);
            break;
        case J:
            rotation = J_Block(tetromino.rotation_state);
            break;
        case L:
            rotation = L_Block(tetromino.rotation_state);
            break;
    }

    //check if we can rotate at all
    bool allowed_to_move = true;
    for (auto rotation_positions : rotation)
    {
        int y_check = rotation_positions.y + tetromino.grid_position.y;
        int x_check = rotation_positions.x + tetromino.grid_position.x;
        if (y_check > GRID_COLUMN - 3)
        {
            allowed_to_move = false;
            break;
        }

        if (x_check > GRID_ROW - 2 || x_check < 1)
        {
            allowed_to_move = false;
            break;
        }

    }
    //early exit if rotation hits a wall
    if (!allowed_to_move) return;

    //change grid representation
    for (int i = 0; i < tetromino.tetromino_default_position.size(); i++)
    {
        //want to set new position
        tetromino.tetromino_default_position[i] = rotation[i];
    }

    tetromino_update(tetromino, vertex_info);


}



//return false means we want to spawn in a new block
inline bool move_block(Tetromino& tetromino, Direction direction, VERTEX_DYNAMIC_INFO& vertex_info)
{

    //we know that each cube is 4 vertices, and that the vertices are cube only
    bool allowed_to_move = true;

    switch (direction)
    {
        case UP:
            //the can rotate check if inside the function, unlike the other movements
            rotate_block(tetromino, vertex_info);
            break;
        case DOWN:
            for (auto tetromino_default_position : tetromino.tetromino_default_position)
            {
                if ( tetromino_default_position.y + tetromino.grid_position.y > GRID_COLUMN - 3)
                {
                    allowed_to_move = false;
                    break;
                }
            }

            if (allowed_to_move)
            {
                tetromino.grid_position.y++;
            }
            else
            {
                // if this is ever false, then we want to return false, to spawn in a new block
                return false;
            }



            tetromino_update(tetromino, vertex_info);
            break;
        case RIGHT:


            for (auto tetromino_default_position : tetromino.tetromino_default_position)
            {
                if ( tetromino_default_position.x + tetromino.grid_position.x > GRID_ROW - 3)
                {
                    allowed_to_move = false;
                    break;
                }
            }

            if (allowed_to_move)
            {
                tetromino.grid_position.x++;
            }

            //tetromino.grid_position.x++;
            tetromino_update(tetromino, vertex_info);
            break;
        case LEFT:
            for (auto tetromino_default_position : tetromino.tetromino_default_position)
            {
                if ( tetromino_default_position.x + tetromino.grid_position.x < 2)
                {
                    allowed_to_move = false;
                    break;
                }
            }
            if (allowed_to_move)
            {
                tetromino.grid_position.x--;
            }


            tetromino_update(tetromino, vertex_info);
            break;
    }

    vertex_info.vertex_buffer_should_update = true;
    return true;
}


inline void update_grid_representation(Tetris_Grid& tetris_grid, Tetromino& current_tetromino, VERTEX_DYNAMIC_INFO& vertex_info)
{
    current_tetromino.grid_position.x = 0;


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
inline bool should_move_block_time_trigger(Tetris_Clock& tetris_clock)
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

inline Tetromino pick_new_tetromino(VERTEX_DYNAMIC_INFO& vertex_info)
{
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    auto type = static_cast<Tetromino_Type>((rand() %  Tetromino_Type::COUNT)); // get a random num
    return create_new_tetromino(vertex_info, type);
}


inline void update_game(Game_State* game_state, VERTEX_DYNAMIC_INFO& vertex_dynamic_info, float dt)
{
    tetris_clock_update(game_state->tetris_clock, dt);
    if (should_move_block_time_trigger(game_state->tetris_clock))
    {

        printf("move_block\n");

        bool spawn_new_block = move_block(game_state->current_tetromino, DOWN, vertex_dynamic_info);

        COLOR Grid_Color = TETROMINO_COLOR_LOOKUP[game_state->current_tetromino.type];

        if (!spawn_new_block)
        {
            //set the grid representation
            for (auto tetromino_default_position : game_state->current_tetromino.tetromino_default_position)
            {
                game_state->tetris_grid.grid_color
                [tetromino_default_position.x + game_state->current_tetromino.grid_position.x][tetromino_default_position.y + game_state->current_tetromino.grid_position.y]
                = Grid_Color;
            }

            refresh_grid(game_state->tetris_grid, vertex_dynamic_info);

            //set new tetromino
            game_state->current_tetromino = pick_new_tetromino(vertex_dynamic_info);
        }
    }
};



#endif //TETRIS_H
