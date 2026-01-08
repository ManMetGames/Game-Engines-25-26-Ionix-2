#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include <vector>
#include <array>
#include <iostream>
#include <queue>
#include <SDL_pixels.h>
#include "Architecture/ECS/Component.hpp"


namespace IonixEngine
{
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
    struct CompareNode {
        bool operator()(const Node& a, const Node& b) const {
            return a.f > b.f; // switches the priority queue to smallet f first instead of largest 
        }
    };



    //struct GridCell {
    //    bool canPass;
    //    Cell cell;
    //    float cost;
    //
    //};

    class NavMef
    {
    private:
        std::vector<b2Vec2> m_corners; // corner coords
        std::vector<Cell> m_cells; // cell
        

        bool ShareEdge(const Cell& a, const Cell& b);



        //check if the cell is blocked by an obstacle
        std::vector<bool> m_blockedCells;

        //checks if thr cell overlaps an obstacle
        bool CellOverlaps(const Cell& cell, const b2Vec2& min, const b2Vec2& max) const;


    public:
        void BuildGrid(b2Vec2 origin, b2Vec2 size, float cellSize);
        // load vertices and indices
        void Load(const std::vector<b2Vec2>& corners, const std::vector<int>& indices);

        const std::vector<Cell>& GetCells() const;

        b2Vec2 GetCellCentre(const Cell& cell) const;

        float CalculateHeuristic(int cellIndex, int goalIndex) const;
        // A* incoming crazy func gotta get wild
        std::vector<int>FindPath(int startCell, int goalCell);

        int GetPositionInMesh(b2Vec2 position);

        //also Olesya gonna make a funnel that is fun *for* all! *Yessss

        std::vector<b2Vec2> Funnel(const std::vector<int>& cellPath);




        //added obstactle handling  by Olesya
        void ClearObstacles();

        //adds an obstacle
        void AddObstacle(const b2Vec2& min, const b2Vec2& max);
        void AddObstacleFromEntity(Entity* entity, float scaleMultiplier0);

        void RebuildClearance();

        // check to see if a cell is unnavailable and blocked 
        bool IsCellBlocked(int cellIndex) const;
        void DrawGrid(float x, float y, float width, float height, float cellSize, SDL_Color color);
    };
}

