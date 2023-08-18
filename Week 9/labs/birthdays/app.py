import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        # validate the input
        name = request.form.get("user_name")
        month = request.form.get("user_birth_month")
        day = request.form.get("user_birth_day")

        try:
            month = int(month)
            day = int(day)
        except ValueError:
            return render_template("index.html", placeholder="Invalid input!")

        if (month == 2 and day > 28) or (month in [4, 6, 9, 11] and day > 30) or (month in [1, 3, 5, 7, 8, 10, 12] and day > 31):
            return render_template("index.html", placeholder="Invalid date!")

        #insert valid input into db
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?);", name, month, day)
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        rows = db.execute("SELECT name, month, day FROM birthdays;")
        return render_template("index.html", birthdays=rows)
