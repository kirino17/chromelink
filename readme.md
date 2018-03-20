chromelink
========================================

这是一个用于`chrome`浏览器上的远程`Web`自动化测试工具,它依据`Chrome DevTools Protocol`协议编写而来,并使用`EPL`公开了一套标准且友好的脚本调用接口。

features(特性)
-------------------------

* 可同时使用多个`chrome`进行自动化测试
* 多个`chrome`间可独立使用缓存
* 提供了完善的`cookie` 访问方法
* 可以动态的清除缓存以及`cookie`
* 集成了丰富的常用自动化测试方法
* 表达式求值及运算
* 输入设备的支持(键盘、鼠标、touch)
* 可以建立仿真的设备环境(屏幕、UA、角度、方向、touch等等)
* 支持模拟触摸设备(可使用: 合成/捏合、横向/纵向滚动、触摸(Tap)等等手势动作)
* 支持模拟地理位置信息

complie(only windows)
-------------------------

可使用`Visual Studio 2017`编译本工具源代码。

Operating
-------------------------

在使用工具前你可能需要拥有一个内核版本在54及以上的[chrome](https://www.google.cn/chrome/)浏览器

### 启动chrome

```Bash
cd "C:\Program Files (x86)\Google\Chrome\Application\"
chrome --remote-debugging-port=9222 --user-data-dir=<some directory>
```

其中:
* `--remote-debugging-port=9222`选项意为开启`chrome`浏览器的远程调试支持,`9222`则具体表示为用以远程调试的端口号。
* `--user-data-dir=<some directory>`选项可以让`chrome`浏览器将缓存数据放在你所定义的文件夹中。

在浏览器中访问`http://localhost:9222`即可查看在`chrome`浏览器中所有打开的标签页面。

### 操作chrome tags

##### 获取所有打开的标签信息:


![gettags](http://www.ps-works.cn/chromelink/doc/img/gettags.png "获取标签")

###### “ec726540-0397-4517-884e-caca92abbdf7” | “百度一下，你就知道” | “https://www.baidu.com/”
###### “0edcf6d5-e9e2-4e23-9a1d-254aa9a52cc3” | “黑猫 - 易浏览器项目” | “http://www.ps-works.cn/”

-------------------------------------------

##### 打开新的标签:

![newtag](http://www.ps-works.cn/chromelink/doc/img/newtag.png "打开标签")

###### “cec7969b-a25c-4548-b3d0-786c4a6e42af” | “百度一下，你就知道” | “https://www.baidu.com/”

---------------------------------------------

##### 关闭标签:

![closetag](http://www.ps-works.cn/chromelink/doc/img/closetag.png "关闭标签")

-----------------------------------------------

##### 激活标签:

![activetag](http://www.ps-works.cn/chromelink/doc/img/activetag.png "激活标签")

------------------------------------------------

### Sample code

###### 跳转URL

![jmpurl](http://www.ps-works.cn/chromelink/doc/img/jmpurl.png "跳转URL")

------------------------------------------------

##### 执行表达式

![eval](http://www.ps-works.cn/chromelink/doc/img/eval.png "执行表达式")
