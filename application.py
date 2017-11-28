from cs50 import SQL
from flask import (
    Flask, flash, redirect, render_template, request, session, url_for)
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = (
            "no-cache, no-store, must-revalidate")
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


# get the number of stocks a user has
def stock_sum(symbol, userid):
    row_total_q = db.execute("""SELECT SUM(NumberOfStocks) FROM holdings
                             WHERE Symbol=:symbol AND UserId=:userid""",
                             symbol=symbol, userid=userid)
    holding_sum = int(row_total_q[0]['SUM(NumberOfStocks)'])
    return holding_sum


# buy or sell a stock
def stock_order(action, symbol, price, amount, userid):

    # calculate necessary variables
    total_value = price * amount
    new_cash = cash(userid) - total_value

    # register the transaction
    db.execute("""INSERT INTO holdings(HoldingID, Symbol, NumberOfStocks,
               PriceOfPurchase, UserID) VALUES(NULL, :symbol, :amount,
               :price, :userid)""", symbol=symbol, amount=amount,
               userid=userid, price=price)

    # update the holding_meta table
    db.execute("""DELETE FROM holding_meta WHERE Symbol=:symbol AND
               Userid=:userid""", symbol=symbol, userid=userid)
    holding_sum = stock_sum(symbol, userid)
    db.execute("""INSERT INTO holding_meta(UserID, Symbol,
               NumberOfStocks) VALUES(:userid,
               :symbol, :holding_sum)""", symbol=symbol, userid=userid,
               holding_sum=holding_sum)

    # update user cash
    db.execute("UPDATE users SET cash=:new_cash WHERE id=:userid",
               new_cash=new_cash, userid=userid)
    return


# get amount of cash user has
def cash(userid):
    cash_row = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])
    return int(cash_row[0]["cash"])


# I really would have liked to make the /index page functional. But despite
# my best efforts I couldn't get the buttons to function. I also would have
# liked to de-duplicate my code, but I couldn't find resources telling me
# how to make functions outside of routes make the browser show the
# 'apology' page, making de-duplication impossible from a practical
# perspective.

