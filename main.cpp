#include <iostream>
#include <unordered_set>

struct Cell
{
    size_t x;
    size_t y;

    Cell() { }
    Cell(size_t x, size_t y)
    {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Cell& otherCell) const
    {
        if (this->x == otherCell.x && this->y == otherCell.y) return true;
        else return false;
    }

    struct HashFunction
    {
        size_t operator()(const Cell& cell) const
        {
            size_t xHash = std::hash<int>()(cell.x);
            size_t yHash = std::hash<int>()(cell.y) << 1;
            return xHash ^ yHash;
        }
    };
};

class Ant
{
    Cell start_position;
    std::unordered_set<Cell, Cell::HashFunction> visited_cells;

    int sum_digits(size_t number){
        int sum = 0;
        while (number != 0) {
            sum = sum + number % 10;
            number = number / 10;
        }
        return sum;
    }

    int sum_cell_coordinates(Cell& cell){
        return sum_digits(cell.x) + sum_digits(cell.y);
    }

    bool is_reachble_cell(Cell& cell){
        return sum_cell_coordinates(cell) <= 25;
    }

    public:

    Ant(size_t x, size_t y){
        start_position = Cell(x,y);
    }

    void Walk(){
        visited_cells.clear();

    }

    size_t Available_cells_count(){
        return visited_cells.size();
    }
};

int main(int, char**){
    Ant ant(1000,1000);
    ant.Walk();
    std::cout << "Количество доступных ячеек: " << ant.Available_cells_count() << std::endl;
    return 0;
}
