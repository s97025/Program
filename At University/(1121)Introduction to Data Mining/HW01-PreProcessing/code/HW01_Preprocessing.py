import numpy as np 
import pandas as pd
import csv
import matplotlib.pyplot as plt


def Sampling_company_chart(subgroup):
  plt.rc('font', family='Microsoft JhengHei')
  chart = subgroup.plot(kind='bar',  # 圖表類型
                        title='水泥工業每月營業收入彙總表',  # 圖表標題
                        xlabel='公司名稱',  # x軸說明文字
                        ylabel='營收數值',  # y軸說明文字
                        legend=True,  # 是否顯示圖例
                        figsize=(10, 5))  # 圖表大小 

def Sampling_company(df):

  subgroup = df.copy()
  subgroup.dropna()

  # delete unneeded table
  subgroup.drop(['出表日期', '資料年月', '公司代號','營業收入-上月比較增減(%)', '營業收入-去年同月增減(%)', '累計營業收入-前期比較增減(%)','備註'], inplace=True, axis=1)

  # chart
  print('資料量大請耐心等候...(做出全部資料圖表)')  
  Sampling_company_chart(subgroup)

  plt.savefig('上市公司每月營業收入彙總表.png', transparent=False, bbox_inches='tight', pad_inches=1)
  print('上市公司每月營業收入彙總表.png 儲存完成')

  # Sampling
  subgroup = subgroup.set_index('公司名稱', drop=True)
  subgroup = subgroup.groupby(['產業別'])
  subgroup = subgroup.get_group('水泥工業')

  Sampling_company_chart(subgroup)
  plt.savefig('Sampling_水泥工業每月營業收入圖表.png', transparent=False, bbox_inches='tight', pad_inches=1)
  print('Sampling_水泥工業每月營業收入圖表.png 儲存完成')
  # plt.show()

  # write file
  subgroup.to_csv('Sampling_水泥工業每月營業收入.csv',encoding='big5')
  print('Sampling_水泥工業每月營業收入.csv 儲存完成')

def Aggregation_company(df):

  subgroup = df.copy()
  subgroup.dropna()

  # delete unneeded table
  subgroup.drop(['出表日期', '資料年月', '公司代號', '公司名稱','營業收入-上月比較增減(%)', '營業收入-去年同月增減(%)', '累計營業收入-前期比較增減(%)','備註'], inplace=True, axis=1)
   
  # Aggregation
  pd.set_option('display.float_format', lambda x : '%.3f' % x ) # 取道小數點後3位
  subset_mean = subgroup.groupby('產業別').mean()
  
  subset_mean = subset_mean.drop(['其他', '其他電子業'], axis = 0)
  subset_std = (subset_mean - subset_mean.min()) / (subset_mean.max() - subset_mean.min())
  subset_std = subset_std.sort_values(by='營業收入-當月營收', ascending=False )

  # 刪除離群值
  condition1 = (subset_std['營業收入-當月營收'] != 1.000) & (subset_std['營業收入-當月營收'] != 0.000) 
  subset_std = subset_std[condition1]

  # draw Grap
  plt.rc('font', family='Microsoft JhengHei')
  chart = subset_std.plot(kind='bar',  # 圖表類型
                          title='上市公司每月營業收入彙總表',  # 圖表標題
                          xlabel='產業別',  # x軸說明文字
                          ylabel='標準化後營收數值',  # y軸說明文字
                          legend=True,  # 是否顯示圖例
                          figsize=(10, 5))  # 圖表大小

  plt.savefig('Aggregation_各產業每月營業收入圖表.png', transparent=False, bbox_inches='tight', pad_inches=1)
  print('Aggregation_各產業每月營業收入圖表.png 儲存完成')
  # plt.show()

  # write file
  subset_std.to_csv('Aggregation_各產業每月營業收入.csv', encoding = 'big5')
  print('Aggregation_各產業每月營業收入.csv 儲存完成' )

def Sampling_cemetery(df):

  subgroup = df.copy()
  subgroup = subgroup.dropna()

  # delete unneeded table
  subgroup.drop(['鄉鎮市區', '地址', '經度','緯度', '聯絡電話','管轄所屬'], inplace=True, axis=1)
    
  # Sampling
  subgroup = subgroup.set_index('名稱')  
  subgroup = subgroup.groupby(['縣市'])
  subgroup = subgroup.get_group('基隆市')  
  # print(subgroup)

  subgroup['數量'] = 1
  # print(subgroup)

  
  plt.rc('font', family='Microsoft JhengHei')
  chart = subgroup.groupby(['類別']).sum().plot(kind='pie', y='數量', autopct="%1.0f%%", startangle=30) 

  plt.savefig('Sampling_殯葬設施_基隆市.png', transparent=False, bbox_inches='tight', pad_inches=1)
  print('Sampling_殯葬設施_基隆市.png 儲存完成')
  # plt.show()

  # write file
  subgroup.to_csv('Sampling_殯葬設施_基隆市.csv', encoding = 'big5')
  print('Sampling_殯葬設施_基隆市.csv 儲存完成')

def Aggregation_cemetery(df):
  subgroup = df.copy()
  subgroup = subgroup.dropna()

  # delete unneeded table
  subgroup.drop(['鄉鎮市區', '地址', '聯絡電話','管轄所屬'], inplace=True, axis=1)
  subgroup = subgroup.set_index('名稱') 
  # draw Grap
  plt.rc('font', family='Microsoft JhengHei')
  subgroup.plot.scatter(x="經度", y="緯度") 

  plt.savefig('殯葬設施_北北基.png', transparent=False, bbox_inches='tight', pad_inches=1)
  print('殯葬設施_北北基.png 儲存完成')

  # Aggregation
  subgroup.plot.scatter(x="經度", y="緯度")  # 圖表大小

  subgroup['經度'] = (subgroup['經度'] - subgroup['經度'].min()) / (subgroup['經度'].max() - subgroup['經度'].min())
  subgroup['緯度'] = (subgroup['緯度'] - subgroup['緯度'].min()) / (subgroup['緯度'].max() - subgroup['緯度'].min())
  subgroup = subgroup.sort_values(by='經度', ascending=False )

  #print(subgroup)

  # draw Grap
  plt.rc('font', family='Microsoft JhengHei')
  subgroup.plot.scatter(x="經度", y="緯度")  # 圖表大小

  plt.savefig('Aggregation_殯葬設施_北北基.png', transparent=False, bbox_inches='tight', pad_inches=1)
  print('Aggregation_殯葬設施_北北基.png 儲存完成')
  # plt.show()

  # write file
  subgroup.to_csv('Aggregation_殯葬設施_北北基.csv', encoding = 'big5')
  print('Aggregation_殯葬設施_北北基.csv 儲存完成')



if __name__=="__main__":

  while(True):
    print("\n")
    print("0 or others. exit")
    print("1.上市公司每月營業收入彙總表")
    print("2.殯葬設施_北北基")
    print("=============================")
    num = input("Input num (0,1,2): ")

    if num == '1' :
      df = pd.read_csv('上市公司每月營業收入彙總表.csv', encoding = 'utf-8')
      Sampling_company(df)
      Aggregation_company(df)
    elif num == '2':
      df = pd.read_csv('殯葬設施_北北基.csv', encoding = 'utf-8')
      Sampling_cemetery(df)
      Aggregation_cemetery(df)  
    else: 
      exit()

