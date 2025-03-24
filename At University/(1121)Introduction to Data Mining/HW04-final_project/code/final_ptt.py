import jieba
import jieba.posseg as pseg
import numpy as np 
import pandas as pd
import csv
import matplotlib.pyplot as plt
from matplotlib import cm
import requests 
# 導入 BeautifulSoup 模組(module)：解析HTML 語法工具
import bs4

Month = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

def OutputCSV(df, Result):   
    Result = Result + '.csv'

    df_SAMPLE = pd.DataFrame.from_dict( df )
    df_SAMPLE.to_csv( Result  , index=False )
    
    print( '成功產出'+Result )

def Draw(fruits, nums) :
    plt.rc('font', family='Microsoft JhengHei')
    plt.rcParams['axes.unicode_minus'] = False
    cmap = cm.jet(np.linspace(0, 1, len(nums)))
    x = np.arange(len(fruits))
    y = np.arange(len(map(int, nums)))
    plt.bar(x, nums, color=cmap)
    plt.xticks(x, fruits)
    plt.yticks(y, nums)
    plt.xlabel('fruit')
    plt.ylabel('num')
    plt.title('各月份最高討論的水果及其討論數')
    plt.savefig('統計表.png', transparent=False, bbox_inches='tight', pad_inches=1)

def Statistics() :
    df = pd.read_csv('result.csv', encoding = 'utf-8').astype(str)
    last_month = 1
    fruit = ""
    fruits = []
    num = 0
    nums = []
    for i in range(len(df)) :
        # print(i)
        month = int(df.loc[i, "month"])
        if month != last_month :
            last_month = month
            fruits.append(fruit)
            nums.append(num)
            
        fruit = df.loc[i, "fruit"]
        num = df.loc[i, "num"]

    fruits.append(fruit)
    nums.append(num)
    print(fruits)
    print(nums)
    fruit_list = {
        "fruit": fruits,
        "num": nums
    }
 
    new_df = pd.DataFrame(fruit_list)
    new_df.index = new_df.index + 1
    print(new_df)
    OutputCSV(new_df, "total")
    Draw(fruits, nums)

def Split_Key_Word(content) :
    nouns = []
    num = []
    words = pseg.cut(content)
    for word, flag in words:
        # print(word, flag)
        if flag == "n" :
            try:
                idx = nouns.index(word)
                num[idx] += 1
            except:
                nouns.append(word)
                num.append(1)

    try :
        max_value = max(num)
        idx = num.index(max_value)
        # print(nouns[idx])
        return nouns[idx]
    except:
        return

def OutputCSV(df, Result):   
    Result = Result + '.csv'

    df_SAMPLE = pd.DataFrame.from_dict( df )
    df_SAMPLE.to_csv( Result  , index=False )
    
    print( '成功產出'+Result )

def Read_Txt() :
    f = open('fruit.txt', encoding="utf-8")
    text = []
    for line in f:
        text.append(line.rstrip())
    print(text)
    return text

def Main(df) :
    fruits = []
    num = []
    months = []
    text = Read_Txt()
    for i in range(len(df)):
        print(i)
        fruit = Split_Key_Word(df.loc[i, "內容"])
        if fruit :
            try:
                idx = fruits.index(fruit)
                num[idx] += 1
            except:
                if fruit in text :
                    fruits.append(fruit)
                    num.append(1)
                    slice1 = slice(4, 7)
                    month = df.loc[i, "日期"]
                    m = Month.index(month[slice1])
                    months.append(m+1)

    fruit_list = {
        "fruit": fruits,
        "month": months,
        "num": num
    }
 
    new_df = pd.DataFrame(fruit_list)
    new_df.sort_values(by=['month','num'], ignore_index=True, inplace=True)
    print(new_df)
    OutputCSV(new_df, "result")
    Statistics()

def Statistics() :
    df = pd.read_csv('result.csv', encoding = 'utf-8').astype(str)
    last_month = 1
    fruit = ""
    fruits = []
    num = 0
    nums = []
    for i in range(len(df)) :
        # print(i)
        month = int(df.loc[i, "month"])
        if month != last_month :
            last_month = month
            fruits.append(fruit)
            nums.append(num)
            
        fruit = df.loc[i, "fruit"]
        num = df.loc[i, "num"]

    fruits.append(fruit)
    nums.append(num)
    print(fruits)
    print(nums)
    fruit_list = {
        "fruit": fruits,
        "num": nums
    }
 
    new_df = pd.DataFrame(fruit_list)
    new_df.index = new_df.index + 1
    print(new_df)
    OutputCSV(new_df, "total")

