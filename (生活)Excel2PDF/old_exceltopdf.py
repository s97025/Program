import os
import PySimpleGUI as sg
from pywintypes import com_error
import win32com.client

def xlsx2pdf(file_path):  

  file_name = file_path.split('.')[0]
  pdf_file = f'{file_name}.pdf'  

  excel = win32com.client.Dispatch("Excel.Application")
  excel.Visible = False
  excel.interactive = False
  excel.DisplayAlerts = False ## 不警告
  
  if os.path.exists(pdf_file):
      os.remove(pdf_file)

  try:
    x2p = excel.Workbooks.Open(file_path)
    # Specify the sheet you want to save by index. 1 is the first (leftmost) sheet.
    x2p.WorkSheets(1).Select()        
    # Save
    #x2p.save(pdf_file, SaveFormat.PDF) 
    #x2p.SaveAs(pdf_file, FileFormat = elFormatPDF)
    x2p.ActiveSheet.ExportAsFixedFormat(0, pdf_file)  
  except com_error as e:
      pdf_file = 'failed'
  else:
      pass
  finally:
      x2p.Close()
      x2p.Save()
      excel.Quit()   
      return pdf_file


  '''
  excel = win32com.client.Dispatch("Excel.Application")
  excel.Visible = False
  excel.interactive = False
  file_name = file_path.split('.')[0]
  pdf_file = f'{file_name}.pdf'   

  if os.path.exists(pdf_file):
      os.remove(pdf_file)

  x2p = excel.Workbooks.Open(file_path)
  # Specify the sheet you want to save by index. 1 is the first (leftmost) sheet.
  x2p.WorkSheets(1).Select()        
  # Save
  #x2p.save(pdf_file, SaveFormat.PDF) 
  #x2p.SaveAs(pdf_file, FileFormat = elFormatPDF)
  x2p.ActiveSheet.ExportAsFixedFormat(0, pdf_file)  
  x2p.Close()
  excel.Quit() 
  return pdf_file
  '''

def main():
    # 选择主题
    sg.theme('LightBlue5')
    # 设置窗口
    layout = [
        [sg.Text('xlsxTopdf', font=('微軟正黑', 12)), sg.Text('', key='filename', size=(10, 1), font=('微软正黑', 10), text_color='blue')],
    
        [sg.Output(size=(80, 10), font=('微軟正黑', 10))],
        [sg.FilesBrowse('選擇文件', key='file', target='filename'), sg.Button('開始轉換'), sg.Button('退出')]]
    # 创建窗口
    window = sg.Window("Python_excel轉PDF", layout, font=("微軟正黑", 15), default_element_size=(50, 1))    

    # 事件循环 
    while True:
        # 窗口的讀取，有兩個返回值（1.事件；2.值）
        event, values = window.read()
        print(event, values)

        if event == "開始轉換":
            # 單個文件
            if values['file'] and (values['file'].split('.')[1] == 'xlsx' or values['file'].split('.')[1] == 'xls') :
                filename = xlsx2pdf(values['file'])
                print('文件个数 ：1')
                if  filename != 'failed.' :
                    print('\n' + '轉换成功！' + '\n')
                    print('文件保存位置：', filename)
            # 多个文件
            elif values['file'] and (values['file'].split(';')[0].split('.')[1] == 'xlsx' or values['file'].split(';')[0].split('.')[1] == 'xls'):
                print('文件個数 ：{}'.format(len(values['file'].split(';'))))
                for f in values['file'].split(';'):
                    filename = xlsx2pdf(f)
                    if  filename != 'failed.' :
                        print('\n' + '轉換成功！' + '\n')
                        print('文件保存位置：', filename)
            else:              
                print('請選擇xlsx/xls格式的文件哦!')

        if event in (None, '退出'):
            break

    window.close()

main()

