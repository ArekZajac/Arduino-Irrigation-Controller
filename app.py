from pyparsing import col
import serial
from tkinter import *
import tkinter as tk
import time
import re

ser = serial.Serial('/dev/tty.usbmodem11401', baudrate = 115200, timeout = 1)
options = [
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7"
]

def dispense():
    ser.write(b'a')

def reader():
    input = str(ser.readline())
    print(input)
    if "t" in input:
        labelTemp2.config(text=re.sub("[^0-9]", "", input))
    if "h" in input:
        labelHum2.config(text=re.sub("[^0-9]", "", input))
    if "w" in input:
        labelWater2.config(text=re.sub("[^0-9]", "", input))
    
    input = str(ser.readline())
    print(input)
    if "t" in input:
        labelTemp2.config(text=re.sub("[^0-9]", "", input))
    if "h" in input:
        labelHum2.config(text=re.sub("[^0-9]", "", input))
    if "w" in input:
        labelWater2.config(text=re.sub("[^0-9]", "", input))
    
    input = str(ser.readline())
    print(input)
    if "t" in input:
        labelTemp2.config(text=re.sub("[^0-9]", "", input))
    if "h" in input:
        labelHum2.config(text=re.sub("[^0-9]", "", input))
    if "w" in input:
        labelWater2.config(text=re.sub("[^0-9]", "", input))

root = Tk()
root.title('Water-O-Matic')
autoEnabled = tk.IntVar();
clicked = StringVar()
clicked.set( "1" )

labelTemp1 = tk.Label(root, text="Current Temperature: ")
labelTemp1.grid(row=0, column=0)

labelTemp2 = tk.Label(root, text="23")
labelTemp2.grid(row=0, column=1)

labelHum1 = tk.Label(root, text="Current Humidity: ")
labelHum1.grid(row=1, column=0)

labelHum2 = tk.Label(root, text="45")
labelHum2.grid(row=1, column=1)

labelWater1 = tk.Label(root, text="Water Reading: ")
labelWater1.grid(row=2, column=0)

labelWater2 = tk.Label(root, text="41")
labelWater2.grid(row=2, column=1)

checker = tk.Button(root, text='Update', command=reader)
checker.grid(row=3, column=0)

btn_Manual = tk.Button(root, text='Water Now', command=dispense)
btn_Manual.grid(row=4, column=0)

check_Automatic = tk.Checkbutton(root, text='Automatic Watering', variable=autoEnabled, onvalue=1, offvalue=0)
check_Automatic.grid(row=5, column=0)

label1 = tk.Label(root, text="Auto water every ")
label1.grid(row=6, column=0)

drop = OptionMenu( root , clicked , *options )
drop.grid(row=6, column=1)

label2 = tk.Label(root, text=" days")
label2.grid(row=6, column=2)

root.geometry('350x350')
root.mainloop()