
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>

#define MIN_TWEET_LEN 5

std::unordered_map<std::string, std::set<std::string>> assoc_mat;
void build();
void print_mat();

int main() 
{

    std::cout << "init build process \n";

    build(); 

    std::cout << "finished build process \n";

    print_mat();
    
}

void build()
{
    std::ifstream in("../data/parsed_trump_tweets.txt", std::ios_base::in);
    for(std::string line; std::getline(in, line);)
    {
        if(line.size() > MIN_TWEET_LEN)
        {
            std::string prev = "";
            std::stringstream ss(line);
            for(std::string tok; std::getline(ss, tok, ' ');)
            {
                if(prev.size() > 0)
                {
                    if(assoc_mat.find(prev) == assoc_mat.end())
                    {
                        assoc_mat[prev] = std::set<std::string>();
                    }

                    if(tok.size() > 0) 
                    {
                        assoc_mat[prev].insert(tok);
                    }
                }
                prev = tok;
            }
        }
    }
}

void print_mat()
{
    std::ofstream out("mat.txt", std::ios_base::out);
    auto it = assoc_mat.begin();
    for(; it != assoc_mat.end(); ++it) 
    {
        std::set<std::string> temp = it->second;
        out << it->first << ": [";
        for(std::string s : temp) 
        {
            out << s << " ";
        }
        out << "] \n";
    }
}



