@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show an overview table of stock holdings"""

    # Get list of stocks held by user
    userid = session["user_id"]
    row = db.execute("""SELECT Symbol, NumberOfStocks FROM holding_meta
                     WHERE userid=:userid""", userid=userid)

    # A table with values per stockwill be passed to the jinja template
    table = []

    # For each distinct stock:
    for i, v in enumerate(row):
        symbol = row[i]["Symbol"]
        total_q = row[i]["NumberOfStocks"]
        # get price and total value per stock,
        stock_info = lookup(symbol)
        if not stock_info:
            return apology("must provide stock symbol")
        stock_price = int(stock_info['price'])

        curvalue = total_q * stock_price
        # append to table
        i = dict(symbol=symbol, total_q=total_q, curvalue=curvalue,
                 curprice=stock_price)
        table.append(i)

    # Total value of stock holdings, cash and networth
    total_curvalue = 0
    for i, v in enumerate(table):
        total_curvalue += table[i]["curvalue"]
    usercash = cash(userid)
    networth = usercash + total_curvalue

    return render_template("index.html", table=table, usercash=usercash,
                           networth=networth,
                           total_curvalue=total_curvalue)

    # if user reached route via POST (as by clicking submit) and provided input
    # I can't get this to actually do something
    if request.method == "POST" and (int(request.form.get("amount")) > 0):
        # Initiate the buy or sell process.

        try:
            amount = int(request.form.get("amount"))
            if amount < 0:
                return apology("Shares must be positive integer")
        except:
            return apology("Shares must be positive integer")

        symbol = request.form.get("symbol")
        stock_info = lookup(symbol)
        if not stock_info:
            return apology("must provide stock symbol")
        stock_price = int(stock_info['price'])

        userid = session["user_id"]
        action = request.form.get("action")

        # check if user has enough cash
        if action == buy:
            total_value = stock_price * amount
            new_cash = cash(userid) - total_value
            if new_cash < 0:
                return apology("too little $$$....")
        elif action == sell:
            # make amount negative
            amount = 0-amount
            # check if user has enough shares to sell
            stockq = stock_sum(symbol, userid)
            if stockq + amount < 0:
                return apology("too little shares to sell")
        else:
            return apology("unknown action")

        stock_order(action, symbol, price, amount, userid)

        return render_template("index.html", table=table, usercash=usercash,
                               networth=networth,
                               total_curvalue=total_curvalue)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        try:
            amount = int(request.form.get("amount"))
            if amount < 0:
                return apology("Shares must be positive integer")
        except:
            return apology("Shares must be positive integer")

        symbol = request.form.get("symbol")
        stock_info = lookup(symbol)
        if not stock_info:
            return apology("must provide stock symbol")
        stock_price = int(stock_info['price'])

        userid = session["user_id"]
        action = buy

        # check if user has enough cash
        total_value = stock_price * amount
        new_cash = cash(userid) - total_value
        if new_cash < 0:
            return apology("too little $$$....")

        stock_order(action, symbol, stock_price, amount, userid)

        return render_template("buy.html")

    # else if user reached route via GET (as by clicking a link or via
    # redirect)
    else:
        return render_template("buy.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        try:
            amount = int(request.form.get("amount"))
            if amount < 0:
                return apology("Shares must be positive integer")
        except:
            return apology("Shares must be positive integer")

        symbol = request.form.get("symbol")
        stock_info = lookup(symbol)
        if not stock_info:
            return apology("must provide stock symbol")
        stock_price = int(stock_info['price'])

        userid = session["user_id"]
        # make amount negative
        amount = 0-amount
        # check if user has enough shares to sell
        stockq = stock_sum(symbol, userid)
        if stockq + amount < 0:
            return apology("too little shares to sell")

        action = sell
        stock_order(action, symbol, stock_price, amount, userid)

        return render_template("sell.html")

    # else if user reached route via GET (as by clicking a link or via
    # redirect)
    else:
        return render_template("sell.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # Get current userid
    userid = session["user_id"]

    # Get list of distinct stocks held
    row = db.execute("""SELECT Symbol, NumberOfStocks, DateOfPurchase,
                     TimeOfPurchase, PriceOfPurchase FROM holdings WHERE
                     userid=:userid""", userid=userid)

    # A table with values per stockwill be passed to the jinja template
    table = []

    # For each distinct stock:
    for i, v in enumerate(row):
        Symbol = row[i]["Symbol"]
        NumberOfStocks = row[i]["NumberOfStocks"]
        DateOfPurchase = row[i]["DateOfPurchase"]
        TimeOfPurchase = row[i]["TimeOfPurchase"]
        PriceOfPurchase = row[i]["PriceOfPurchase"]

        # append to table
        i = dict(Symbol=Symbol, NumberOfStocks=NumberOfStocks,
                 DateOfPurchase=DateOfPurchase, TimeOfPurchase=TimeOfPurchase,
                 PriceOfPurchase=PriceOfPurchase)
        table.append(i)

    return render_template("history.html", table=table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # ensure username exists and password is correct
        if (len(rows) != 1 or not (pwd_context.verify(request.form.get
                                   ("password"), rows[0]["hash"]))):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via
    # redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # redirect user to quoted page
        return redirect(url_for("quoted", symbol=request.form.get("symbol")))

    # else if user reached route via GET (as by clicking a link or via
    # redirect)
    else:
        return render_template("quote.html")


@app.route("/quoted/<string:symbol>", methods=["GET", "POST"])
@login_required
def quoted(symbol):
    """Show stock quote."""

    # if user reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":

        # call lookup
        stock_info = lookup(symbol)
        if not stock_info:
            return apology("must provide stock symbol")

        # render html with returned values from lookup
        return render_template("quoted.html", stock_info=stock_info)

    # else if user reached route via POST (as by submitting a form via POST)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username to register")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password to register")

        # ensure passwords match
        if not request.form.get("password") == (
                request.form.get("password_confirm")):

            return apology("make sure your password matches")

        # ensure username does not yet exist
        # rows = db.execute("SELECT * FROM users WHERE username = :username",
        #   username=request.form.get("username"))
        # return rows
        # return apology("username already exists")

        # ensure the password meets a minimum standard

        # hash the password and save the stuff to db
        password = request.form.get("password")
        hashedpwd = pwd_context.hash(password)

        db.execute("""INSERT INTO users(username, hash)
                   VALUES(:username, :hashedpwd)""",
                   username=request.form.get("username"), hashedpwd=hashedpwd)

        # redirect user to home page

        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via
    # redirect)
    else:
        return render_template("register.html")
