#ifndef BUILD_H
#define BUILD_H

#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>

#define MIN_TWEET_LEN 5


struct assoc_node;


typedef struct assoc_node 
{
    std::string tok;
    std::vector<std::pair<assoc_node*,int>> next_states;
    assoc_node(std::string input) : tok(input), next_states(std::vector<std::pair<assoc_node*, int>>()) {}
} mkv_state;

class generator
{


    std::string src_file_path;
    std::vector<mkv_state*> node_list;

    void print_mat()
    {
        std::cout << "printing assoc matrix into mat.txt" << std::endl;
        std::ofstream out("mat.txt", std::ios_base::out);
        for (mkv_state* node : node_list)
        {
            out << node->tok << " = [ ";
            for (std::pair<assoc_node*,int> edges : node->next_states)
            {
                out << edges.first->tok << " : " << edges.second << "|";
            }
            out << "]" << std::endl;
        }
        out.clear();
    }

    public:
    generator(std::string);
    ~generator();
    void build_assoc_mat();
    void build_markov_mat();
    std::string generate();

};

#endif
