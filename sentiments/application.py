from flask import Flask, redirect, render_template, request, url_for
import os
import sys
import helpers
from analyzer import Analyzer
from nltk.tokenize import TweetTokenizer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))


    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, 100)
    if tweets == None:
        print("Retrieving tweets failed")
        sys.exit
    tknzr = TweetTokenizer(strip_handles=True, reduce_len=True)

    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)

    positive, negative, neutral = 0.0, 0.0, 100.0
    for i in range(len(tweets)):
        tweet = tknzr.tokenize(tweets[i])
        score = analyzer.analyze(tweet)
        if score > 0.0:
            positive += 1
            neutral -= 1
        elif score < 0.0:
            negative += 1
            neutral -= 1
        else:
            continue

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
