#!/usr/bin/env python
# coding=utf-8
import urllib2
import re
from bs4 import BeautifulSoup

def OpenPage(url):
    Myheaders = {}
    request = urllib2.Request(url, headers = Myheaders)
    f = urllib2.urlopen(request)
    data = f.read()
    return data.decode("GBK", errors="ignore").encode("utf-8")

def Test1():
    print OpenPage("http://www.shengxu6.com/book/2967.html")


def ParseMainPage(page):
    soup = BeautifulSoup(page, "html.parser")
    ListCharts = soup.find_all(href=re.compile("read"))
    urlList = []
    for item in ListCharts:
        url = "http://www.shengxu6.com"+item["href"]
        urlList.append(url)
    return urlList

def Test2():
    page = OpenPage("http://www.shengxu6.com/book/2967.html")
    print ParseMainPage(page)

def ParseDetailPage(page):
    soup = BeautifulSoup(page, "html.parser")
    title = soup.find_all(class_="panel-heading")[0].get_text()
    content = soup.find_all(class_="panel-body")[0].get_text()
    return title, content

def Test3():
    page = OpenPage("http://www.shengxu6.com/read/2967_2008175.html")
    title, content = ParseDetailPage(page)
    print title
    print content

def WriteDataToFile(data):
    f = open("output.txt", "a+")
    f.write(data)
    f.close()


if __name__ == "__main__":
    MainPage = OpenPage("http://www.shengxu6.com/book/2967.html")
    GetUrl = ParseMainPage(MainPage)
    for item in GetUrl:
        print "Clone " + item
        page = OpenPage(item)
        title, content = ParseDetailPage(page)
        print "Clone title is"+ title
        data = "\n\n\n" + title + "\n\n\n" + content
        WriteDataToFile(data.encode("utf-8"))
    #Test1()
    #Test2()
    #Test3()

