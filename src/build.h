#ifndef BUILD_H
#define BUILD_H

#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>
#include <climits>
#include <memory>
#define MIN_TWEET_LEN 5


struct assoc_node;


typedef struct assoc_node 
{
    std::string tok;
    std::vector<std::pair<assoc_node*,int>> next_states;
    assoc_node(std::string input) : tok(input), next_states(std::vector<std::pair<assoc_node*, int>>()) {}
} mkv_state;

void add_out_link(mkv_state* node, std::string tok)
{
    bool found = false;
    if(node && tok.size() > 0)
    {
        for(int i = 0; i < node->next_states.size(); ++i)
        {
            if(node->next_states[i].first->tok.compare(tok) == 0)
            {
                node->next_states[i].second++;
                found = true;
            }
        }
    }
    if(!found)
    {
        mkv_state* newborn = new mkv_state(tok);
        node->next_states.push_back(std::pair<assoc_node*, int>(newborn,1));
    }
}

class generator
{
    std::string src_file_path;
    std::vector<mkv_state*> node_list;
    std::set<mkv_state*> starting_words;
    std::unordered_map<std::string, mkv_state*> counts;

    void print_mat()
    {
        std::cout << "~ Printing mat.txt" << std::endl;
        std::ofstream out("mat.txt", std::ios_base::out);
        for (mkv_state* node : node_list)
        {
            if (node) 
            {
                out << node->tok << " >>> ";
                for (std::pair<assoc_node*,int> edges : node->next_states)
                {
                    out << edges.first->tok << " (" << edges.second << ") ";
                }
                out << std::endl;
            }
        }
        out.clear();

        std::ofstream out2("starters.txt", std::ios_base::out);

        for (auto it = starting_words.begin(); it != starting_words.end(); ++it)
        {
            std::string word = (*it)->tok;
            out2 << word << std::endl;
        }
        out2.clear();
    }

    // only allow Donald Trumps' twitter handle to go through
    bool filter_twitter_handle(std::string handle)
    {
        if(handle[0] == '@' && handle.compare("@realDonaldTrump") == 0) 
        {
            return false;
        }
        return true;
    }

    public:
    generator(std::string);
    ~generator();
    void build_assoc_mat();
    void build_markov_mat();
    std::string generate(int);

};

#endif
