from flask import Flask,render_template, request
import sqlite3

app = Flask(__name__)

@app.route('/', methods=['Get', 'POST'])
def cal():
    if request.method == 'GET':
        return render_template('calculator.html')

    elif request.method == 'POST':
        result = request.form.get('result')
        answer = eval(result)
        return 'result %r' % answer


if __name__ == '__main__':
    app.run(debug=True)
