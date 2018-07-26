import tkinter as tk
from crack import PoJie
from tkinter import *
import time  #时间
import tkinter.messagebox

class GUI():
    def __init__(self):
        self.pj = PoJie()
        self.window = tk.Tk(screenName='wifi破解器')
        self.nameLabel = tk.Label(self.window, text="wifi暴力破解器")

        self.button1 = tk.Button(self.window, text="搜索wifi", command=self.start_buttonClick)
        self.button2 = tk.Button(self.window, text="开始破解", command=self.search_Click)
        self.listb1 = tk.Listbox(self.window, selectmode=SINGLE, width=20, height=20)
        self.listb1.bind('<Double-Button-1>', self.SelectSsid)

        self.listb2 = tk.Listbox(self.window, width=10, height=20)
        self.listb3 = tk.Listbox(self.window, width=20, height=20)
        #  创建两个列表组件
        self.nameLabel.pack()
        self.button1.pack()
        self.button2.pack()

        self.listb1.pack(side=LEFT)
        self.listb2.pack(side=RIGHT)
        self.listb3.pack(side=RIGHT)
        self.window.mainloop()

        self.ssid = None
    def start_buttonClick(self):

        self.listb1.delete(0,END)
        self.listb2.delete(0, END)
        self.listb3.delete(0, END)
        wifi_list = self.pj.scan()
        for item in wifi_list:
            #result = item[0]+'...........'+str(item[1])
            self.listb1.insert(END, item[0])
            self.listb2.insert(END,str(item[1]))
            self.listb3.insert(END, str(item[2]))

    def SelectSsid(self,event):
        currentssid = self.listb1.get(self.listb1.curselection())
        print('current id is',currentssid,'XXX')

        res = tkinter.messagebox.askyesno("FishC Demo", "确定要破解吗？")
        print(res)
        if res is True:
            self.ssid = currentssid
            print('ssid已经修改为',self.ssid)

        else:
            self.ssid = None


    def search_Click(self):
        if self.ssid is None:
            tkinter.messagebox.showinfo('请先双击需要破解密码的WiFi名称！')
            return

        print('开始破解')
        key = self.pj.test_connect(self.ssid)
        tkinter.messagebox.showinfo('破解成功','密码是'+key)





gu=GUI()



