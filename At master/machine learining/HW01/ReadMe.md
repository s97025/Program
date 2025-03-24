# 房價預測 - 機器學習作業一

## 作業連結:
* 傳送門: https://www.kaggle.com/competitions/machine-learning-2024-iaii-regression

## 安裝流程

* 安裝套件
    使用Anaconda作為虛擬還境 (我使用此環境開發)
    ```
    conda install --yes --file requirements.txt
    ```

* 其他安裝
    * 預測關係圖，中文亂碼問題 (matplotlib)
        * 參考資料: https://reurl.cc/34gZVM
        * 修改matplotlib底下檔案
        * 刪除.matplotlib cache檔案
        * 把下載字體加入matplotlib底下的fonts\ttf資料夾
   
* 執行程式:
    * 開啟Vscode
        * 點開HW01.ipynb
        * 點擊上方 RUN All 
        * 預測結果: house_price_predictions_HW01.csv
    
* 參考資料:
    * conda 指令: https://hackmd.io/@CILS110/r1QNFeDYY
    * pandas 
        * df_xlsx讀寫檔:  https://cynthiachuang.github.io/How-to-Read-a-Xlsx-File-Using-the-Pandas-Library-in-Python/
        * drop 刪除缺失值: https://vocus.cc/article/66d3d653fd897800012694c3
        * drop 刪除值定列: https://www.cupoy.com/qa/collection/0000017705894B9C000000036375706F795F72656C656173654355/000001771507EE3B0000000A6375706F795F70726572656C656173654349/0000017A3CE59438000000146375706F795F72656C656173655155455354
    * 房價預測 
        * 簡易版: https://medium.com/@elvennote/kaggle-house-prices-advanced-regression-techniques%E6%88%BF%E5%83%B9%E9%A0%90%E6%B8%AC%E5%AF%A6%E4%BD%9C-14a988e663a0

 