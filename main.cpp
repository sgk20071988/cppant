#include <iostream>
#include <unordered_set>
#include <stack>
#include <vector>
#include <algorithm>
#include <locale.h>
#include <chrono>

struct Cell
{
    size_t x;
    size_t y;

    Cell() { }
    Cell(size_t x, size_t y){
        this->x = x;
        this->y = y;
    }

    bool operator==(const Cell& otherCell) const{
        if (this->x == otherCell.x && this->y == otherCell.y) return true;
        else return false;
    }

    struct HashFunction{
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

    bool is_visited(Cell& cell){
        return visited_cells.count(cell) > 0;    
    }

    void generate_new_cells(std::stack<Cell>& stack, Cell& cell){
        auto up = Cell(cell.x,cell.y+1);
        if(is_reachble_cell(up) &&  !is_visited(up)){
            stack.emplace(up);
        }
        auto down = Cell(cell.x,cell.y-1);
        if(is_reachble_cell(down) &&  !is_visited(down)){
            stack.emplace(down);
        }
        auto left = Cell(cell.x-1,cell.y);
        if(is_reachble_cell(left) &&  !is_visited(left)){
            stack.emplace(left);
        }
        auto right = Cell(cell.x+1,cell.y);
        if(is_reachble_cell(right) &&  !is_visited(right)){
            stack.emplace(right);
        }
    }
    
    public:

    Ant(size_t x, size_t y){
        start_position = Cell(x,y);
    }

    void Walk(){
        visited_cells.clear();
        std::stack<Cell> new_cells;
        if(is_reachble_cell(start_position)){
            new_cells.push(start_position);
        }
        while (!new_cells.empty())
        {
            Cell& cell = new_cells.top();
            new_cells.pop();
            visited_cells.insert(cell);
            generate_new_cells(new_cells,cell);
        }
    }

    size_t Available_cells_count(){
        return visited_cells.size();
    }
};

int main(int, char**){
    std::locale rus("rus_rus.866");
    std::wcout.imbue(rus);
    Ant ant(1000,1000);
    auto begin = std::chrono::steady_clock::now();
    ant.Walk();
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::wcout << L"Количество доступных ячеек: " << ant.Available_cells_count() << std::endl;
    std::wcout << L"Время, млсек: " << elapsed_ms.count() << std::endl;
    return 0;
}
