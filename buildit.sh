cd ./data
rm parsed_trump_tweets.txt
python only_text.py
cd ../src
g++ -std=c++11 build.cc -o build_binary
./build_binary
cd ../
echo "completed run script"
