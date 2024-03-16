#include <memory>
#include <vector>
#include "Triangle.h"
#include "Shape.h"
#include "Group.h"


class InvalidLineParser : public std::exception {
    public:
char * what () {
        return "Invalid line";
    }
};

class Parser{
private:
    std::string gen_group_id();
    static inline unsigned long Group_ID{0};
    void read_line(const std::string& line);
    void parse_vertex(std::istringstream& iss);
    void parse_normal(std::istringstream& iss);
    void parse_group(std::istringstream& iss);
    void parse_face(std::istringstream& iss);
    unsigned long invalid_reads=0;
    unsigned long current_line=0;
    std::string current_group="";
    std::vector<Tuple> vertices = {GenPoint(0,0,0)};
    std::vector<Tuple> normals = {GenVec(0,0,0)};
    std::map<std::string, std::unique_ptr<Group>> recorded_groups;
    void reset_vertices();
public:
    void read(const std::string& fname, bool file=true);
    unsigned long get_invalid_reads() const;
    unsigned long get_current_line() const;
    std::unique_ptr<Group> emit();
    Tuple get_vertex(int i) const;
    const std::vector<Tuple> get_vertices() const;
    unsigned long get_total_vertices() const;
};