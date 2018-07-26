# -*- coding: utf-8 -*-
"""
Created on Wed Jun 13 21:47:09 2018
baidu
@author: mhy
"""

import requests
import re
import json
import time
import random
import math



#百度坐标拾取器获得 北京城区

#lat1=39.820735
#lng1=116.216091
#
#lat2=40.024808
#lng2=116.548392

#淮南市城区
lat1=32.573204
lng1=116.804016

lat2=32.722048
lng2=116.986264



all_shop_id=[]


headers={       
'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36',       
}

#批量生成坐标
def genPositon(lat1,lng1,lat2,lng2,step=1):
    #step单位是km，1经度度约等于111km
    step_lng = step / (111.0)
    step_lat = step / (111.0) * math.cos(math.radians(lat1))

    pos_list = []

    cur_lon = lng1 + step_lng / 2
    while (cur_lon < lng2):
        cur_lat = lat1 + step_lat / 2
        while (cur_lat < lat2):
            print([cur_lat, cur_lon])
            pos_list.append([cur_lat, cur_lon])
            cur_lat = cur_lat + step_lat

        cur_lon = cur_lon + step_lng
    return pos_list


def coords_transfer(lng,lat):
    coords=str(lng)+','+str(lat)

    #坐标转换API
    url='http://api.map.baidu.com/geoconv/v1/?coords='+coords+'&from=5&to=6&ak=X3jQGUexW0e6SpgEuGOEBPByeDA2ttm3'
    coords_req=requests.get(url=url,headers=headers)
    
    xy=coords_req.text

    json_dict=json.loads(xy)
    lng = json_dict['result'][0]['x']
    lat = json_dict['result'][0]['y']   
    return lng,lat


def getURL(lng,lat):
    url='http://waimai.baidu.com/waimai'
    data={
    'qt': 'shoplist',
    'lat': lat,
    'lng': lng,
    }

    req=requests.get(url=url,params=data,headers=headers,allow_redirects=False)
    #发生重定向
    new_url=''
    if req.status_code==302:
        location=req.headers['Location']
        new_url='http://waimai.baidu.com'+location
    return new_url


fjw=open('shop_jw_huainan.txt','w')
#获取当前搜索中心所有商家信息
def count_shops(new_url):
    count=0
    allshops=[]
    shops=[]
    num=1 
    
    while( (len(shops)!=0) or (num==1)):
        time.sleep(0.2+random.random())
        cur_url = new_url + '?display=json&page='+str(num)+'&count=40'
        print('URL: ' + cur_url)
        try:
            req=requests.get(url=cur_url,headers=headers,timeout=3)
            shops=re.findall('"shop_id":"[0-9]+"',req.text,re.S)
            latlngs=re.findall('"shop_lng":[0-9]+,"shop_lat":[0-9]+',req.text,re.S)
        except:
            print('wrong')
            
        k=0
        for shop in shops:
            sid=shop.split(':')[1]
            sid=sid[1:len(sid)-1]
            
            if sid not in allshops:
                allshops.append(sid) 
            #如果不在所有商家列表中
            if sid not in all_shop_id:
                all_shop_id.append(sid)
                s=sid+'\t'+latlngs[k]+'\n'
                
                print('!!!! new SHOP ID '+s)
                fjw.write(s)
            k=k+1
               
        num=num+1
    count=len(allshops) #该经纬度商家计数
    return count


fout=open('format_count_huainan.txt','w')
coords=genPositon(lat1,lng1,lat2,lng2,2)
print('\ntotal coord is'+ str(len(coords)))
kk=1
for (lat,lng) in coords: 
    print('\n\n-----current latitude and longitude is')
    print(lat)
    print(lng)  
    print('Number '+str(kk))
    lng1,lat1=coords_transfer(lng,lat)
    new_url = getURL(lng1,lat1)
    count=count_shops(new_url)
    print(count)
    s='{"lng":'+str(lng)+',"lat":'+str(lat)+',"count":'+str(count)+'},'
    print('\n'+s)
    fout.write(s)
    kk=kk+1

fout.close()
fjw.close()

