# Auto Warehouse System

## Team
> * 육군 17사단 정보보호병 상병 방준식
> * 육군 20사단 통신병 병장 김민욱

## 프로젝트 소개
> 자동 창고관리 시스템
> 1. 창고에 보관된 물건의 양을 자동으로 파악한다. 
> 2. 블루투스로 사용자가 스마트폰에서 물건 이름을 검색하면, 물건의 위치와 현재 보관중인 양을 알려준다. 
> 3. 지정된 온도 범위를 벗어나면 자동으로 냉(난)방 장치를 가동한다. 
> 4. 침입자가 발생하면 부저를 울려 알린다.

## 개발 동기
> 계속 성장하고 있는 물류 산업!
> 계속 상승하는 인건비(최저시급)로 인한 자동화 시스템의 늘어나는 수요!
> 또한 군대에도 수많은 창고들이 있으며  ~
> 간단하고 저렴한 아두이노 센서들을 이용하여 창고 관리를 자동화 할 수 없을까?

## 개발 환경
> * Arduino Uno
> * 모듈 : 초음파 센서, RGB LED, 온습도 센서, PIR 모션 인식센서, HC-06 블루투스, 능동부저, 5V릴레이 모듈

## 설계
![iot](https://user-images.githubusercontent.com/32294568/67346184-dd592b80-f578-11e9-9a93-f991ba9a0f39.jpg)

## 구현 내용
> * 초음파 센서로 거리 측정을 통해 물건의 양을 파악한다.
> * RGB LED로 물건의 위치 및 남은 공간 등 사용자가 요청한 정보를 시각적으로 제공한다.
> * 방범 기능을 켰을 때, PIR 모션 인식센서로 움직임이 발생하면 부저를 울린다.
> * 온습도 센서로 온도를 측정한 뒤 사용자가 설정한 정상 온도 범위를 벗어나면, 5V릴레이 모듈로 냉(난)방기를 가동시킨다.
> * HC-06 블루투스 모듈을 통해 사용자 스마트폰으로부터 명령을 입력받고 정보를 제공해 준다.

## 기대 효과
> * add

## 사용 방법
> * ### 선반 사용
>	1. add
>	2. add
> * ### 블루투스로 입력 가능한 명령어
>	* 명령어 목록
>		* INPUT
>			* NAME [shelf(num)] [보관된 물건 이름]
>			* SIZE [shelf(num)] [보관된 물건의 1개당 길이]
>		* FIND
>			* NAME [찾을 물건 이름]
>			* SPACE
>		* OUTTING
>			* ON
>			* OFF
>		* SET
>			* TMP [설정할 최대 온도]
>			* CYCLE [설정할 거리 측정 주기]
>		* SHOW
>			* TMP
>	* 사용 예시
>		* INPUT NAME shelf0 apple -> 0번째(가장 아래 칸) 선반에 보관된 물건의 이름을 apple로 입력한다.
>		* INPUT SIZE shelf1 5 -> 1번째 선반에 보관된 물건의 1개당 길이를 5cm라고 입력한다.
>		* FIND NAME apple -> apple이 보관된 선반을 찾는다.
>		* FIND SPACE -> 가장 많은 공간이 남은 선반을 찾는다.
>		* OUTTING ON -> 외출 모드를 ON한다. 움직임이 감지되면 부저가 울린다.
>		* OUTTING OFF -> 외출 모드를 OFF한다.
>		* SET TMP 30 -> 온도 한계치를 30°C로 설정한다. 현재 온도가 이보다 높아지면 냉방 장치가 가동된다.
>		* SET CYCLE 1000 -> 거리(물건의 양) 측정 주기를 1000밀리초(1초)로 설정한다.
>		* SHOW TMP -> 현재 온도를 보여준다.

## 더 해보고 싶은 것
> * add


## 어플리케이션 소스코드
>http://ai2.appinventor.mit.edu/?galleryId=5185962499506176



## 크레딧
>Copyright © 2019 Junsik Bang, Minwook Kim
>All Rights Reserved
>
>이 프로젝트에서는 아래와 같은 오픈소스들이 사용되었음
>
>Arduino
>License : GPLv2, LGPLv2 https://github.com/arduino/Arduino/blob/master/license.txt
>
>App Inventor
>https://appinventor.mit.edu/
>Copyright © 2012 – 2019 Massachusetts Institute of Technology
>License : Creative Commons Attribution ShareAlike 3.0 Unported
>
>DHT-sensor
>Copyright © 2011 – 2019 Adafruit Industries
>License : MIT License https://github.com/adafruit/DHT-sensor-library/blob/master/DHT.h

