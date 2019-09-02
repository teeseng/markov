
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>


#define MIN_TWEET_LEN 5



class generator
{
    std::unordered_map<std::string,int> str_to_index;
    std::unordered_map<std::string,int> index_to_str;
    std::vector<std::vector<int>> assoc_mat;
    std::string src_file_path;

    void print_mat()
    {
        std::cout << "printing assoc matrix into mat.txt" << std::endl;
        std::ofstream out("mat.txt", std::ios_base::out);
        for(int i = 0; i < assoc_mat.size(); ++i) 
        {
            out << index_to_str[i] << ": [";
            for(int j = 0; j < assoc_mat[i].size(); ++j) 
            {
                out << index_to_str[j] << " = " << assoc_mat[i][j] << " | ";
            }
            out << "]";
        }
    }
    public:
    generator(std::string);
    ~generator();
    void build_assoc_mat();
    void build_markov_mat();
    std::string generate();

};
