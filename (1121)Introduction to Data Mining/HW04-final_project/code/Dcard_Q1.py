# 載入使用的套件
import pandas as pd
import requests
import json
import time

url = 'https://www.dcard.tw/service/api/v2/forums/talk/posts?limit=30'
headers = { 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36'}

alldata = []
last_article = ''

for i in range(5):
    if i != 0: # 判斷是否是第一次執行
        request_url = url +'&before='+ str(last_article)
    else:
        request_url = url # 第一次執行，不須加上後方的before
    list_req = requests.get(request_url, headers=headers) # 請求
    print(list_req.text)
    #######################################
    #rejs = list_req.json()  -->json error
    #######################################
    '''
    # 將整夜程式碼爬下來
    getdata = json.loads(list_req.content)
    alldata.extend(getdata) # 將另一個陣列插在最後面   

    last_article = getdata[-1]['id'] # 取最後一篇文章
    print(last_article)
    '''    
    time.sleep(20)

alldata = pd.DataFrame(alldata)

