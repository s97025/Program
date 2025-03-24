from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from time import sleep
from selenium import webdriver

textTitles = []
textContents = []

def Crawler_Content(urls, driver_content) :
    for u in urls :
        # 前往網頁
        driver_content.get(u)
        try:
            print(u)
            # 等待文章元素出現
            wait = WebDriverWait(driver_content, 10)
            articles = wait.until(EC.presence_of_all_elements_located((By.TAG_NAME, "article")))

            # 提取文章的連結
            # urls = [article.find_element(By.TAG_NAME, "a").get_attribute("href") for article in articles]
            textContent = [article.find_element(By.TAG_NAME, "div").get_attribute("textContent") for article in articles]
            print(textContent)
            textContents.append(textContent)
            
        except Exception as e:
            print("錯誤:", e)
    

def Crawler_Title(driver_title):
    try:
        # 等待文章元素出現
        wait = WebDriverWait(driver_title, 10)
        articles = wait.until(EC.presence_of_all_elements_located((By.TAG_NAME, "article")))

        # 提取文章的連結
        urls = [article.find_element(By.TAG_NAME, "a").get_attribute("href") for article in articles]
        textTitle = [article.find_element(By.TAG_NAME, "a").get_attribute("textContent") for article in articles]  
        print(urls)
        print(textTitle)
        textTitles.append(textTitle)
        Crawler_Content(urls, driver_title)
        
        
    except Exception as e:
        print("錯誤:", e)

# 自動登入
def Signup(driver):   
  driver_title.get("https://www.dcard.tw/signup")
  email = "s97025@gmail.com"
  password = "s97025@a"
  email_input = driver.find_element(By.ID, "email")
  password_input = driver.find_element(By.ID, "password")
  login_Btn = driver.find_element(By.XPATH, "/html/body/div[1]/div/div[2]/div/div[2]/div[1]/form/div/button")
  email_input.send_keys(email)
  sleep(2)
  password_input.send_keys(password)
  sleep(5)
  login_Btn.send_keys(Keys.ENTER)

if __name__ == '__main__':
    # 鎖定使用相同使用者
    '''
    option = webdriver.ChromeOptions()
    option.add_argument('--user-data-dir=C:/Users/wukai/AppData/Local/Microsoft/Edge/User Data/Default') 
    driver_title = webdriver.Chrome(options=option)
    '''
    driver_title = webdriver.Chrome()
    # 前往網頁
    driver_title.get("https://www.dcard.tw/f/cycu")
    sleep(10)
    Signup(driver_title)
    
    # 爬文章
    for i in range(1):
      # 等待頁面載入
      Crawler_Title(driver_title)
      driver_title.forward() #下一頁
      sleep(5)

    # 關閉瀏覽器
    driver_title.quit()
   