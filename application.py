import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)
app.debug = True

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))


@app.route("/articles")
def articles():
    """Look up articles for geo."""
    # if api key is missing, run in flask terminal windown:
    # $ export API_KEY=AIzaSyDBUKAiRHxL0r4e6fxw7R79maPJDjbpSdg
    # example URL: http://ide50-jason314dcsd.c9users.io:8080/articles?geo=02138

    # if user reached route via GET (as by submitting a form via GET)
    if request.method == "GET":
        geo1 = request.args.get('geo')
        if not geo1:
            raise RuntimeError("no location specified in URL")
        geo2 = lookup(geo1)
        if len(geo2) > 6:
            del geo2[7:(len(geo2))]
        return jsonify(geo2)
    else:
        return render_template("index.html")


@app.route("/search")
def search():
    """Search for places that match query."""
    # example URL:
    # http://ide50-jason314dcsd.c9users.io:8080/search?q=Cambridge+Massa+0210

    # if user reached route via GET (as by submitting a form via GET)
    if request.method == "GET":
        q = request.args.get('q')
        if not q:
            raise RuntimeError("no query specified in URL")

        # process the query into single search terms
        qlist = []

        if q.find(',') == -1:  # these items have no ',' as a delimiter
            qlist = q.split()

        else:  # these items have a ',' as a delimiter
            qlist = q.split(',')
            for index, query in enumerate(qlist):
                qlist[index] = query.strip()

        regex = re.compile('[^0-9a-zA-Z ]')
        for index, query in enumerate(qlist):

            # for each query we remove non-alphanumeric chars with regex
            # the regex was found on https://stackoverflow.com/a/22521156
            query = regex.sub('', query)

            # for each query we make a SQL where-clause
            qlist[index] = ("(postal_code LIKE '%" + query +
                            "%' OR place_name LIKE '%" + query +
                            "%' OR admin_name1 LIKE '%" + query +
                            "%' OR country_code LIKE '%" + query +
                            "%' OR admin_code1 LIKE '%" + query + "%')")

        # join the generated WHERE clauses, and then join them with a base
        # query
        separator = " AND "
        qlist = separator.join(qlist)
        sqlq = """SELECT accuracy, admin_code1, admin_code2, admin_code3,
                   admin_name1, admin_name2, admin_name3,country_code,
                   latitude, longitude, place_name, postal_code FROM places
                   WHERE """ + qlist

        # execute the query and make the results into JSON
        rows = db.execute(sqlq)
        return jsonify(rows)

    else:
        return render_template("index.html")


@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND
            (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND
            (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
