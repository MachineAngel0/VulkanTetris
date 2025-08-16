#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Renderer.h"

//since all offsets rn are fixed size i dont really need to keep an offset
static int mesh_id = 0;


struct Mesh {
    int id;
    //int start;
    //int offset;
    std::vector<Vertex> vertices;
};

// Modified create_quad function to properly position quads
std::vector<Vertex> create_quad(glm::vec2 pos, glm::vec3 color, float scale);

int add_quad(glm::vec2 pos, glm::vec3 color, float scale, VERTEX_DYNAMIC_INFO& vertex_info);


#endif
