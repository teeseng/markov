import csv
import sys
import re

def parse_out_hashtags_links(text_str):
    #toks.replace("...", "+")
    toks = re.split(r'[\s\+\-\+\n\+\"\+!\+]', text_str)
    text = ""
    patterns = [r'#.', r'https://.']

    for t in toks:
        if not re.match(r'#.', t) and not re.match(r'https://.', t) and not re.match(r'http://.', t) and not re.match(r'www\.', t) and not re.match(r'pic\.twitter\.com.', t) and not re.match(r'twitter.com.', t):
            if len(t) > 0:
                text += t.lower() + " "
        else:
            return ""
    return text

with open('parsed_trump_tweets.txt', 'w') as f:
    with open('data.csv', 'rU') as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',', quotechar='\"')
        for r in reader:
            text_str = r['Text']
            if len(text_str) > 0:
                text_str.lstrip('\"')
                f.write(parse_out_hashtags_links(text_str) + "\n")