def Read_CSV() :
    with open('output.csv', 'r', encoding="utf-8") as csvfile:
        # 讀取 CSV 檔案內容
        rows = csv.reader(csvfile)

        # 以迴圈輸出每一列
        for row in rows:
            print(row)

def Write_CSV(author, board, title, date, content) :
    # 開啟輸出的 CSV 檔案
    with open('output.csv', 'a+', encoding='UTF-8', newline='') as csvfile:
        # 建立 CSV 檔寫入器
        writer = csv.writer(csvfile)

        # 寫入另外幾列資料
        writer.writerow([author, board, title, date, content])

def Crawler_content(URL) :
    # 設定Header與Cookie
    my_headers = {'cookie': 'over18=1;'}
    # 發送get 請求 到 ptt 八卦版
    response = requests.get(URL, headers = my_headers)


    #  把網頁程式碼(HTML) 丟入 bs4模組分析
    soup = bs4.BeautifulSoup(response.text,"html.parser")

    ## PTT 上方4個欄位
    header = soup.find_all('span','article-meta-value')
    try :
        # 作者
        author = header[0].text
        # 看版
        board = header[1].text
        # 標題
        title = header[2].text
        # 日期
        date = header[3].text


        ## 查找所有html 元素 抓出內容
        main_container = soup.find(id='main-container')
        # 把所有文字都抓出來
        all_text = main_container.text
        # 把整個內容切割透過 "-- " 切割成2個陣列
        pre_text = all_text.split('--')[0]
            
        # 把每段文字 根據 '\n' 切開
        texts = pre_text.split('\n')
        # 如果你爬多篇你會發現 
        contents = texts[2:]
        # 內容
        content = '\n'.join(contents)

        # 顯示
        '''
        print('作者：'+author)
        print('看板：'+board)
        print('標題：'+title)
        print('日期：'+date)
        print('內容：'+content)
        '''

        Write_CSV(author, board, title, date, content)
    except:
        pass

def Get_link(URLs) :
    #目標網站
    URL = "https://www.ptt.cc/bbs/fruits/index300.html" 
    for page in range(1, 100):
        print(page)
        # 設定Header與Cookie
        my_headers = {'cookie': 'over18=1;'}
        #請求網站
        r = requests.get(URL, headers = my_headers)
        #檢查回應。如果是200則成功請求

        #透過BeautiFul整理且用html.parser解析
        root = bs4.BeautifulSoup(r.text,"html.parser")
        #找到所有屬性class = "m-ent"
        ment = root.find_all("div",class_="title")
        #一個一個印出要的資料
        for title in ment:
            try :
                str = title.a.string
            except :
                str = title.string
            
            if SplitTitle(str) :
                # print(str)#取得文章標題
                # print("https://www.ptt.cc"+title.a.get("href"))#取得文章連結
                URLs.append("https://www.ptt.cc"+title.a.get("href"))

        # 抓取下一頁連結
        btn = root.select('div.btn-group > a')
        up_page_href = btn[3]['href']
        next_page_url = 'https://www.ptt.cc' + up_page_href
        URL = next_page_url

    return URLs

def SplitTitle(title):
    if "本文已被刪除" in title:
        print("本文已被刪除")
        return
    if "[" not in title:
        print("[")
        return
    if "]" not in title:
        print("]")
        return
    if "已被ubcs刪除" in title:
        print("已被ubcs刪除")
        return

    a = title.index("[") + 1
    b = title.index("]")

    category = title[a:b].strip()
    title = title[b + 1 :].strip()

    return title

if __name__ == '__main__':
    URLs = []
    URLs = Get_link(URLs)
    if URLs :
        with open('output.csv', 'w', encoding="utf-8", newline='') as csvfile:
            # 建立 CSV 檔寫入器
            writer = csv.writer(csvfile)
            # 寫入一列資料
            writer.writerow(['作者', '看板', '標題', '日期', '內容'])

        for url in URLs :
            print(url)
            Crawler_content(url)

    # Read_CSV()
    df = pd.read_csv('output.csv', encoding = 'utf-8').astype(str)
    df.drop(['作者', '看板', '標題'], inplace=True, axis=1)
    # print(df)
    Main(df)
    Statistics()
