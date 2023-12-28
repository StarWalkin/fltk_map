# SJTU CS2309 Project1: fltk_map
这是2023-2024学年第一学期SJTUCS2309“问题求解与实践”课程的项目1代码，主要实现的功能是***通过fltk库进行GUI编程，使得能够导入一张包含比例尺的百度地图截图，可以选定任一块区域，将其高亮化显示，并计算出像素面积和实际面积。***

<img src="demo1.jpg" style="zoom: 25%;" />

<center>示例1.验收要求中提供的示例图</center>

<img src="demo2.jpg" style="zoom: 25%;" />

<center>示例2.华东师范大学（闵行校区）</center>

<br>
<br>
如图所示，窗口的顶部为输入输出栏。

左边为输出区，输出框分别输出：新导入的点（current(x,y)），鼠标点击的位置（clicked(x,y)），连线的起点（start(x,y)），像素面积(pixel area),实际面积（real area）
<br>
右边为控制栏，next x和next y分别为下一个加入的点，Plotting Scale为当前图片比例尺，输入了nextx，nexty后点击Next Point，即会在图上连接上一个点和新加入点，加点完毕且输入好比例尺后点击finish，即会高亮显示并输出面积。clear和Quit分别为清屏和退出。

### 问题描述
本项目功能的实现主要包含如下的部分
* 导入带比例尺的地图
* 获取需要连接的点的坐标
* 连接各点
* 高亮化显示合围区域
* 计算像素面积与实际面积
* 输出数据

解决方案：

1.用课程提供的Graph_lib中Image类导入图片。

注1：对于fltk不支持的格式，利用mac自带的preview（或windows的“画图”）进行转码。

注2：这里创建窗口大小为1400*855（综合课程组要求与本人电脑屏幕大小），如果图片过或过小，会进行缩放后再添加到窗口的中间位置。

2.确定多边形各顶点采取两种方案，既可以直接给定点坐标，也可以通过鼠标点击（我设定了回调函数，如果鼠标点击图片中某位置，会在输出框clicked(x,y)中输出当前点击的坐标）

3.fltk库不支持透明度设置，为了近似实现高亮效果，我采用的方法是用稀疏的竖直黄线覆盖选中多边形区域。用射线法可以判断一点是否在形内，进而据此容易画出阴影。

4.关于计算像素面积，我使用计算几何中的多边形面积计算方法，拆分成三角形，分别使用向量叉积计算有向面积并累加即可。

5.比例尺的获得以及实际面积的计算：根据项目设定，给定截图中应包含比例尺。将图片导入并展示在窗口后，我们可以用鼠标分别点击比例尺两端，在clicked(x,y)的输出框中读出像素长度，从而与图示实际长度比较，得到比例尺。进而程序中会据此计算出比率，把像素面积换算成真实面积并在GUI界面输出。

### 运行方法
需在mingw,codeblocks,Visual Studio等中配置fltk环境。暂略。

<br>
<br>
<br>
有些遗憾的是，由于本人window和mac设备在运行fltk库时均会出现一些难以溯源的故障，只能借用同学设备运行测试，给项目进展带来诸多不便，故本项目在较好实现要求的功能的基础上，没有做过多的拓展。

<br>
<br>
感谢老师和助教一学期的陪伴和指导，祝新年快乐，万事胜意!


  
