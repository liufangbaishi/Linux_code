#!/usr/bin/env python
# coding=utf-8

import MySQLdb
from flask import Flask
from flask import render_template
from flask import request
from flask import redirect

app = Flask(__name__)

@app.route("/hello")
def Hello():
    str1 = "Message send"
    Info = [
        ("100", "zhangsan", "18"),
        ("101", "lisi", "20")
    ]
    dict = {"Ip":"127.0.0.1"}
    return render_template("index.html", message=str1, student=Info, Dict=dict)

@app.route("/main", methods=["GET","POST"])
def main():
    getData = GetMainData()
    return render_template("main.html", Data=getData)
    # data = request.args.get("Num")
    # return data

@app.route("/login", methods=["GET","POST"])
def login():
    if request.method == "POST":
        get_no = request.form.get("UserNo")
        get_psw = request.form.get("UserPsw")
        if get_no == "wei" and get_psw == "123":
            return redirect("/main")
        else:
            return "登录失败"
        
    return render_template("login.html")

def GetMainData():
    db = MySQLdb.connect(host="localhost", user="root", passwd="", db="wei", charset="utf8")
    cursor = db.cursor()
    sql = "select title, position, price from job"
    cursor.execute(sql)
    result = cursor.fetchall()
    return result

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8888)
    # GetData = GetMainData()[0]
    # for item in GetData:
    #     print item
