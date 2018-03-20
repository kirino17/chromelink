chromelink
========================================

这是一个简单的用于`chrome`浏览器上的外部`Web`自动化测试工具,它依据`Chrome DevTools Protocol`协议编写而来,并向`EPL`提供了一套标准且友好的调用接口。

complie(only windows)
-------------------------

可使用`Visual Studio 2017`编译本工具源代码。

Operating
-------------------------

         在使用工具前你需要一个![chrome](https://www.google.cn/chrome/)浏览器

###### 启动chrome

```Bash
cd "C:\Program Files (x86)\Google\Chrome\Application\"
chrome --remote-debugging-port=9222 --user-data-dir=<some directory>
```

其中`--remote-debugging-port=9222`选项意为开启`chrome`浏览器的远程调试支持,`9222`则具体表示为用以远程调试的端口号,而`--user-data-dir=<some directory>`选项将可以使`chrome`浏览器把缓存数据存放至自定义的文件夹中。

等待`chrome`浏览器启动完成后访问`http://localhost:9222`即可查看在`chrome`浏览器中所有打开的标签页面信息。

###### 操作chrome tags


