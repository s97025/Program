# 載入使用的套件
import pandas as pd
import requests
from requests_html import HTML
import re
import json


# 檢視資料結構
ID = '254334386'
url = 'https://www.dcard.tw/f/cycu' 
#url = 'https://www.ptt.cc/bbs/NBA/index.html' 
headers = { 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36'}
# headers = { 'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.0.3 Safari/605.1.15' }
# 透過request套件抓下這個網址的資料
requ = requests.get(url, headers=headers)
# 初步檢視抓到的資料結構
if requ.status_code == 200:
    with open('output.html', 'w', encoding='utf-8') as f:
        f.write(requ.text)
    print("寫入成功!")
else:
    print("沒有抓到網頁!")



'''
# 將抓下來的資料轉為DataFrame
ID = '254334386'
url = url = 'https://www.dcard.tw/_api/posts/' + ID
requ = requests.get(url)

rejs = requ.json()
pd.DataFrame(
    data=
    [{'ID':rejs['id'],
      'title':rejs['title'],
      'content':rejs['content'],
      'excerpt':rejs['excerpt'],
      'createdAt':rejs['createdAt'],
      'updatedAt':rejs['updatedAt'],
      'commentCount':rejs['commentCount'],
      'forumName':rejs['forumName'],
      'forumAlias':rejs['forumAlias'],
      'gender':rejs['gender'],
      'likeCount':rejs['likeCount'],
      'reactions':rejs['reactions'],
      'topics':rejs['topics']}],
    columns=['ID','title','content','excerpt','createdAt','updatedAt','commentCount','forumName','forumAlias','gender','likeCount','reactions','topics'])

# 撰寫簡單的函數，透過輸入文章ID，就輸出文章的資料
'''

def Crawl(ID):
    link = 'https://www.dcard.tw/_api/posts/' + str(ID)
    requ = requests.get(link)
    rejs = requ.json()
    return(pd.DataFrame(
        data=
        [{'ID':rejs['id'],
          'title':rejs['title'],
          'content':rejs['content'],
          'excerpt':rejs['excerpt'],
          'createdAt':rejs['createdAt'],
          'updatedAt':rejs['updatedAt'],
          'commentCount':rejs['commentCount'],
          'forumName':rejs['forumName'],
          'forumAlias':rejs['forumAlias'],
          'gender':rejs['gender'],
          'likeCount':rejs['likeCount'],
          'reactions':rejs['reactions'],
          'topics':rejs['topics']}],
        columns=['ID','title','content','excerpt','createdAt','updatedAt','commentCount','forumName','forumAlias','gender','likeCount','reactions','topics']))

'''     
if __name__ == '__main__':
  # 嘗試使用撰寫出的函數，抓取編號231030181的文章
  Crawl(254122411)

  # 一次讀取100篇最熱門的文章
  url = 'https://www.dcard.tw/_api/posts?popular=true&limit=100'
  resq = requests.get(url)
  rejs = resq.json()
  df = pd.DataFrame()
  for i in range(len(rejs)):
      df = df.append(Crawl(rejs[i]['id']),ignore_index=True)
  print(df.shape)
  print(df)

  # 透過迴圈讀取10*100篇文章，若需讀取更多資料，可以將range(10)中的數值提升
  for j in range(10):
      last = str(int(df.tail(1).ID)) # 找出爬出資料的最後一筆ID
      url = 'https://www.dcard.tw/_api/posts?popular=true&limit=100&before=' + last
      resq = requests.get(url)
      rejs = resq.json()
      for i in range(len(rejs)):
          df = df.append(Crawl(rejs[i]['id']), ignore_index=True)
  print(df.shape)
  print(df)

  # 將資料存到桌面
  df.to_excel('Dcard.xlsx')
'''
