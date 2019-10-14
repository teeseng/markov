cd ./data
rm parsed_trump_tweets.txt
echo "-------------- Cleaning Training Data ----------"
python only_text.py
cd ../src
echo "-------------- Building Markov Dictionary ----------"
g++ -std=c++11 -Wall build.h build.cc -o build_binary
./build_binary
echo "--------------- Generator Completed --------------"
cd ../
