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


def ParseMainPage(page):
    soup = BeautifulSoup(page, "html.parser")
    ListCharts = soup.find_all(href=re.compile("htm"))
    urlList = []
    for item in ListCharts:
        url = "http://www.my285.com/zt/dygw/yzm/"+item["href"]
        urlList.append(url)
    return urlList

def Test2():
    page = OpenPage("http://www.my285.com/zt/dygw/yzm/")
    print ParseMainPage(page)


def ParseDetailPage(page):
    soup = BeautifulSoup(page, "html.parser")
    title = soup.find_all("td", colspan="2")[0].get_text()
    content = soup.find_all("td", colspan="2")[2].get_text()
    return title, content

def Test3():
    page = OpenPage("http://www.my285.com/zt/dygw/yzm/02.htm")
    title, content = ParseDetailPage(page)
    print title
    print content

def WriteDataToFile(data):
    f = open("output.txt", "a+")
    f.write(data)
    f.close()


if __name__ == "__main__":
    MainPage = OpenPage("http://www.my285.com/zt/dygw/yzm/")
    GetUrl = ParseMainPage(MainPage)
    for item in GetUrl[1:-1]:
        page = OpenPage(item)
        title, content = ParseDetailPage(page)
        print "Clone"+title
        data = "\n\n\n" + title + "\n\n\n" + content
        WriteDataToFile(data.encode("utf-8"))
    print "end"

