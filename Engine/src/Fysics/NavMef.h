#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <vector>
#include <array>
#include <iostream>

struct Cell {
    std::array<int, 3> verts;
    std::vector<int> neighbors;
};

class NavMef {
private:
    std::vector<b2Vec2> m_vertices;
    std::vector<Cell> m_cells;

    bool ShareEdge(const Cell& a, const Cell& b) {
        int shared = 0;
        for (int va : a.verts) {
            for (int vb : b.verts) {
                if (va == vb) {
                    shared++;
                }
            }
                
        }
            

        // if 2 vertices are shared between the triangles, they touch with an edge and are adjacent
        return shared >= 2;
    }


public:
    // load vertices and indices
    void Load(const std::vector<b2Vec2>& vertices, const std::vector<int>& indices) {
        m_vertices = vertices;
        const int threeCount = indices.size() / 3;
        m_cells.resize(threeCount);

        // cells
        for (int i = 0; i < threeCount; i++) {
            m_cells[i].verts = { indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2] };
        }

        // check adjacent triangles with their edges
        for (int a = 0; a < threeCount; a++) {
            for (int b = a + 1; b < threeCount; b++) {
                if (ShareEdge(m_cells[a], m_cells[b])) {
                    m_cells[a].neighbors.push_back(b);
                    m_cells[b].neighbors.push_back(a);
                }
            }
        }
    }

    const std::vector<Cell>& GetCells() const { return m_cells; }


};
