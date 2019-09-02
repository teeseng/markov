#include "build.h"
#include <random>



bool filler_twitter_handle(std::string);
int get_random_num(int n);

generator::generator(std::string fp) : src_file_path(fp) {

}

generator::~generator() { }


// one pass u fkers
void generator::build_assoc_mat()
{
    std::ifstream in(src_file_path, std::ios_base::in);

    int cur_index = 0;
    for(std::string line; std::getline(in, line);)
    {
        if(line.size() > MIN_TWEET_LEN)
        {
            std::string prev = "";
            std::stringstream ss(line);
            for(std::string tok; std::getline(ss, tok, ' ');)
            {
                if(str_to_index.find(tok) == str_to_index.end())
                {
                    str_to_index.insert(std::pair<std::string,int>(tok, cur_index));
                    assoc_mat[cur_index] = std::vector<int>(0,cur_index);
                    cur_index++;
                } 
                if(prev.size() > 0)
                {
                    int prev_index = str_to_index[prev];
                    int cur_index =  str_to_index[tok];
                    
                    if(assoc_mat[prev_index].size() < cur_index)
                    {
                        std::vector<int> tmp = std::vector<int>(0, cur_index + 1);
                        for (int i = 0; i < assoc_mat[prev_index].size(); ++i)
                        {
                            tmp[i] = assoc_mat[prev_index][i];  
                        }
                        assoc_mat[prev_index] = tmp;
                    }
                    assoc_mat[prev_index][cur_index]++;
                }
            }
        }
    }
}

std::string generator::generate() {
    //  return string for now for testing
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, assoc_mat.size());

    int start = dist(rng);

    return "";
}

// only allow Donald Trumps' twitter handle to go through
bool filter_twitter_handle(std::string handle)
{
    if(handle[0] == '@' && handle.compare("@realDonaldTrump") != 0) 
    {
        return false;
    }
    return true;
}

int main() {

}

