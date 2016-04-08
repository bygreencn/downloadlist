# downloadlist

这是一个多线程同时下载多个文件的程序。使用get方法请求网络资源。

下载目录是download。

采用json配置待下载的文件。一个文件对应一个object:


{
	"name":"example.txt",
	"url":"http://www.example.com/example.txt"
}

参见download下的示例配置文件。

# 依赖
* libcurl
* [libcpr](https://github.com/webcpp/libcpr)
* libjsoncpp

