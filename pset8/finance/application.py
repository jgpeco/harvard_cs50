import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    #getting user info
    names = db.execute('SELECT username, cash FROM users WHERE id=:id', id=session["user_id"])
    userInfo = {}
    userInfo['name'] = names[0]['username']
    userInfo['cash'] = names[0]['cash']
    userInfo['grandTotal'] = userInfo['cash']

    #getting stocks of user
    rows = db.execute('SELECT * FROM stocks WHERE username_id=:id', id=session["user_id"])

    stocks = []
    for stock in rows:
        #don't show stocks that you have sold all the shares
        if stock['shares'] <= 0:
            continue
        #getting current price of each stock
        stockInfo = lookup(stock['symbol'])
        stock['currentPrice'] = stockInfo['price']
        #getting total price of holdings to the stock
        stock['totalHoldings'] = stockInfo['price'] * stock['shares']
        #updating grandTotal of user
        userInfo['grandTotal'] = userInfo['grandTotal'] + stock['totalHoldings']
        #appending to list that will be passed

        #convertin to usd for better visualizaztion
        stock['totalHoldings'] = usd(stock['totalHoldings'])
        stock['currentPrice'] = usd(stock['currentPrice'])
        stocks.append(stock)

    #converting to a better visualization
    userInfo['cash'] = usd(userInfo['cash'])
    userInfo['grandTotal'] = usd(userInfo['grandTotal'])

    return render_template('dashboard.html', userInfo=userInfo, stocks=stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == 'POST':
        symbolInput = request.form.get('symbol')
        shares = request.form.get('shares')
        symbol = lookup(symbolInput)

        if not symbolInput or not shares:
            return apology('You must provide a value for the fields', 403)
        elif not symbol:
            return apology('You must provide a valid stock symbol', 403)

        #getting cash from table
        rows = db.execute('SELECT cash FROM users WHERE id=:id', id=session["user_id"])
        cash = rows[0]['cash']

        purchasePrice = symbol['price'] * int(shares)

        if purchasePrice > cash:
            flash("You don't have enough funds for this purchase")
            return redirect('/buy')



        #insert in transactions history
        db.execute('INSERT INTO transactions (username_id, type, symbol, shares, stock_price, total_price, date) VALUES (:username_id, :type, :symbol, :shares, :stock_price, :total_price, :date)', username_id=session["user_id"], type='BUY', symbol=symbol['symbol'], shares=shares, stock_price=symbol['price'], total_price=purchasePrice, date=datetime.now())
        #insert into stocks owned by user
        stocksOwned = db.execute('SELECT symbol FROM stocks WHERE symbol=:symbol AND username_id=:user_id', symbol=symbol['symbol'], user_id=session["user_id"])
        if not stocksOwned:
            db.execute('INSERT INTO stocks (username_id, symbol, shares) VALUES (:username_id, :symbol, :shares)', username_id=session["user_id"], symbol=symbol['symbol'], shares=shares)
        else:
            rows = db.execute('SELECT symbol FROM stocks WHERE symbol=:symbol AND username_id=:user_id', symbol=symbol['symbol'], user_id=session["user_id"])
            oldShares = rows[0]['shares']
            db.execute('UPDATE stocks SET shares = :new_shares WHERE symbol=:symbol AND username_id=:user_id', new_shares = (oldShares + shares), symbol=symbol['symbol'], user_id=session["user_id"])
        #update cash from user
        newCash = cash - purchasePrice
        db.execute('UPDATE users SET cash = :new_cash WHERE id=:id', new_cash = newCash, id=session["user_id"])

        flash('Purchase completed!')
        return redirect('/')
    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = []
    userHistory = db.execute('SELECT * FROM transactions WHERE username_id=:id', id=session['user_id'])

    for itemHistory in userHistory:
        item = {}
        item['type'] = itemHistory['type']
        item['symbol'] = itemHistory['symbol']
        item['shares'] = itemHistory['shares']
        item['stockPrice'] = itemHistory['stock_price']
        item['totalPrice'] = itemHistory['total_price']
        item['date'] = itemHistory['date']
        history.append(item)

    #changing the order of operations to display the last first
    history.reverse()
    return render_template('history.html', history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    if session.get('_flashes'):
        flashes = session.get('_flashes')
        session.clear()
        session['_flashes'] = flashes
    else:
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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    if request.method == 'POST':
        symbol = request.form.get('symbol')
        responseSymbol = lookup(symbol)

        if not responseSymbol:
            return apology("Couldn't find the symbol submitted. Please try again.")

        return render_template('quoted.html', symbol=responseSymbol)
    else:
        return render_template('quote.html')


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == 'POST':
        #checking username/pw
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if not username:
            return apology('Must provide an username', 403)
        elif not password:
            return apology('Must provide a password', 403)
        elif not confirmation:
            return apology('Must confirm password', 403)

        if password != confirmation:
            return apology('Passwords doesnt match, please check', 403)

        #checking users on db
        rows = db.execute('SELECT username FROM users WHERE username=:username', username=username)

        if len(rows) > 0:
            return apology('This username already exists, please try again with a different one', 403)

        db.execute('INSERT INTO users (username, hash) VALUES (:username, :password)', username = username, password = generate_password_hash(password))
        flash('User created successfully!')
        return redirect('/login')

    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == 'POST':
        stockSymbol = request.form.get('stockSymbol')
        shares = int(request.form.get('shares'))
        stocks = []

        rows = db.execute('SELECT symbol, shares FROM stocks WHERE username_id=:id', id=session["user_id"])
        for stock in rows:
            stocks.append(stock['symbol'])

        #checking if stock is on the user stock collection
        if stockSymbol not in stocks:
            return apology('Sorry, the stock you select is not part of your stock collection', 406)

        #checking the number of shares
        searchStock = db.execute('SELECT shares FROM stocks WHERE symbol=:symbol AND username_id=:id', id=session["user_id"], symbol=stockSymbol)
        stockSharesTotal = searchStock[0]['shares']
        if shares > stockSharesTotal or shares < 0:
            return apology('Sorry, the amount of shares provided is not valid, please check your shares.', 403)

        #if everything is ok
        stockInfo = lookup(stockSymbol)
        stockPrice = stockInfo['price']
        totalPrice = stockPrice * shares
        #updates the number of shares
        newShares = stockSharesTotal - shares
        db.execute('UPDATE stocks SET shares=:newShares WHERE symbol=:symbol', newShares=newShares, symbol=stockSymbol)
        #insert new history transaction
        db.execute('INSERT INTO transactions (username_id, type, symbol, shares, stock_price, total_price, date) VALUES (:username_id, :type, :symbol, :shares, :stock_price, :total_price, :date)', username_id=session["user_id"], type='SELL', symbol=stockSymbol, shares=shares, stock_price=stockPrice, total_price=totalPrice, date=datetime.now())
        #update user cash
        user = db.execute('SELECT cash FROM users WHERE id=:id', id=session["user_id"])
        cash = user[0]['cash'] + stockPrice
        db.execute('UPDATE users SET cash=:cash WHERE id=:id', cash=cash, id=session["user_id"])

        flash('Stocks sold successfuly!')
        return redirect('/')
    else:
        stocks = []

        rows = db.execute('SELECT * FROM stocks WHERE username_id=:id', id=session["user_id"])
        for stock in rows:
            stocks.append(stock['symbol'])

        return render_template('sell.html', stocks=stocks)

@app.route('/delete', methods=['GET', 'POST'])
@login_required
def delete():
    if request.method == 'POST':
        db.execute('DELETE FROM transactions WHERE username_id=:id', id=session['user_id'])
        db.execute('DELETE FROM stocks WHERE username_id=:id', id=session['user_id'])
        db.execute('DELETE FROM users WHERE id=:id', id=session['user_id'])
        session.clear()

        flash('User account deleted')
        return redirect('/login')
    else:
        return render_template('delete.html')

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
