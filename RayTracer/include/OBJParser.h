#include <memory>
#include <vector>
#include "Triangle.h"
#include "Group.h"


struct line_item{
    std::string character;
    std::vector<double> points;
    void print() const{
    std::cout << "Char: " << this->character << std::endl << "Points: ";
    for(auto const& i: this->points){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
};

class Parser{
private:
    bool parse_line(std::istringstream& iss, struct line_item& out_param);
    Tuple parse_vertex(const struct line_item& input) const;
    bool parse_face(const struct line_item& input, Tuple& output) const;
    unsigned long invalid_reads;
    unsigned long current_line;
    std::vector<Tuple> vertices = {GenPoint(0,0,0)};
    void reset_vertices();
public:
    std::unique_ptr<Group> read(const std::string& fname, bool file=true);
    unsigned long get_invalid_reads() const;
    unsigned long get_current_line() const;

    Tuple get_vertex(int i) const;
    unsigned long get_total_vertices() const;
};