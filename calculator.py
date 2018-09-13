from flask import Flask,render_template, request
import sqlite3

app = Flask(__name__)

@app.route('/')
def cal():
        return render_template('calculator.html')



@app.route('/add', methods=['Get', 'POST'])
def add():
    if request.method == 'GET':
        return render_template('add.html')

    elif request.method == 'POST':
        num1 = request.form.get('Num1')
        num2 = request.form.get('Num2')
        answer = int(num1) + int(num2)
        return 'result %r' % answer
    
@app.route('/subtract', methods=['Get', 'POST'])
def subtract():
    if request.method == 'GET':
        return render_template('subtract.html')

    elif request.method == 'POST':
        num1 = request.form.get('Num1')
        num2 = request.form.get('Num2')
        answer = int(num1) - int(num2)
        return 'result %r' % answer

@app.route('/divide', methods=['Get', 'POST'])
def divide():
    if request.method == 'GET':
        return render_template('divide.html')

    elif request.method == 'POST':
        num1 = request.form.get('Num1')
        num2 = request.form.get('Num2')
        answer = int(num1) / int(num2)
        return 'result %r' % answer

@app.route('/multiply', methods=['Get', 'POST'])
def multiply():
    if request.method == 'GET':
        return render_template('multiply.html')

    elif request.method == 'POST':
        num1 = request.form.get('Num1')
        num2 = request.form.get('Num2')
        answer = int(num1) * int(num2)
        return 'result %r' % answer


if __name__ == '__main__':
    app.run(debug=True)
