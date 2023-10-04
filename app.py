import serial
import tkinter as tk
import re
from pyparsing import col  # Not used in the provided code

class SerialReader:
    def __init__(self, port):
        self.ser = serial.Serial(port, baudrate=115200, timeout=1)

    def read(self):
        return str(self.ser.readline())

    def write(self, data):
        self.ser.write(data)

class WaterOMatic:
    def __init__(self, root, serial_reader):
        self.root = root
        self.serial_reader = serial_reader
        self.create_widgets()

    def create_widgets(self):
        self.root.title('Water-O-Matic')

        self.auto_enabled = tk.IntVar()
        self.clicked = tk.StringVar(value="1")

        self.labels = {
            'Temperature': (tk.Label(self.root, text="Current Temperature: "), tk.Label(self.root, text="23")),
            'Humidity': (tk.Label(self.root, text="Current Humidity: "), tk.Label(self.root, text="45")),
            'Water': (tk.Label(self.root, text="Water Reading: "), tk.Label(self.root, text="41"))
        }

        for i, (key, (label1, label2)) in enumerate(self.labels.items()):
            label1.grid(row=i, column=0)
            label2.grid(row=i, column=1)

        tk.Button(self.root, text='Update', command=self.reader).grid(row=3, column=0)
        tk.Button(self.root, text='Water Now', command=self.dispense).grid(row=4, column=0)

        tk.Checkbutton(self.root, text='Automatic Watering', variable=self.auto_enabled, onvalue=1, offvalue=0).grid(row=5, column=0)

        tk.Label(self.root, text="Auto water every ").grid(row=6, column=0)
        tk.OptionMenu(self.root, self.clicked, *["1", "2", "3", "4", "5", "6", "7"]).grid(row=6, column=1)
        tk.Label(self.root, text=" days").grid(row=6, column=2)

        self.root.geometry('350x350')

    def reader(self):
        self.update_labels(self.serial_reader.read())

    def dispense(self):
        self.serial_reader.write(b'a')

    def update_labels(self, input):
        print(input)
        for prefix, (_, label) in self.labels.items():
            if prefix[0].lower() in input:
                label.config(text=re.sub("[^0-9]", "", input))

def main():
    root = tk.Tk()
    serial_reader = SerialReader('/dev/tty.usbmodem11401')
    app = WaterOMatic(root, serial_reader)
    root.mainloop()

if __name__ == "__main__":
    main()
