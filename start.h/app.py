from flask import Flask,render_template, redirect, url_for, request, g

import sqlite3

app = Flask(__name__)



@app.route('/account/<user>', methods=['GET', 'POST'])
def account(user):
    if request.method == 'GET':
        return render_template('account.html', user = user)
    elif request.method =='POST':
        conn = sqlite3.connect('data.db')
        c = conn.cursor()
        data = request.form.get('item')
        description = request.form.get('done')
        c.execute('INSERT INTO Items VALUES(?, ?, ?)',(user, data, description))
        conn.commit()
        c.close()
    return render_template('account.html', user = user)


@app.route('/info/<user>', methods=['GET', 'POST'])
def info(user):
    g.db = sqlite3.connect('data.db')
    cur = g.db.execute('select * from Items')
    Item = [dict(id = row[0], item = row[1], description = row[2]) for row in cur.fetchall()]
    if request.method == 'POST':
        g.db.execute("delete from Items WHERE id = ?", (user,))
        g.db.commit()
        g.db.close()
    return render_template('info.html', post = Item, user=user)

@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        conn = sqlite3.connect('data.db')
        c = conn.cursor()
        c.execute('''Create TABLE IF NOT EXISTS Items(id Text, item Text, description Text)''')
        user = request.form.get('username')
        return redirect(url_for('account', user=user))
            
    return render_template('login.html')

@app.route('/edit/<user>', methods=['GET', 'POST'])
def edit(user):
    if request.method == 'GET':
        return render_template('edit.html', user = user)
    elif request.method =='POST':
        conn = sqlite3.connect('data.db')
        c = conn.cursor()
        data = request.form.get('item')
        description = request.form.get('done')
        c.execute('UPDATE Items SET description = ? WHERE item = ?',(description, data))
        conn.commit()
        c.close()
    return render_template('edit.html', user = user)

if __name__ == '__main__':
    app.run(debug=True)
