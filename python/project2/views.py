#!/usr/bin/env python
# coding=utf-8

import urllib2
import json
import MySQLdb
import base64
import sys
from bs4 import BeautifulSoup
reload(sys)
sys.setdefaultencoding("utf-8")

def OpenPage(url):
    Myheaders = {}
    req = urllib2.Request(url, headers = Myheaders)
    f = urllib2.urlopen(req)
    data = f.read()
    return data

def Test1():
    url = "http://jy.51uns.com:8022/Frame/Data/jdp.ashx?rnd=1534591939846&fn=GetZhaopinList&StartDate=2000-01-01&SearchKey=&InfoType=-1&CompanyAttr=&CompanyType=&Area=&City=&CompanyProvice=&Post=&Zhuanye=&XLkey=&Age=&start=0&limit=15&DateType=999&InfoState=1&WorkType=0&CompanyKey=" 
    print OpenPage(url)

def ParseMainPage(page):
    data = json.loads(page)
    Idlist = []
    for item in data["rows"]:
        Idlist.append(item["Id"])
    return Idlist

def Test2():
    url = "http://jy.51uns.com:8022/Frame/Data/jdp.ashx?rnd=1534591939846&fn=GetZhaopinList&StartDate=2000-01-01&SearchKey=&InfoType=-1&CompanyAttr=&CompanyType=&Area=&City=&CompanyProvice=&Post=&Zhuanye=&XLkey=&Age=&start=0&limit=15&DateType=999&InfoState=1&WorkType=0&CompanyKey=" 
    page = OpenPage(url)
    print ParseMainPage(page)

def ParseDetailPage(page):
    JsonData = json.loads(page)
    Data = JsonData["Data"]
    Id = Data["Id"]
    Title = Data["CompanyTitle"]
    Price = Data["WorkPrice"]
    Position = Data["WorkPositon"]
    soup = BeautifulSoup(Data["EmployContent"], "html.parser")
    GetP = soup.find_all("p")
    ContentList = []
    for item in GetP:
        ContentList.append(item.get_text())
    Content = '\n'.join(ContentList)
    return Id, Title, Price, Position, Content

def Test3():
    #url = "http://jy.51uns.com:8022/Frame/Data/jdp.ashx?rnd=1534593752299&fn=GetOneZhaopin&JobId=b7986ae4f3f94e0bb79677c68b871e30&StartDate=2000-01-01"
    url = "http://jy.51uns.com:8022/Frame/Data/jdp.ashx?rnd=1534663777077&fn=GetOneZhaopin&JobId=0f58cb85e58f4ebab2f7a67068a3bef4&StartDate=2000-01-01"
    page = OpenPage(url)
    data = ParseDetailPage(page)
    print data

def WriteDataToMySQL(data):
    db = MySQLdb.connect(host="localhost", user="root", passwd="", db="wei", charset="utf8")
    cursor = db.cursor()
    #content = data[4]
    content = base64.b64encode(data[4])
    sql = "insert into job values('%s', '%s', '%s', '%s', '%s')" % (data[0], data[1], data[2], data[3], content)
    cursor.execute(sql)

    try:
        db.commit()
    except Exception,e:
        db.rollback()
        print e
    db.close()
    return "Success"

def Test4():
    data = ("100", "bite", "8-15k", "呵呵", "长得漂亮")
    WriteDataToMySQL(data)


if __name__ == "__main__":
    url = "http://jy.51uns.com:8022/Frame/Data/jdp.ashx?rnd=1534591939846&fn=GetZhaopinList&StartDate=2000-01-01&SearchKey=&InfoType=-1&CompanyAttr=&CompanyType=&Area=&City=&CompanyProvice=&Post=&Zhuanye=&XLkey=&Age=&start=0&limit=825&DateType=999&InfoState=1&WorkType=0&CompanyKey=" 
    page = OpenPage(url)
    IdList = ParseMainPage(page)
    prefix = "http://jy.51uns.com:8022/Frame/Data/jdp.ashx?rnd=1534593752299&fn=GetOneZhaopin&StartDate=2000-01-01&JobId="
    print "Clone begin"
    for item in IdList:
        DeatilUrl = prefix + item
        Deatil = OpenPage(DeatilUrl)
        data = ParseDetailPage(Deatil)
        print "Clone" + data[0]
        WriteDataToMySQL(data)
    print "Clone done"
