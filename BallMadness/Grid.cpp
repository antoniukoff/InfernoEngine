#include "Grid.h"

Grid::Grid(int width, int height, int cellSize): m_width(width), 
												m_height(height),
												m_cellSize(cellSize)
{
	m_numXCells = ceil((float)m_width / cellSize);
	m_numYCells = ceil((float)m_height / cellSize);

	//allocating cells
	const int BALLS_TO_RESERVE = 20;
	m_cells.resize(m_numYCells * m_numXCells);
	for (int i = 0; i < m_cells.size(); i++) {
		m_cells[i].balls.reserve(BALLS_TO_RESERVE);
	}
}

Grid::~Grid()
{
}

void Grid::addBall(Ball* ball)
{
	Cell* cell = getCell(ball->position);
	
	cell->balls.push_back(ball);     

	ball->ownerCell = cell;

	ball->cellVectorIndex = cell->balls.size() - 1;
}

void Grid::addBall(Ball* ball, Cell* cell)
{
	cell->balls.push_back(ball);

	ball->ownerCell = cell;

	ball->cellVectorIndex = cell->balls.size() - 1;
}

Cell* Grid::getCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells;

	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells;

	return &m_cells[y * m_numXCells + x];
}

Cell* Grid::getCell(const glm::vec2 pos)
{
	
	
	int CellX = (int)(pos.x / m_cellSize);
	int CellY = (int)(pos.y / m_cellSize);


	return getCell(CellX, CellY);
}

void Grid::removeBallFromCell(Ball* ball)
{
	std::vector<Ball*>& balls = ball->ownerCell->balls;
	//normal vector swap
	balls[ball->cellVectorIndex] = balls.back();
	balls.pop_back();
	//update vector index
	if (ball->cellVectorIndex < balls.size()) {
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}
	//set the index of ball to -1
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;
}

