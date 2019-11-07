#include "build.h"

bool is_ending_token(std::string);
int get_random_num(int n);
std::string clean_string(std::string input);
bool is_ending_token(char c);
std::string capitalize(std::string);


generator::generator(std::string fp) {
    src_file_path = fp;
}

generator::~generator() { 
    for (mkv_state* node : node_list)
    {
        delete node;
    }
}

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
                }
                else 
                {
                    if(tok[0] >= 'a' && tok[0] < 'z') 
                    {
                        this->starting_words.insert(counts[tok]);
                    }
                }

                if(has_period)
                {
                    mkv_state* node = counts[tok];
                    add_out_link(node,".");
                    prev = "";
                }
                else 
                {
                    prev = tok;
                }
            }
        }
    }
}


std::string generator::generate(int num_sentences, int min_sentence_len) {

    //int min_sentence_len = 10;
    std::random_device rd;
    std::mt19937 rng(rd());
    typedef std::uniform_int_distribution<std::mt19937::result_type> rng_19937;
    rng_19937 dist(0, starting_words.size());

    std::string result = "";
    for (unsigned int i = 0; i < num_sentences; ++i) 
    {
        int len = 0;
        int index = dist(rng);
        auto set_iter = starting_words.begin();
        while(index--)
        {
            ++set_iter;
        }
        std::string cur = (*set_iter)->tok;
        result += capitalize(cur);

        while(cur.compare(".") != 0) {
            mkv_state* node = counts[cur];
            int max_count = -1;
            std::string next_string;
            std::pair<assoc_node*,int>* highest_freq_pair;
            for(int i = 0; i < node->next_states.size(); ++i)
            {
                std::pair<assoc_node*,int>* p = &node->next_states[i];
                if(p->second > max_count)
                {
                    // remove stupid tokens that weren't cleaned correnctly in 
                    // the data set
                    if(p->first->tok.compare(".") == 0 && len < min_sentence_len) 
                    {
                        continue; 
                    }
                    if(p->first->tok.find("http") > -1)
                    {
                        continue;
                    }
                    max_count = p->second;
                    next_string = p->first->tok;
                    highest_freq_pair = p;
                }
            }
            for(std::pair<assoc_node*,int> p : node->next_states)
            {
                if(p.second > max_count)
                {
                    if(p.first->tok.compare(".") == 0 && len >= min_sentence_len) 
                    {
                        continue; 
                    }
                    if(p.first->tok.find("http") > -1)
                    {
                        continue;
                    }
                    if(p.first->next_states.size() == 0)
                    {
                        continue;
                    }
                    max_count = p.second;
                    next_string = p.first->tok;
                    highest_freq_pair = &p;
                } 
            }
            highest_freq_pair->second = 1;
            cur = next_string;
            if (cur.compare(".") == 0) 
            {
                result += cur + " ";
            }
            else 
            {
                result += " " + cur;
            }
            len++;
        }
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

std::string capitalize(std::string in)
{
    if(in[0] >= 'a' && in[0] <= 'z')
    {
        in[0] = in[0] - 32;
    }
    return in;
}

std::string clean_text(std::string input)
{
    std::string cleaned_text = input;
    std::stringstream ss(cleaned_text);
    std::string temp;
    while (getline(ss, temp, ' ')) 
    {
        if(temp.compare("i") == 0)
        {
            temp = "I";
        }
    }
    cleaned_text += "\n";
    return cleaned_text;
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
    std::string generated_text = gen_inst->generate(11,1);
    generated_text = clean_text(generated_text);

    out_file << generated_text;
    out_file << std::endl;
    delete gen_inst;

    return 0;
}















