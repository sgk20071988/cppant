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

    void generate_new_cells(std::vector<Cell>&new_cells, Cell& cell){
        new_cells.clear();
        new_cells.reserve(4);
        new_cells.emplace_back (cell.x,cell.y+1);
        new_cells.emplace_back (cell.x,cell.y-1);
        new_cells.emplace_back (cell.x-1,cell.y);
        new_cells.emplace_back (cell.x+1,cell.y);
    }

    void filter_cells(std::vector<Cell>& from, std::vector<Cell>& to){
        std::copy_if(from.begin(), from.end(),
                 std::back_inserter(to),
                 [this](Cell& cell) { return is_reachble_cell(cell) &&  !is_visited(cell); });
    }  
    
    void to_stack(std::stack<Cell>& stack, std::vector<Cell>& from){
        for (auto cell : from){
            stack.push(cell);
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
            Cell cell = new_cells.top();
            new_cells.pop();
            visited_cells.insert(cell);
            std::vector<Cell> gen_cells;
            std::vector<Cell> filtred_cells;
            generate_new_cells(gen_cells,cell);
            filter_cells(gen_cells,filtred_cells);
            to_stack(new_cells,filtred_cells);
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
