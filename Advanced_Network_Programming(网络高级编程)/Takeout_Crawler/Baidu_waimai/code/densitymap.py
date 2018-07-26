# -*- coding: utf-8 -*-
"""
Created on Thu Jun 14 19:47:42 2018
密度图
@author: mhy
"""
import math
import pandas as pd


##淮南市城区
lat1=32.573204
lon1=116.804016

lat2=32.722048
lon2=116.986264
#

##北京
lat1=39.820735
lon1=116.216091

lat2=40.024808
lon2=116.548392

#计算地图中心
center_lat=(lat1+lat2) /2
center_lng=(lon1+lon2) /2

ak='XeFrQYFhgsqs3WzdHCV0WvS4NyZmWlRZ'

#读取所有商家坐标
fin = open('GPSjw_bj.txt','r')

shop_infos=fin.readlines()

res=[]
for shop in shop_infos:
    shop = shop.split('\t')
    x=float(shop[0])
    y=float(shop[1])
    res.append([x,y])

df=pd.DataFrame(res)
df.rename(columns={0:'lon',1:'lat'},inplace=True)

#根据首尾坐标生成每个矩形的顶点坐标
def getRectangle(lat1,lon1,lat2,lon2,step=2):
    step=2
    step_lon = step / (111.0)
    step_lat = step / (111.0) * math.cos(math.radians(lat1))
    boxes=[] #c矩形4个点
    lon_left=lon1
    lon_right=lon_left+step_lon    
    while (lon_right < lon2):
        lat_left = lat1
        lat_right = lat_left + step_lat
        while (lat_right < lat2):    
            find=df.loc[ (df['lat']<lat_right) & (df['lat']>lat_left)
                       & (df['lon']<lon_right)  &   (df['lon']>lon_left) ]
            count=len(find)
            box=[lat_left, lon_left, lat_right, lon_right, count]
            boxes.append(box) #矩形坐标  
            lat_left=lat_right
            lat_right=lat_right+step_lat    
        lon_left=lon_right
        lon_right=lon_right+step_lon  
        
    df_box=pd.DataFrame(boxes)
    name_dict={0:'lat_left',1:'lon_left',2:'lat_right',3:'lon_right',4:'count'}
    df_box.rename(columns=name_dict,inplace=True)
    #统计四分位点，用于生成不同的颜色
    v1 = df_box['count'].quantile(0.25)
    v2 = df_box['count'].quantile(0.5)
    v3 = df_box['count'].quantile(0.75)
    return df_box,v1,v2,v3

df_box,v1,v2,v3 = getRectangle(lat1,lon1,lat2,lon2,2)



rectanges=''
maps=''
num=0

for b in df_box.index:
    p=df_box.loc[b].values
    #根据count值的不同大小，生成不同的颜色
    if p[4]>v3:
        color='Red'
    
    elif (p[4] <=v3) & (p[4] >v2) :
        color='orange'
    elif (p[4] <=v2) & (p[4] >v1) :
        color='yellow'
    elif p[4]<=v1:
        color='white'
        
    rectanges=rectanges+'''
      var pStart = new BMap.Point('''+ str(p[1])+',' + str(p[0]) +''');
    	var pEnd = new BMap.Point('''+str(p[3])+','+ str(p[2]) +''');
    	var rectangle'''+ str( num) +''' = new BMap.Polygon([
    		new BMap.Point(pStart.lng,pStart.lat),
    		new BMap.Point(pEnd.lng,pStart.lat),
    		new BMap.Point(pEnd.lng,pEnd.lat),
    		new BMap.Point(pStart.lng,pEnd.lat)
    	], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5, fillColor:" '''+  color +'"});  '
    
    maps=maps+'map.addOverlay(rectangle'+ str( num) +'''); \n       '''
    num=num+1


page='''
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
	<style type="text/css">
		body, html{width: 100%;height: 100%;margin:0;font-family:"微软雅黑";}
		#allmap{height:500px;width:100%;}
		#r-result{width:100%;}
	</style>
	<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak='''+ak+'''"></script>
	<title>添加/删除覆盖物</title>
</head>
<body>
	<div id="allmap"></div>
	<div id="r-result">
		<input type="button" onclick="add_overlay();" value="添加覆盖物" />
		<input type="button" onclick="remove_overlay();" value="删除覆盖物" />
        红色 TOP 0-25%  橙色 TOP 25-50%  黄色 TOP 50-75%  白色 TOP 75-100% 
	</div>
</body>
</html>
<script type="text/javascript">
	// 百度地图API功能
	var map = new BMap.Map("allmap");
	var point = new BMap.Point('''+ str(center_lng) +','+ str(center_lat) +''');
	map.centerAndZoom(point, 11);
	map.enableScrollWheelZoom(); 

''' + rectanges +'''
	
	//添加覆盖物
	function add_overlay(){
        '''+maps+'''
	}
	//清除覆盖物
	function remove_overlay(){
		map.clearOverlays();         
	}
</script>

'''

fout=open('densitymap_beijing.html','w',encoding="utf-8")
fout.write(page)
fout.close()



