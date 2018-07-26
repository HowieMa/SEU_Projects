# -*- coding: utf-8 -*-
"""
Created on Thu Jun 14 19:47:42 2018
散点图
@author: mhy
"""
import requests
import json
headers={       
'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36',       
}

##北京
lat1=39.820735
lng1=116.216091

lat2=40.024808
lng2=116.548392


##淮南市城区
#lat1=32.573204
#lng1=116.804016
#
#lat2=32.722048
#lng2=116.986264

#计算地图中心
center_lat=(lat1+lat2) /2
center_lng=(lng1+lng2) /2

def coords_transfer(lng,lat):
    coords=str(lng)+','+str(lat)

    #坐标转换API
    url='http://api.map.baidu.com/geoconv/v1/?coords='+coords+'&from=6&to=5&ak=X3jQGUexW0e6SpgEuGOEBPByeDA2ttm3'
    try:
        coords_req=requests.get(url=url,headers=headers,timeout=3)
        xy=coords_req.text

        json_dict=json.loads(xy)
        lng = json_dict['result'][0]['x']
        lat = json_dict['result'][0]['y']   
        return lng,lat
    except:
        return 0,0


fin=open('shop_jw_beijing.txt','r')
values = fin.readlines()
fin.close()

fout1=open('GPSjw_bj.txt','w')
point_list=[]
num=0
for value in values:
    print(num)
    num=num+1
    xy=value.split(',')
    lng=xy[0][xy[0].index(':')+1:len(xy[0])]
    lat=xy[1][xy[1].index(':')+1:len(xy[1])-1]
    lng,lat=coords_transfer(lng,lat)
    point_list.append ([lng,lat])
    fout1.write(str(lng)+'\t'+str(lat)+'\n')
    


fout1.close()
  

ak='XeFrQYFhgsqs3WzdHCV0WvS4NyZmWlRZ'

val='var data = {"data":' +str(point_list) +'}'
page='''

<!DOCTYPE HTML>
<html>
<head>
  <title>加载海量点</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=no">
  <style type="text/css">
    html,body{
        margin:0;
        width:100%;
        height:100%;
        background:#ffffff;
    }
    #map{
        width:100%;
        height:100%;
    }
    #panel {
        position: absolute;
        top:30px;
        left:10px;
        z-index: 999;
        color: #fff;
    }
    #login{
        position:absolute;
        width:300px;
        height:40px;
        left:50%;
        top:50%;
        margin:-40px 0 0 -150px;
    }
    #login input[type=password]{
        width:200px;
        height:30px;
        padding:3px;
        line-height:30px;
        border:1px solid #000;
    }
    #login input[type=submit]{
        width:80px;
        height:38px;
        display:inline-block;
        line-height:38px;
    }
  </style>
  <script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak='''+ ak +'''"></script>
    <script type="text/javascript" src=""></script>
</head>
<body>
    <div id="map"></div>
    <script type="text/javascript">
    '''+val+'''
    var map = new BMap.Map("map", {});                        // 创建Map实例
    
    var point = new BMap.Point(''' + str(center_lng) +','+ str(center_lat) +''');
    
    map.centerAndZoom(point, 11);     // 初始化地图,设置中心点坐标和地图级别
    map.enableScrollWheelZoom();                        //启用滚轮放大缩小
    if (document.createElement('canvas').getContext) {  // 判断当前浏览器是否支持绘制海量点
        var points = [];  // 添加海量点数据
        for (var i = 0; i < data.data.length; i++) {
          points.push(new BMap.Point(data.data[i][0], data.data[i][1]));
        }
        var options = {
            size: BMAP_POINT_SIZE_SMALL,
            shape: BMAP_POINT_SHAPE_STAR,
            color: '#d340c3'
        }
        var pointCollection = new BMap.PointCollection(points, options);  // 初始化PointCollection
        pointCollection.addEventListener('click', function (e) {
          alert('单击点的坐标为：' + e.point.lng + ',' + e.point.lat);  // 监听点击事件
        });
        map.addOverlay(pointCollection);  // 添加Overlay
    } else {
        alert('请在chrome、safari、IE8+以上浏览器查看本示例');
    }
  </script>
</body>
</html>

'''

fout=open('scatter_bj.html','w',encoding="utf-8")
fout.write(page)
fout.close()


















