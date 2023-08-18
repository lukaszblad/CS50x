import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
@login_required
def index():
    """Show portfolio of stocks"""
    # extract data from db
    user = session["user_id"]
    stocks = db.execute("SELECT * FROM shares_owned WHERE user_id = ?;", user)
    cash = round(db.execute("SELECT cash FROM users WHERE id = ?;", user)[0]["cash"], 2)
    print(f"cash {cash}")

    # adding current prices to the dictionary
    total = 0
    for i in range(len(stocks)):
        stocks[i]["price"] = round(int(lookup(stocks[i]["symbol"])["price"]), 2)
        total = round(
            total + float(stocks[i]["price"]) * float(stocks[i]["shares_owned"]), 2
        )
        stocks[i]["value"] = format(
            round(stocks[i]["price"] * stocks[i]["shares_owned"], 2), ".2f"
        )
        stocks[i]["price"] = format(stocks[i]["price"], ".2f")

    # formatting values
    grandtotal = total + cash
    total = format(total, ".2f")
    cash = format(cash, ".2f")

    # render web page
    return render_template(
        "index.html", stocks=stocks, cash=cash, total=total, grandtotal=grandtotal
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # validate stock
        if lookup(symbol) is None:
            return apology("Invalid stock!")

        # validate number of shares
        try:
            shares = int(shares)
        except ValueError:
            return apology("Invalid input!")

        if shares < 1:
            return apology("Invalid amount of shares!")

        # check if user hase enough funds
        user = session["user_id"]
        price_per_share = round(lookup(symbol)["price"], 2)
        total_price = round(price_per_share * shares, 2)
        user_funds = round(
            int(db.execute("SELECT cash FROM users WHERE id = ?;", user)[0]["cash"]), 2
        )

        if total_price > user_funds:
            return render_template("buy.html", funds="Insufficient funds!")

        # subtract funds from user's funds
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?;",
            format(user_funds - total_price, ".2f"),
            user,
        )

        # register transaction in db
        db.execute(
            "INSERT INTO transactions (user_id, type, symbol, shares, price_per_share, total_value, date) VALUES (?, 'buy', ?, ?, ?, ?, CURRENT_TIMESTAMP);",
            user,
            symbol,
            shares,
            format(price_per_share, ".2f"),
            format(total_price, ".2f"),
        )

        # update shares owned by the user
        # check if user already owns the stock
        stock_owned = db.execute(
            "SELECT * FROM shares_owned WHERE user_id = ? AND symbol = ?", user, symbol
        )
        if len(stock_owned) == 0:
            db.execute(
                "INSERT INTO shares_owned (user_id, symbol, shares_owned) VALUES (?, ?, ?);",
                user,
                symbol,
                shares,
            )
        else:
            db.execute(
                "UPDATE shares_owned SET shares_owned = ?;",
                stock_owned[0]["shares_owned"] + shares,
            )

        # redirect to index
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history", methods=["GET"])
@login_required
def history():
    """Show history of transactions"""
    user = session["user_id"]
    history = db.execute("SELECT * FROM transactions WHERE user_id = ?;", user)
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if len(symbol) == 0:
            return apology("Insert Symbol!")
        # validate provided symbol
        if lookup(symbol) is None:
            return apology("Invalid Stock!")

        # render quoted if stock valid
        price = round(float(lookup(symbol)["price"]), 2)
        return render_template("quoted.html", symbol=symbol, price=format(price, ".2f"))

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # validate username
        username = request.form.get("username")
        if len(username) == 0:
            return apology("Insert username")
        username_db = db.execute("SELECT * FROM users WHERE username = ?;", username)
        if len(username_db) != 0:
            return apology("Username already taken")

        # validate password
        pass_1 = request.form.get("password")
        if len(pass_1) == 0:
            return apology("Insert username!")
        pass_2 = request.form.get("confirmation")
        if pass_1 != pass_2:
            return apology("Different passwords!")

        # insert user in db
        password_hash = generate_password_hash(pass_1)
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?);", username, password_hash
        )
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # global variables
    user = session["user_id"]
    stocks = db.execute("SELECT * FROM shares_owned WHERE user_id = ?;", user)

    if request.method == "POST":
        user_funds = db.execute("SELECT cash FROM users WHERE id = ?;", user)[0]["cash"]
        symbol = request.form.get("symbol")
        quantity = int(request.form.get("shares"))
        stock_price = float(lookup(symbol)["price"])

        # validate quantity input
        if quantity > int(stocks[0]["shares_owned"]):
            return apology("Not enough shares!")

        # update users table in db
        db.execute(
            "UPDATE users SET cash  = ? WHERE id = ?;",
            user_funds + quantity * stock_price,
            user,
        )

        # update transaction table in db
        db.execute(
            "INSERT INTO transactions (user_id, type, symbol, shares, price_per_share, total_value, date) VALUES (?, 'sell', ?, ?, ?, ?, CURRENT_TIMESTAMP);",
            user,
            symbol,
            quantity,
            stock_price,
            quantity * stock_price,
        )

        # update shares owned in db
        db.execute(
            "UPDATE shares_owned SET shares_owned = ? WHERE user_id = ?;",
            int(stocks[0]["shares_owned"]) - quantity,
            user,
        )

        return redirect("/")

    else:
        return render_template("sell.html", stocks=stocks)
