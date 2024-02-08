import  jpype     
import  asposecells     

jpype.startJVM() 
from asposecells.api import Workbook
workbook = Workbook("C:\Users\user\Desktop\1111\電機系打工.xlsx")
workbook.save("C:\Users\user\Desktop\1111電機系打工.pdf")
jpype.shutdownJVM()