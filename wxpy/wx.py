#!/usr/bin/env python
# coding=utf-8

import requests
from requests import exceptions
import urllib2
from bs4 import BeautifulSoup
import re
from wxpy import *
#import  schedule
import  time
import sys
reload(sys)
sys.setdefaultencoding("utf-8")

 
bot = Bot(console_qr=True, cache_path=True) 

def sendmsg(content):
    my_group = bot.friends().search(u'秦璠')[0]
    my_group.send(content) #发送天气预报            

def job():
    resp=urllib2.urlopen('http://www.weather.com.cn/weather/101110101.shtml')
    soup=BeautifulSoup(resp,'html.parser')
    tagToday=soup.find('p',class_="tem")  #第一个包含class="tem"的p标签即为存放今天天气数据的标签
    try:
        temperatureHigh=tagToday.span.string  #有时候这个最高温度是不显示的，此时利用第二天的最高温度代替。
    except AttributeError as e:
        temperatureHigh=tagToday.find_next('p',class_="tem").span.string  #获取第二天的最高温度代替
    
    temperatureLow=tagToday.i.string  #获取最低温度
    weather=soup.find('p',class_="wea").string #获取天气
    contents = '户县' + '\n' +  '最高温度:' + temperatureHigh + '\n' + '最低温度:' + temperatureLow + '\n' +  '天气:' + weather 
    sendmsg(contents)
    #print contents

if __name__ == "__main__":
    job()

bot.join()
#schedule.every().day.at("12:30").do(job) #规定每天12：30执行job()函数


