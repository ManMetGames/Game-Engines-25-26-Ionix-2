#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <vector>
#include <array>
#include <iostream>

struct Cell {
    std::array<int, 4> corns; // index of cell
    std::vector<int> neighbors;
    //float heuristicValue;
};
struct Node {
    int cellIndex;
    float h;
    float g;
    float f;
    int previousCell;

};

//struct GridCell {
//    bool canPass;
//    Cell cell;
//    float cost;
//
//};

class NavMef {
private:
    std::vector<b2Vec2> m_corners; // corner coords
    std::vector<Cell> m_cells; // cell

    bool ShareEdge(const Cell& a, const Cell& b) {
        int shared = 0;
        for (int va : a.corns) {
            for (int vb : b.corns) {
                if (va == vb) {
                    shared++;
                }
            }
                
        }
            

        // if 2 vertices are shared between the rectangles, they touch with an edge and are adjacent
        return shared >= 2;
    }


public:
    // load vertices and indices
    void Load(const std::vector<b2Vec2>& corners, const std::vector<int>& indices) {
        m_corners = corners;
        const int fourCount = indices.size() / 4;
        m_cells.resize(fourCount);

        // cells
        for (int i = 0; i < fourCount; i++) {
            m_cells[i].corns = { indices[i * 4], indices[i * 4 + 1], indices[i * 4 + 2], indices[i*4+2]}; // the corner values
            //m_cells[i].heuristicValue = CalculateHeuristic(m_cells);
        }

        // check adjacent rectangles with their edges
        for (int a = 0; a < fourCount; a++) {
            for (int b = a + 1; b < fourCount; b++) {
                if (ShareEdge(m_cells[a], m_cells[b])) {
                    m_cells[a].neighbors.push_back(b);
                    m_cells[b].neighbors.push_back(a);
                }
            }
        }
    }

    const std::vector<Cell>& GetCells() const { return m_cells; }

    b2Vec2 GetCellCentre(const Cell& cell) const {
        //finds the centre of the cell (rectangle)  from the 3 verticies;
        b2Vec2 centre = { 0,0 };
        for (int cornIndex : cell.corns) {
            centre += m_corners[cornIndex];
            
        }
        centre *= 0.25f;
        return centre;
    }

    float CalculateHeuristic(int cellIndex, int goalIndex) const{
        //takes current cell index then the goal and calculates the h value by calculating the distance between them
        b2Vec2 cellCentre = GetCellCentre(m_cells[cellIndex]);
        b2Vec2 goalCentre = GetCellCentre(m_cells[goalIndex]);
        return b2Distance(cellCentre, goalCentre);
    }
    // A* incoming crazy func gotta get wild
    std::vector<int>FindPath(int startCell, int goalCell) {
        //get values from node calculate g using b2distance, calc h for the each cell using heuristic function, calc f from both;
    }

    //also Olesya gonna make a funnel that is fun *for* all!


};
