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

// adds the ball to the vector of balls on the cell they are on based on their position 
void Grid::addBall(Ball* ball)
{
	Cell* cell = getCell(ball->position);
	
	cell->balls.push_back(ball);     

	ball->ownerCell = cell;

	ball->cellVectorIndex = cell->balls.size() - 1;
}
// adds the ball to the vector of balls based on a particular cell
void Grid::addBall(Ball* ball, Cell* cell)
{
	cell->balls.push_back(ball);

	ball->ownerCell = cell;

	ball->cellVectorIndex = cell->balls.size() - 1;
}
///get the cell 
Cell* Grid::getCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;

	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;
	// get the position of a 1d array as if it was a 2d array(better cache-coherency)
	//each row represents a data of columns scattered around no the heap which does 
	//not make the 2D array contiguous
	return &m_cells[y * m_numXCells + x];
}

//gets the cell based on the balls position
Cell* Grid::getCell(const glm::vec2 pos)
{
	
	
	int CellX = (int)(pos.x / m_cellSize);
	int CellY = (int)(pos.y / m_cellSize);


	return getCell(CellX, CellY);
}

void Grid::removeBallFromCell(Ball* ball)
{
	//just a shortcut to the balls vector of the cell that the ball we pass belongs to
	std::vector<Ball*>& balls = ball->ownerCell->balls;
	//overriding the position of the ball we pass in in the vector with the back element of the vector 
	balls[ball->cellVectorIndex] = balls.back();
	// popping the original back element
	balls.pop_back();
	//update the new element with the index of the previous element that we passed in
	if (ball->cellVectorIndex < balls.size()) {
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}
	//set the index of ball to -1
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;
}

