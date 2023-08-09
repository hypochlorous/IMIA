### project1功能介绍



MainWindow.cpp

---

* 构造函数MainWindow()中初始化主窗口,类成员;

> 1. 按下按钮1选择并打开模型文件(只支持STL类型目前)  
> 2. 切换鼠标操作对象为Camera;默认左键表示旋转，右边的按钮表示缩放，中间的按钮表示平移，CTRL +左键用于旋转，shift +右键用于旋转环境旋转。(用较少的鼠标按钮，ctrl + shift +左键表示缩放，shift + left按钮表示平移)  
> 3. 切换鼠标操作对象为Actor;鼠标绑定如下:左键表示旋转，右边的按钮表示缩放，中间的按钮表示平移，并按CTRL +左键旋转(CTRL + shift +左键是缩放按钮，shift +左键是平移按钮)  
> 4. 按下按钮4得到renderer中所有Actors,并移除栈顶Actor(最后添加的一个)
>5. 创建多边形并加入到Renderer中(已弃用)

>6. 按下按钮进行切割,并进行体渲染展示;(切割位置与renderer中的假体actor位置同步)
>7. 创建一个点拾取器pointPicker,并加入到交互
>8. 创建骨头包围box
>9. 按下按钮进入划线模式,鼠标按下为起点,松开为终点,创建actor_line并添加到renderer()中
>10. 用于测试,输出当前假体位置信息;

* clip_func():输入被切割物体A(骨)切割物体B(假体),位移向量vector[3];返回切割结果;
* show_images():输入两份imagedata类型数据并进行体渲染;
* normalize():将space!={1,1,1}的数据标准化;(结果存在问题)