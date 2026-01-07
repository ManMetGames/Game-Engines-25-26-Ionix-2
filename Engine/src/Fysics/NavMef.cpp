#include "NavMef.h"

#include "FysicsManager.h"
#include "Architecture/Application.h"
//#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
    /*
	NavMef NavMef::GetNavMef() {
		return Application::Get().layerNavigation->GetNavMef();
	}*/

    bool NavMef::ShareEdge(const Cell& a, const Cell& b) {
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
    void NavMef::BuildGrid(b2Vec2 origin, b2Vec2 size, float cellSize) {
        std::vector<b2Vec2> corners;
        std::vector<int> indices;

        int cols = static_cast<int>(size.x / cellSize);
        int rows = static_cast<int>(size.y / cellSize);

        //calc corners
        for (int y = 0; y <= rows; y++)
        {
            for (int x = 0; x <= cols; x++)
            {
                corners.push_back({
                    origin.x + x * cellSize,
                    origin.y + y * cellSize
                    });
            }
        }

        auto CornerIndex = [&](int x, int y)
            {
                return y * (cols + 1) + x;
            };

        //calc cell indices
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                int bl = CornerIndex(x, y);
                int br = CornerIndex(x + 1, y);
                int tl = CornerIndex(x, y + 1);
                int tr = CornerIndex(x + 1, y + 1);

                indices.push_back(bl);
                indices.push_back(br);
                indices.push_back(tr);
                indices.push_back(tl);
            }
        }

        //load mesh
        Load(corners, indices);
    }
	void NavMef::Load(const std::vector<b2Vec2>& corners, const std::vector<int>& indices) {
		m_corners = corners;
		const int fourCount = indices.size() / 4;
        m_cells.clear();
		m_cells.resize(fourCount);

		// cells
		for (int i = 0; i < fourCount; i++) {
			//m_cells[i].corns = { indices[i * 4], indices[i * 4 + 1], indices[i * 4 + 2], indices[i*4+2]}; // the corner values (index's)
			m_cells[i].corns = { indices[i * 4], indices[i * 4 + 1], indices[i * 4 + 2], indices[i * 4 + 3] }; // fixed

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

        m_blockedCells.clear();
        m_blockedCells.resize(fourCount, false);

	    std::cout << "NavMef Loaded" << endl;
	}
	const std::vector<Cell>& NavMef::GetCells() const { return m_cells; }
	b2Vec2 NavMef::GetCellCentre(const Cell& cell) const {
		//finds the centre of the cell (rectangle)  from the 3 verticies;
		b2Vec2 centre = { 0,0 };
		for (int cornIndex : cell.corns) { // cycles through the corners in the cell
			centre += m_corners[cornIndex]; // gets the position of each corner from the cornerindex in the cell

		}
		centre *= 0.25f; // uses box2d syntax that lets multiply vector
		return centre;
	}
	float NavMef::CalculateHeuristic(int cellIndex, int goalIndex) const {
		//takes current cell index then the goal and calculates the h value by calculating the distance between them
		b2Vec2 cellCentre = GetCellCentre(m_cells[cellIndex]);
		b2Vec2 goalCentre = GetCellCentre(m_cells[goalIndex]);
		return b2Distance(cellCentre, goalCentre);
	}

    
    std::vector<int>NavMef::FindPath(int startCell, int goalCell) {
        //get values from node calculate g using b2distance, calc h for the each cell using heuristic function, calc f from both;
        //if (startCell < 0 || goalCell < 0 || m_cells.size() >= startCell || m_cells.size() >= goalCell) {
        //    return{}; // wrong size check
        //}
        bool pathFound = false;
        
        if (startCell == goalCell)
        {
            return{startCell};
        }
        
        if (startCell < 0 || goalCell < 0 ||
            startCell >= m_cells.size() || goalCell >= m_cells.size())
        {
            return {};
        }

        if (IsCellBlocked(startCell) || IsCellBlocked(goalCell))
        {
            std::cout << "Cell is blocked" << std::endl;
            return {};
        }

        const int cellCount = m_cells.size();
        std::vector<Node> nodes(cellCount);
        // initialise nodes 
        for (int i = 0; i < cellCount; i++) {
            nodes[i].cellIndex = i;
            nodes[i].h = CalculateHeuristic(i, goalCell);

            nodes[i].g = std::numeric_limits<float>::infinity();
            nodes[i].f = std::numeric_limits<float>::infinity();
            nodes[i].previousCell = -1;
        }
        nodes[startCell].g = 0;
        nodes[startCell].f = nodes[startCell].h;
        std::priority_queue<Node, std::vector<Node>, CompareNode> openList;
        openList.push(nodes[startCell]);
        //closed list 
        std::vector<bool> closeList(cellCount, false);

        //while loop to calc distance between nodes
        while (!openList.empty()) {
            Node currentNode = openList.top();
            openList.pop();
            int currentIndex = currentNode.cellIndex;
            if (closeList[currentIndex] == true) {
                continue;
            }
            closeList[currentIndex] = true;
            if (currentIndex == goalCell) {
                //found path
                pathFound = true;
                break;
            }
            //find adjacencys of cell
            for (int adjacencyIndex : m_cells[currentIndex].neighbors) {
                //added adjacence check for obstactle handling
                if (IsCellBlocked(adjacencyIndex))
                {
                    continue;
                }
                
                if (closeList[adjacencyIndex]) {
                    continue;
                }
                // calc distance
                float distanceBetween = b2Distance(GetCellCentre(m_cells[currentIndex]), GetCellCentre(m_cells[adjacencyIndex]));
                float checkG = nodes[currentIndex].g + distanceBetween;
                //if adjacent closer record it
                if (checkG < nodes[adjacencyIndex].g) {
                    nodes[adjacencyIndex].previousCell = currentIndex;
                    nodes[adjacencyIndex].g = checkG;
                    nodes[adjacencyIndex].f = nodes[adjacencyIndex].g + nodes[adjacencyIndex].h;

                    openList.push(nodes[adjacencyIndex]);
                }
            }
            //think I need to reconstruct path here not sure, edit I didnt I needed to put it out the while loop good job me
                
        }
        std::vector<int> path;
        int current = goalCell;

        if (!pathFound) {
            return {};
        }

        while (current != -1) {
            path.push_back(current);
            current = nodes[current].previousCell;
        }

        std::reverse(path.begin(), path.end());
        return path;


    }
    int NavMef::GetPositionInMesh(b2Vec2 position)
    {
        for (int i = 0; i < m_cells.size(); i++)
        {
            
            const Cell& cell = m_cells[i];

            b2Vec2 c0 = m_corners[cell.corns[0]];
            b2Vec2 c1 = m_corners[cell.corns[1]];
            b2Vec2 c2 = m_corners[cell.corns[2]];
            b2Vec2 c3 = m_corners[cell.corns[3]];

            float minX = std::min({ c0.x, c1.x, c2.x, c3.x });
            float maxX = std::max({ c0.x, c1.x, c2.x, c3.x });
            float minY = std::min({ c0.y, c1.y, c2.y, c3.y });
            float maxY = std::max({ c0.y, c1.y, c2.y, c3.y });

            if (position.x >= minX && position.x <= maxX &&
                position.y >= minY && position.y <= maxY)
            {
                return i;
            }
        }

        return -1;
    }
    //get position function get the sprite or entity or agent maybe a shape IDK - then get its position check position in the navmesh using an in point polygon test to check if in rectangle then get the cell to use and place agent.



    //Olesya's funnel algorithm <3
    std::vector<b2Vec2> NavMef::Funnel(const std::vector<int>& cellPath)
    {
        //std::cout << "Cell Path: " << cellPath.size() << std::endl;

        // get waypoints through shared edges
        std::vector<b2Vec2> result;
        if (cellPath.size() < 2) 
        { 
            return result; 
        }

        //collect corridor edges
        std::vector<b2Vec2> left;
        std::vector<b2Vec2> right;

        
        for (int i = 0; i < cellPath.size() - 1; i++) {
            int a = cellPath[i];
            int b = cellPath[i + 1];

            const Cell& ca = m_cells[a];
            const Cell& cb = m_cells[b];

            int sharedCount = 0;

            b2Vec2 p0;
            b2Vec2 p1;

            // find two shared corners between different cells
            for (int ia = 0; ia < 4; ia++) {
                for (int ib = 0; ib < 4; ib++) {
                    if (ca.corns[ia] == cb.corns[ib]) {
                        if (sharedCount == 0) { p0 = m_corners[ca.corns[ia]]; }
                        if (sharedCount == 1) { p1 = m_corners[ca.corns[ia]]; }
                        sharedCount++;
                    }
                }
            }

            // push the edge to either left or right to sort the order
            if (sharedCount == 2) {
                if (p0.x < p1.x) {
                    left.push_back(p0);
                    right.push_back(p1);
                }
                else {

                    left.push_back(p1);
                    right.push_back(p0);
                }
            }
        }

        b2Vec2 apex = GetCellCentre(m_cells[cellPath[0]]);

        // set up  funnel apex
        int leftIndex = 0;
        int rightIndex = 0;

        b2Vec2 leftLeg = left[0] - apex;
        b2Vec2 rightLeg = right[0] - apex;

        result.push_back(apex);

        int i = 1;
        while (i < left.size()) {

            b2Vec2 newLeft = left[i] - apex;


            if (b2Cross(rightLeg, newLeft) <= 0) {
                if (b2Cross(leftLeg, newLeft) >= 0) {
                    leftLeg = newLeft;
                    leftIndex = i;
                }
                else {
                    apex = apex + leftLeg;
                    result.push_back(apex);
                    rightIndex = i;
                    rightLeg = right[rightIndex] - apex;
                    leftLeg = left[leftIndex] - apex;
                }
            }

            b2Vec2 newRight = right[i] - apex;

            if (b2Cross(newRight, leftLeg) <= 0) {

                if (b2Cross(newRight, rightLeg) >= 0) {
                    rightLeg = newRight;
                    rightIndex = i;
                }
                else {
                    apex = apex + rightLeg;
                    result.push_back(apex);

                    leftIndex = i;

                    leftLeg = left[leftIndex] - apex;
                    rightLeg = right[rightIndex] - apex;
                }
            }

            i++;
        }

        result.push_back(GetCellCentre(m_cells[cellPath.back()]));
        return result;
    }




    // obstactles implemented 
    bool NavMef::CellOverlaps(const Cell& cell, const b2Vec2& min, const b2Vec2& max) const
    {

        // compute cell corners
        b2Vec2 c0 = m_corners[cell.corns[0]];
        b2Vec2 c1 = m_corners[cell.corns[1]];
        b2Vec2 c2 = m_corners[cell.corns[2]];
        b2Vec2 c3 = m_corners[cell.corns[3]];

        float cellMinX = std::min({ c0.x, c1.x, c2.x, c3.x });
        float cellMaxX = std::max({ c0.x, c1.x, c2.x, c3.x });
        float cellMinY = std::min({ c0.y, c1.y, c2.y, c3.y });
        float cellMaxY = std::max({ c0.y, c1.y, c2.y, c3.y });

        // test the overlap
        if (cellMaxX < min.x || cellMinX > max.x)
        {

            return false;
        }

        if (cellMaxY < min.y || cellMinY > max.y)
        {

            return false;
        }

        return true;
    }

    void NavMef::ClearObstacles()
    {
        for (int i = 0; i < m_blockedCells.size(); i++)
        {

            m_blockedCells[i] = false;
        }
    }

    void NavMef::AddObstacle(const b2Vec2& min, const b2Vec2& max)
    {
        
        for (int i = 0; i < m_cells.size(); i++)
        {

            if (CellOverlaps(m_cells[i], min, max))
            {
                m_blockedCells[i] = true;
            }

        }
    }

    bool NavMef::IsCellBlocked(int cellIndex) const
    {
        if (cellIndex < 0 || cellIndex >= m_blockedCells.size())
        {

            return true;
        }

        return m_blockedCells[cellIndex];
    }

    void NavMef::DrawGrid(float x, float y, float width, float height, float cellSize, SDL_Color color)
    {
        SDL_Renderer* renderer = Application::Get().GetWindow().GetSdlRenderer();

        // Draw filled rectangles for blocked cells first
        for (int i = 0; i < m_cells.size(); ++i)
        {
            if (!m_blockedCells.empty() && m_blockedCells[i])
            {
                const Cell& cell = m_cells[i];

                // get the 4 corners
                b2Vec2 c0 = m_corners[cell.corns[0]];
                b2Vec2 c1 = m_corners[cell.corns[1]];
                b2Vec2 c2 = m_corners[cell.corns[2]];
                b2Vec2 c3 = m_corners[cell.corns[3]];

                // get the bounding rect
                float minX = std::min({ c0.x, c1.x, c2.x, c3.x });
                float maxX = std::max({ c0.x, c1.x, c2.x, c3.x });
                float minY = std::min({ c0.y, c1.y, c2.y, c3.y });
                float maxY = std::max({ c0.y, c1.y, c2.y, c3.y });

                SDL_FRect rect = { minX * 100, minY * 100, maxX * 100 - minX * 100, maxY * 100 - minY * 100 };

                // fill blocked cell in red (semi-transparent)
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
                SDL_RenderFillRectF(renderer, &rect);
            }
        }

        // Draw the base grid lines over everything
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        // vertical lines
        for (float i = 0; i <= width; i += cellSize)
        {
            float xPos = x + i;
            SDL_RenderDrawLineF(renderer, xPos, y, xPos, y + height);
        }

        // horizontal lines
        for (float j = 0; j <= height; j += cellSize)
        {
            float yPos = y + j;
            SDL_RenderDrawLineF(renderer, x, yPos, x + width, yPos);
        }
    }



}