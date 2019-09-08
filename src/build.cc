#include "build.h"
#include <random>


bool is_ending_token(std::string);
bool filler_twitter_handle(std::string);
int get_random_num(int n);

generator::generator(std::string fp) {
    src_file_path = fp;
}

generator::~generator() { }


// one pass u fkers
void generator::build_assoc_mat()
{
    std::unordered_map<std::string, mkv_state*> counts;

    // first pass
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
                if(counts.find(tok) == counts.end())
                { 
                    mkv_state* newborn = new mkv_state(tok);
                    counts.insert(std::pair<std::string, mkv_state*>(tok, newborn));
                    node_list.push_back(newborn); 
                } 
                if(prev.size() > 0)
                {
                    mkv_state* prev_node = counts[prev];
                         
                }
            }
        }
    }
  
}

std::string generator::generate() {
    //  return string for now for testing
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, node_list.size());

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

bool is_ending_token(std::string token)
{
    return token[token.size() - 1] == '.' || token[token.size() - 1] == '!' || token[token.size()-1] == '?';
}

int main() {

    std::cout << "Testing my builder \n";
    generator* gen_inst = new generator("../data/parsed_trump_tweets.txt");
    gen_inst->build_assoc_mat();
    
    std::cout << "Building assoc mat finished \n";
    std::cout << gen_inst->generate() << std::endl;

    return 0;
}















