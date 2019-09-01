import csv
import sys
import re

def parse_out_hashtags_links(text_str):
    toks = re.split(r'[\s\+|\n\+\"\+-\+]', text_str)
    text = ""
    patterns = [r'#.', r'https://.']

    for t in toks:
        if not re.match(r'#.', t) and not re.match(r'https://.', t) and not re.match(r'http://.', t) and not re.match(r'www\.', t) and not re.match(r'pic\.twitter\.com.', t):
            t = t.rstrip('.!?')
            text += t + " "
        else:
            return ""
    return text

with open('parsed_trump_tweets.txt', 'w') as f:
    with open('data.csv', 'rU') as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',', quotechar='\n')
        for r in reader:
            text_str = r['Text']
            if text_str is not None:
                text_str.lstrip('\"')
                f.write(parse_out_hashtags_links(text_str) + "\n")


print("parsing out data file finished")
