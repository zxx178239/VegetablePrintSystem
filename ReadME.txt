蔬菜录入系统

一个瘸腿系统，本来的目的是在录入之后进行打印操作，但是发现对打印一窍不通，网上资料也基本没有，貌似想要实现"一单一切"的目的需要定制打印机？

好吧，未完成的程序，下面记录一下程序中用到的技术

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

客户端 (VegetablePrintClient)
主界面使用Qt开发的，一个low且丑到爆的界面， 主要放置了两个大的功能，菜单打印和查询系统。

菜单打印功能已经实现了菜单生成，但是打印实在是没思路，普通打印是可以，但是要实现那种"一单一切"的打印效果不知道该如何做

查询系统已经从数据库中提取到了数据，没有显示出来，懒得搞吧，后期有心情再搞。

主要用到的Qt控件有
QTabWidget、QTableView、QPushButton、QPrinter、QCommoBox、QDateEdit、以及一些不重要的Label控件，界面布局丑的就不谈了。

其中datamanager用于存储从服务端接收过来的数据，sockclient用于和服务端进行交互，其他的就是一些显示的操作了。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

服务端 (VegetablePrintServer)
最基本的win32控制台编程，没有啥好的东西，主要用到的技术点如下

iocp (纯粹为了学习用一下，不知道有没啥错误)、mysql连接、C++

其中MainServer用于建立iocp，SQLClient用于和mysql进行交互，其他就没啥了。

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

数据库

数据库纯粹的使用的是mysql，没啥看头，未上传过来。

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

写在最后

目标未实现，就当成一个练手的low项目吧，顺便学习一波iocp，至于有没有必要c/s架构、有没有必要存数据库，这个问题就谈了，其实没必要；

纯练手吧，没啥技术含量的东西；

后期如果学会了打印技术再进行完善吧。

以上！