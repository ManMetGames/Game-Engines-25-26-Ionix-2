#include "NavMef.h"
#include "Architecture/Application.h"
//#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
	/*NavMef NavMef::GetNavMef() {
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
	void NavMef::Load(const std::vector<b2Vec2>& corners, const std::vector<int>& indices) {
		m_corners = corners;
		const int fourCount = indices.size() / 4;
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
        if (startCell < 0 || goalCell < 0 ||
            startCell >= m_cells.size() || goalCell >= m_cells.size())
        {
            return {};
        }

        const int cellCount = m_cells.size();
        std::vector<Node> nodes(cellCount);
        // initialise nodes 
        for (int i; i < cellCount; i++) {
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
                break;
            }
            //find adjacencys of cell
            for (int adjacencyIndex : m_cells[currentIndex].neighbors) {
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
            //think I need to reconstruct path here not sure
            // --- Reconstruct the path ---
            std::vector<int> path;
            int current = goalCell;

            if (nodes[current].previousCell == -1) {
                return {};
            }

            while (current != -1) {
                path.push_back(current);
                current = nodes[current].previousCell;
            }

            std::reverse(path.begin(), path.end());
            return path;

        }


    }

	//FysicsManager::~FysicsManager()
	//{
	//	// helpers
	//	delete shapes;
	//	delete force;

	//	// cleans up all bodies in the map
	//	bodyEntityMap.clear();

	//	// (this also destroys all bodies/fixtures/joints)
	//	delete world;
	//}
}