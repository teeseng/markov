#include "build.h"

bool is_ending_token(std::string);
int get_random_num(int n);
std::string clean_string(std::string input);
bool is_ending_token(char c);


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
                bool has_period = false;
                int len = tok.size() - 1;
                if(tok[len] == '.' && tok.size() > 1 && tok[len-1] != '.')
                {
                    has_period = true;
                }

                tok = clean_string(tok);

                if(counts.find(tok) == counts.end())
                { 
                    mkv_state* newborn = new mkv_state(tok);
                    counts.insert(std::pair<std::string, mkv_state*>(tok, newborn));
                    node_list.push_back(newborn); 
                } 


                if(prev.size() > 0)
                {
                    mkv_state* prev_node = counts[prev];
                    add_out_link(prev_node,tok);
                    /*
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
                    if(!found && prev.compare(tok) != 0)
                    {
                        prev_node->next_states.push_back(
                            std::pair<assoc_node*,int>(counts[tok],1)
                        );  
                    }
                    */
                }
                if(has_period)
                {
                    mkv_state* node = counts[tok];
                    add_out_link(node,".");
                    /*
                    mkv_state* period_node = new mkv_state(".");
                    for (int i = 0; i < node->next_states.size(); ++i) 
                    {
                        if(node->next_states[i].first->tok == '.')
                        {
                        }
                    }
                    node->next_states.push_back(std::pair<assoc_node*, int>(period_node,1));
                    */
                    prev = "";
                }
                else 
                {
                    prev = tok;
                }
            }
        }
    }

    // if DEBUG
    print_mat();
}


std::string generator::generate(int num_sentences) {
    //  return string for now for testing
    std::random_device rd;
    std::mt19937 rng(rd());
    typedef std::uniform_int_distribution<std::mt19937::result_type> rng_19937;
    rng_19937 dist(0, node_list.size());

    std::set<std::string> visited;
    std::string result = "";
    for (int i = 0; i < num_sentences; ++i) {

        int cur = dist(rng);
        mkv_state* start_node = node_list[cur];
        auto out_nodes = start_node->next_states;
        while(out_nodes.size() < 3)
        {
            cur = dist(rng);
            start_node = node_list[cur];
            out_nodes = start_node->next_states;
        }



        int max_freq = INT_MIN;
        visited.insert(start_node->tok);
        result += start_node->tok + " ";

        do {
            rng_19937 change_out(0, out_nodes.size());
            dist = change_out;
            cur = dist(rng);
        } while (node_list[cur]->next_states.size() < 3);
        
    }
    return result;
}

std::string clean_string(std::string input)
{
    int len = (int)(input.size() - 1);
    while(is_ending_token(input[len]))
    {
        len--; 
    }
    input = input.substr(0, ++len);
    return input;
}

bool is_ending_token(char c)
{
    return c == '.' || c == '!' || c == ',' || c == '?' || c == ':' || c == '"';
}

int main() {

    std::cout << "INIT build \n";
    generator* gen_inst = new generator("../data/parsed_trump_tweets.txt");

    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    gen_inst->build_assoc_mat();

    auto stop = high_resolution_clock::now();
    auto time = duration_cast<microseconds>(stop-start);

    std::ofstream out_file("generated_tweet.txt", std::ios_base::out);
    std::cout << "Building assoc mat finished, took " << time.count() << " microsecs\n";
    out_file << gen_inst->generate(3);
    out_file << std::endl;
    delete gen_inst;

    return 0;
}















