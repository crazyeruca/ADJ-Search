# AJD-Search

面对大量文档/文本内容的实时检索，采用单纯的字符串匹配依然不能满足需求，在此，
采用开源的检索库Xapian和中文分词库SCWS实现了大量文档的ADJ实时查询功能。

SCWS并不支持字母、数字或字母数字混合词组的分词，故在原有的基础上扩展了对字母
、数字和混合词组的分词功能，具体见commit：

>https://github.com/crazyeruca/ADJ-Search/commit/715ff410ebf156cdbbb6a68422bdf2d696f0f74f

同时在Xapian中新增ADJ Query Parser，具体见commit：

>https://github.com/crazyeruca/ADJ-Search/commit/715ff410ebf156cdbbb6a68422bdf2d696f0f74f

## Feature

* 实时全文检索
* 亦可用于其他检索需求，如联系人、邮件检索等。

## Example

本想采用Electron制作示例，奈何还未形成Node模块，故借助CEF3实现，技术栈如下：
1. CEF3，Chromium嵌入式框架；
2. Win32
3. React，采用React实现Web呈现；
4. mobx，辅助React实现数据管理，作为Redux的替代品，使用简单。
6. ES6

效果如下：

![alt](https://github.com/crazyeruca/ADJ-Search/blob/master/example.PNG)

## Tips

* 代码是跨平台的，在win10下测试通过。

## TODO

* 引入ICU，增加拼音检索。
* 采用ffi后gyp形成Node模块。

## 下载

* CEF3，当前使用的版本为：cef_binary_3.2743.1444.g7c94121_windows32.tar.tar
> http://opensource.spotify.com/cefbuilds/index.html

* xapian & scws
> http://www.xunsearch.com/

