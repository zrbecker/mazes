#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <tuple>

#include "Image.hpp"

typedef std::pair<unsigned int, unsigned int> Cell;
typedef std::pair<Cell, Cell> Edge;

template <typename S, typename T>
class std::hash<std::pair<S, T>>
{
public:
    size_t operator()(Cell cell)
    {
        return (std::hash<S>{}(cell.first) * 17) ^ std::hash<T>{}(cell.second);
    }
};

class Random
{
public:
    Random()
        : m_device()
        , m_engine(m_device())
    {
    }

    Cell randomCell(unsigned int size)
    {
        std::uniform_int_distribution<unsigned int> dist(0, size - 1);
        return Cell(dist(m_engine), dist(m_engine));
    }

    template <typename T>
    typename T::iterator randomElement(T &container)
    {
        std::uniform_int_distribution<unsigned int> dist(0, container.size() - 1);
        auto it = container.begin();
        std::advance(it, dist(m_engine));
        return it;
    }

private:
    std::random_device m_device;
    std::default_random_engine m_engine;
};

namespace
{
    std::vector<Cell> validMoves(const Cell &current, const std::vector<bool> &visited, unsigned int size)
    {
        std::vector<Cell> moves;
        auto addMoveIfValid = [&moves, &visited, &size](unsigned int x, unsigned int y)
        {
            Cell c(x, y);
            if (c.first < size && c.second < size && !visited[c.first + size * c.second])
                moves.push_back(c);
        };
        addMoveIfValid(current.first + 1, current.second);
        addMoveIfValid(current.first - 1, current.second);
        addMoveIfValid(current.first, current.second + 1);
        addMoveIfValid(current.first, current.second - 1);
        return moves;
    }
}

std::vector<Edge> generateMaze(unsigned int size)
{
    Random rand;
    std::vector<bool> visited(size * size, false);
    std::vector<Cell> fringe;
    std::vector<Edge> edges;
    Cell startCell = rand.randomCell(size);
    fringe.push_back(startCell);
    visited[startCell.first + size * startCell.second] = true;

    while (edges.size() < size * size - 1)
    {
        assert(fringe.size() != 0);
        auto current = rand.randomElement(fringe);

        auto moves = ::validMoves(*current, visited, size);
        if (moves.size() > 0)
        {
            auto move = rand.randomElement(moves);
            edges.emplace_back(*current, *move);
            visited[move->first + size * move->second] = true;
            fringe.push_back(*move);
        }
        else
        {
            std::swap(*current, fringe.back());
            fringe.pop_back();
        }
    }

    return edges;
}

Image generateImage(const std::vector<Edge> &edges, unsigned int size)
{
    Image image(3 * size, 3 * size);
    for (unsigned int x = 0; x < size; ++x)
        for (unsigned int y = 0; y < size; ++y)
            image[3 * x + 1][3 * y + 1] = Color{ 0, 0, 0 };

    for (const Edge &edge : edges)
    {
	unsigned int x1 = edge.first.first;
	unsigned int y1 = edge.first.second;
	unsigned int x2 = edge.second.first;
	unsigned int y2 = edge.second.second;
        if (x1 == x2)
        {
            if (y1 < y2)
            {
                image[3 * x1 + 1][3 * y1 + 2] = Color{ 0, 0, 0 };
                image[3 * x2 + 1][3 * y2] = Color{ 0, 0, 0 };
            }
            else
            {
                image[3 * x1 + 1][3 * y1] = Color{ 0, 0, 0 };
                image[3 * x2 + 1][3 * y2 + 2] = Color{ 0, 0, 0 };
            }
        }
        else
        {
            if (x1 < x2)
            {
                image[3 * x1 + 2][3 * y1 + 1] = Color{ 0, 0, 0 };
                image[3 * x2][3 * y2 + 1] = Color{ 0, 0, 0 };
            }
            else
            {
                image[3 * x1][3 * y1 + 1] = Color{ 0, 0, 0 };
                image[3 * x2 + 2][3 * y2 + 1] = Color{ 0, 0, 0 };
            }
        }
    }

    return image;
}

int main(int argc, char *argv[])
{
    int size = 50;
    std::string filename = "output.ppm";
    if (argc >= 2)
        size = std::stoi(argv[1]);
    if (argc >= 3)
        filename = argv[2];

    std::vector<Edge> edges = generateMaze(size);
    Image image = generateImage(edges, size);
    image.savePPM(filename);

    return 0;
}
