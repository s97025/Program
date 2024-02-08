import os
import tkinter as tk
import tkinter.ttk as ttk
from tkinter import filedialog
import win32com.client
from pywintypes import com_error


class Inface:

  def __init__(self, win):
    #window
    self.win = win
    #視窗標題
    self.win.title( "EXCEL_TO_PFD" ) 
    #視窗大小   
    self.win.geometry("900x600+300+100") # 寬*高
    self.win.resizable( False, False ) # 視窗不可縮放
    #透明度
    self.win.attributes( "-alpha", 1 ) # 1~0 1=100% 0=0%
    #置頂
    self.win.attributes( "-topmost", False ) 



    self.table_frame = tk.Frame()
    self.table_frame.pack()  

    #style 中間
    style = ttk.Style()
    style.theme_use( "default" )

    #scale 中間 
    self.yscroll = tk.Scrollbar( self.table_frame )
    self.yscroll.config( orient=tk.VERTICAL ) 
    self.yscroll.pack( side=tk.RIGHT, fill=tk.Y )

    #Label 中間 
    columns = ['輸入檔名', '輸出檔名', '成功/失敗']
    self.exetable = ttk.Treeview(
               master=self.table_frame,  # 父容器
               height=20,  # 表格显示的行数,height行
               columns=columns,  # 显示的列
               yscrollcommand = self.yscroll.set, 
               show='headings',  # 隐藏首列
               )
    self.exetable.heading( '輸入檔名', text='輸入檔名' )  # 定義標頭
    self.exetable.heading( '輸出檔名', text='輸出檔名' )  # 定義標頭
    self.exetable.heading( '成功/失敗', text='成功/失敗' )  # 定義標頭
    self.exetable.column( '輸入檔名', width=380, minwidth=350, anchor=tk.W )  # 定義列
    self.exetable.column( '輸出檔名', width=380, minwidth=350, anchor=tk.W )  # 定義列
    self.exetable.column( '成功/失敗', width=80, minwidth=80, anchor=tk.W )  # 定義列
    self.exetable.pack( pady = 10 )
    self.yscroll.config( command=self.exetable.yview ) 


    #frame 上方
    self.open_frame = tk.Frame()
    self.open_frame.config( height = 50, width = 50, bg = "silver" )
    self.open_frame.pack(side = tk.RIGHT, padx = 20)  


    #Button 上方
    self.btnChoose = tk.Button( self.open_frame )
    self.btnChoose.config( text="選擇文件", font="微軟正黑 12", width=8, command=self.OpenInput )
    self.btnChoose.grid( row=0, column=1, pady = 10, padx = 20 )
    
    self.btnOutputPlace = tk.Button( self.open_frame )
    self.btnOutputPlace.config( text="輸出位置", font="微軟正黑 12", width=8, command=self.OutputPath )
    self.btnOutputPlace.grid( row=1, column=1, pady = 5, padx = 20 )

    #table 上方
    self.EntChoose = tk.Entry( self.open_frame )
    self.EntChoose.config( bg = "white", width=50 ) 
    self.EntChoose.grid( row=0, column=0, pady = 5, padx = 20 )

    self.EntOutputPlace = tk.Entry( self.open_frame )
    self.EntOutputPlace.config( bg = "white", width=50 ) 
    self.EntOutputPlace.grid( row=1, column=0, pady = 5, padx = 20 )
    
    # 退出/轉換
    self.btnChange = tk.Button( self.open_frame )
    self.btnChange.config( text="開始轉換", font="微軟正黑 15", width=20, command=self.ExcelToPDF )
    self.btnChange.grid( row=3, column=0, pady = 10, padx = 0 )

    self.btnExit = tk.Button( self.open_frame )
    self.btnExit.config( text="退出", font="微軟正黑 12", width=8, command=self.Exit )
    self.btnExit.grid(  row=3, column=1, pady = 10, padx = 0 )  




    #frame 底
    self.text_frame = tk.Frame()
    self.text_frame.config( height = 50, width = 800, bg = "silver", bd = 8 )
    self.text_frame.pack(side = tk.LEFT, padx = 20)  
    
    self.labelSum = tk.Label( self.text_frame )
    self.labelSum.config( bg = "silver", width=8, font="微軟正黑 12", text= "文件總數: " , anchor=tk.W ) 
    self.labelSum.grid( row=0, column=0, pady = 10, padx = 0 )

    self.labelSucc = tk.Label( self.text_frame )
    self.labelSucc.config( bg = "silver", width=8, font="微軟正黑 12", text= "成功: ", anchor=tk.W ) 
    self.labelSucc.grid( row=1, column=0, pady = 10, padx = 0 )

    self.labelFail = tk.Label( self.text_frame )
    self.labelFail.config( bg = "silver", width=8, font="微軟正黑 12", text= "失敗: ", anchor=tk.W ) 
    self.labelFail.grid( row=2, column=0, pady = 10, padx = 0 )


    self.labelSumtxt = tk.Label( self.text_frame )
    self.labelSumtxt.config( bg = "gray95", width=18, font="微軟正黑 12" , anchor=tk.W ) 
    self.labelSumtxt.grid( row=0, column=1, pady = 10, padx = 20 )

    self.labelSucctxt = tk.Label( self.text_frame )
    self.labelSucctxt.config( bg = "gray95", width=18, font="微軟正黑 12", anchor=tk.W ) 
    self.labelSucctxt.grid( row=1, column=1, pady = 10, padx = 15 )

    self.labelFailtxt = tk.Label( self.text_frame )
    self.labelFailtxt.config( bg = "gray95", width=18, font="微軟正黑 12", anchor=tk.W ) 
    self.labelFailtxt.grid( row=2, column=1, pady = 10, padx = 15 )



  def Clear(self):
    self.EntChoose.delete( 0, tk.END )
    self.EntOutputPlace.delete( 0, tk.END )
    self.exetable.delete( *self.exetable.get_children() )
    self.labelSumtxt.config( text="" )
    self.labelSucctxt.config( text="" )
    self.labelFailtxt.config( text="" )

  def Exit(self):
    self.win.destroy()

  def OpenInput(self):
    
    file_path=[]
    for name in filedialog.askopenfilenames() :
      file_path.append( name.replace('/', '\\') )# 選擇檔案後回傳檔案路徑與名稱

    if ( len(file_path) != 0 ):
      self.Clear()
      split_file = ""
      count = 0 # 條紋

      self.EntChoose.insert ( 0, file_path ) 
      
      self.exetable.tag_configure( 'oddrow' , background="white" )
      self.exetable.tag_configure( 'evenrow' , background="lightblue" )
      for i in file_path:
        if count % 2 == 0:
          self.exetable.insert( parent="", index = 'end', iid=i, text="", values=(i, "", ""), tags=("evenrow",) )
        else:
          self.exetable.insert( parent="", index = 'end', iid=i, text="", values=(i, "", ""), tags=('oddrow',) )
        
        self.exetable.update()
        count+=1
      
      #split_file = split_file.split("\\")
      #self.EntOutputPlace.insert ( 0, split_file[0:len(split_file)-2])

      sum = str(len(file_path))
      self.labelSumtxt.config(text=sum )



  def OutputPath(self):
    outfile_path = filedialog.askdirectory() # 選擇資料夾回傳資料夾路徑與名稱
    if ( len(outfile_path) != 0 ):
      self.EntOutputPlace.delete( 0, tk.END )
      outfile_path = outfile_path.replace('/', '\\')
      self.EntOutputPlace.insert ( 0, outfile_path )

  def ExcelToPDF(self):

    output_folder = self.EntOutputPlace.get()
    lineValues = self.exetable.get_children()
    
    failed = 0
    success = 0

    for i in lineValues:
      InputFile = self.exetable.item(i,'values')[0]
      outputFile = ""
      true_False = ""
      
      filename = InputFile.split('.')
      if filename[len(filename)-1] != "xls" and  filename[len(filename)-1] != "xlsx":
        outputFile = ""
        true_False = "失敗"
        failed += 1
      else:
        name = filename[0].split('\\')
        outputFile = output_folder + "\\" + name[len(name)-1] + ".pdf"     
        if os.path.exists(outputFile):
          os.remove(outputFile) 
        else:
          pass

        try: 
          excel = win32com.client.Dispatch("Excel.Application")
          excel.Visible = False
          excel.interactive = False
          excel.DisplayAlerts = False ## 不警告
          x2p = excel.Workbooks.Open(InputFile, None, True)
          x2p.WorkSheets(1).Select()     
          x2p.ActiveSheet.ExportAsFixedFormat(0, outputFile) 


        except com_error as e:
          outputFile = ""
          true_False = "失敗"
          failed += 1
        else:
          true_False = "成功"
          success += 1
        finally:
          x2p.Close(SaveChanges=0)    
          excel.Quit() 
          del excel

      self.exetable.set(i, column='輸出檔名', value=outputFile )
      self.exetable.set(i, column='成功/失敗', value=true_False )
      self.exetable.see(i)
      #self.exetable.selection()
      self.exetable.update()

    
    self.labelFailtxt.config( text=str(failed) ) 
    self.labelSucctxt.config( text=str(success) )    
    


def main():
  win = tk.Tk()
  inface = Inface(win)
  win.mainloop()


  
 

if __name__ == '__main__':
  main()