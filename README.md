# downloadlist

## 简介
这是一个多线程同时下载多个文件的程序。使用get方法请求网络资源。

通过命令行选项指定下载目录和配置。

采用json配置待下载的文件。一个文件对应一个object:

```
{
	"name":"example.txt",
	"url":"http://www.example.com/example.txt",
        "timeout":60
}
```

* name:保存文件名
* url:下载链接
* timeout: 超时设置，以秒计

参见downloadlist.json示例配置文件。

例如

```
./downloadlist --conf=downloadlist.json --dir=download
```



## 示例
```
make && make test
```

## 依赖
* libcurl
* [libcpr](https://github.com/webcpp/libcpr)
* libjsoncpp

