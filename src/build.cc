#include "build.h"

#include <chrono>
#include <random>


bool is_ending_token(std::string);
int get_random_num(int n);

generator::generator(std::string fp) {
    src_file_path = fp;
}

generator::~generator() { 
    std::cout << "~generator() " << std::endl;
    for (mkv_state* node : node_list)
    {
        delete node;
    }
}


void generator::build_assoc_mat()
{
    std::unordered_map<std::string, mkv_state*> counts;

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
                if(filter_twitter_handle(tok))
                {
                    std::cout << "twitter_handle" << std::endl;
                    continue;
                }
                if(counts.find(tok) == counts.end())
                { 
                    mkv_state* newborn = new mkv_state(tok);
                    counts.insert(std::pair<std::string, mkv_state*>(tok, newborn));
                    node_list.push_back(newborn); 
                } 
                if(prev.size() > 0)
                {
                    mkv_state* prev_node = counts[prev];
                    bool found = false;
                    for(int i = 0; i < prev_node->next_states.size(); i++)
                    {
                        std::string cur = prev_node->next_states[i].first->tok;
                        if (tok.compare(cur) == 0)
                        {
                            prev_node->next_states[i].second++;
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        prev_node->next_states.push_back(
                            std::pair<assoc_node*,int>(counts[tok],1)
                        );  
                    }

                }
                prev = tok;
            }
        }
    }

    print_mat();
}

std::string generator::generate() {
    //  return string for now for testing
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, node_list.size());

    int start = dist(rng);


    return "";
}


bool is_ending_token(std::string token)
{
    return token[token.size() - 1] == '.' || 
                token[token.size() - 1] == '!' || 
                    token[token.size()-1] == '?';
}

int main() {

    std::cout << "Testing my builder \n";
    generator* gen_inst = new generator("../data/parsed_trump_tweets.txt");

    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    gen_inst->build_assoc_mat();

    auto stop = high_resolution_clock::now();
    auto time = duration_cast<microseconds>(stop-start);


    
    
    std::cout << "Building assoc mat finished, took " << time.count() << " microsecs\n";
//    std::cout << gen_inst->generate() << std::endl;
    delete gen_inst;

    return 0;
}















