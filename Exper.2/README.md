## 目录

[TOC]

------

## 利用NS3部署星型网络和多跳网络



### 实验目的

- 理解不同无线网络拓扑对性能的影响

### 实验环境

- Linux

### 实验语言

- c/c++

### 实验内容

- 利用NS2/NS3部署一个星型无线网络（一个AP，不少于5个接入点）、一个多跳无线网络（不少于6个网络节点）。
- 测量两种网络拓扑下的网络链路吞吐量，用图表表示。

### 实验理论

- #### NS3

  - 网址：https://blog.csdn.net/qq_41816035/article/details/88071765

  - 简介

    - NS3是一个离散事件驱动的网络模拟器
    - 始于2006年的开源项目
    - 第一版发布时间：2008.6.30
    - 适用平台：Linux/Mac OS/Cygwin
    - “NS3项目”的主要文档由四部份组成：
      - NS3Doxygen/Manual：模拟器公共API的说明文档
      -  Tutorial
      - Reference Manual：参考手册
      - NS3 wiki (NS-3维基百科)

  - 基本模型

    ![2](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\2.png)

  - 关键概念

    - 节点

      - 即基本计算设备，是一个可以添加各种功能的计算机。

      - 要使它工作需要添加网卡、协议栈、应用程序

      - 创建节点

        ```c++
        NodeContainer nodes;
        nodes.Create(2);
        ```

    - 应用

      - 需要仿真的用户程序被抽象为应用，用类Application描述

    - 信道

      - 网络中数据流过的媒介
      - 节点需要连接到信道上来进行数据交换
      - 用类Channel描述

    - 网络设备

      - 相当硬件设备和软件驱动的总和
      - 安装在节点上，与信道相对应
      - 用类NetDevice描述

  - C++脚本的编写

    

    - | 步骤                      | 实例                                                         |
      | ------------------------- | ------------------------------------------------------------ |
      | 1. 创建节点               | NodeContainer                                                |
      | 2. 创建链路类型           | XxxDeviceHelper                                              |
      | 3. 安装链路类型，生成网卡 | XxxDeviceContainer=XxxDeviceHelper.install(NodeContainer)    |
      | 4. 安装协议栈             | XxxStack.install(NodeContainer)                              |
      | 5. 配置IP地址             | XxxAddressHelper.setBase(“IP”,“NETMASK”)                     |
      | 6. 生成网络接口           | XxxInterfaceContainer = XxxAddressHelper.Assign(NetDeviceContainer ) |
      | 7. 安装应用               | ApplicationContainer = XxxHelper.Install(NodeContainer);     |
      | 8. 开始仿真               |                                                              |

### 实验实践

1. #### NS3环境搭建

   - 安装依赖库

     - 安装语言环境

       ```bash
       sudo apt-get install gcc g++ python python-dev
       ```

     - NS3代码维护使用的源码版本控制管理系统

       ```bash
       sudo apt-get install mercurial
       ```

     - 运行python绑定`ns-3-dev`需要`bazaar`这个组件

       ```bash
       sudo apt-get install bzr
       ```

     - 调试工具

       ```bash
       sudo apt-get install gdb valgrind 
       ```

     - 支持更多精确WIFI模块的`GNU Scientific Library (GSL)`

       ```bash
       sudo apt-get install gsl-bin libgsl-dev libgsl2
       ```

     - 仿真必需的词法分析器和语法分析生成器

       ```bash
       sudo apt-get install flex bison libfl-dev
       ```

     - `Network Simulation Cradle (nsc) stacks`需要`gcc-3.4`

       ```bash
       sudo apt-get install g++-3.4 gcc-3.4
       ```

     - 读取`pcap`的`packet traces`

       ```bash
       sudo apt-get install tcpdump
       ```

     - 支持统计特性的数据库软件

       ```bash
       sudo apt-get install sqlite sqlite3 libsqlite3-dev 
       ```

     - xml的配置存储软件

       ```bash
       sudo apt-get install libxml2 libxml2-dev
       ```

     - 基于GTK的配置系统

       ```bash
       sudo apt-get install libgtk2.0-0 libgtk2.0-dev
       ```

     - 在虚拟机and `ns-3`上测试

       ```bash
       sudo apt-get install vtun lxc
       ```

     - 支持`utils/check-style.py `代码风格检查程序

       ```bash
       sudo apt-get install uncrustify
       ```

     -  文档生成器

       ```bash
       sudo apt-get install doxygen graphviz imagemagick
       ```

     - `Gustavo’s ns-3-pyviz`的可视化软件

       ```bash
       sudo apt-get install python-pygraphviz python-kiwi python-pygoocanvas libgoocanvas-dev mercurial qt5-default
       ```

     - 支持`openflow `模块

       ```bash
       sudo apt-get install libboost-signals-dev libboost-filesystem-dev
       ```

     - 支持基于 MPI的分布式仿真

       ```bash
       sudo apt-get install openmpi*
       ```

     - 其他

       ```bash
       sudo apt-get install texlive texlive-extra-utils texlive-latex-extra
       sudo apt-get install python-sphinx dia
       ```

   - 安装NS3（通过`Tarballs`方式)

     - 创建目录

       ```bash
       mkdir tarballs
       cd tarballs
       ```

     - 下载压缩包

       ```bash
       wget http://www.nsnam.org/release/ns-allinone-3.28.tar.bz2
       ```

     - 解压

       ```bash
       tar xjf ns-allinone-3.28.tar.bz2
       ```

   - 编译NS3

     ```bash
     cd ns-allinone-3.28
     
     ./build.py 
     
     cd ns-3.28
     
     ./waf distclean （清除整个build目录）
     
     ./waf configure --build-profile=debug --enable-examples --enable-tests（打开debug并开启例子及帮助）
     
     ./waf build 
     ```

     ![3](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\3.png)

   - 测试

     ```
     ./test.py -c core
     ```

     ![4](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\4.png)

     ![5](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\5.png)

   - 运行`hello-simulator`程序

     ```bash
     ./waf --run hello-simulator
     ```

     ![6](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\6.png)

   - 创建新程序

     - 在`~/tarballs/ns-allinone-3.28/ns-3.28/examples`目录下新建文件夹并进入

       ```bash
       mkdir ns3_test
       cd ns3_test
       ```

     - 新建需要运行的文件`ns3_test.cc`

       ```c++
       #include "ns3/network-module.h"
       #include "ns3/core-module.h"
       #include "ns3/internet-stack-helper.h"
       #include <iostream>
       using namespace std;
       using namespace ns3;
       int main()
       {
           NodeContainer n;
           n.Create(2);
           InternetStackHelper internet;
           internet.Install(n);
           Ptr<Node> node=n.Get(1);
           Node::AggregateIterator iterator = node->GetAggregateIterator();
           while(iterator.HasNext())
           {
               Ptr<const Object> obj = iterator.Next();
               std::cout<<"InternetStackHelper::Install name="<<obj->GetInstanceTyp    eId().GetName()<<std::endl;
           }
       }
       ```

     - 新建引导文件`wscript`

       ```javascript
       def build(bld):
           obj = bld.create_ns3_program('ns3_test',['csma','internet','applications    '])
           obj.source = 'ns3_test.cc'
       ```

     - 在`~/tarballs/ns-allinone-3.28/ns-3.28/`目录下运行

       ```
       ./waf    #重新编译修改部分
       ./waf --run ns3_test
       ```

       ![7](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\7.png)

       ![8](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\8.png)

   - 另一种方法进行脚本运行

     - 将`.cc`文件放入`scratch`目录下

     - 编译

       ```bash
       ./waf
       ```

     - 运行脚本

       ```bash
       ./waf --run scratch/star
       ```

   - 代码可视化

     - 安装`NetAnim`

       这是一个基于`Qt5`的`NS3`可视化程序，它的源码也一同附带在了`NS3`的源码包中，我们要做的就是使用`Qt5`编译它

       - 首先需要安装`Qt5`和`Mercurial`

         ```bash
         sudo apt-get install mercurial
         sudo apt-get install qt5-default
         ```

       - 使用`Qt5`的编译器`qmake`进行编译

         ```bash
         cd ns-allinone-3.28/netanim-3.108
         qmake ./NetAnim.pro
         ```

       - 打开`NetAnim`程序

         ```bash
         ./NetAnim
         ```

         ![9](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\9.png)

   - 吞吐量工具

     - 安装`gnuplot`和`gawk`

       ```bash
       sudo apt-get install gnuplot
       sudo apt-get install gawk
       ```

       这是一个根据`NS3`脚本运行时产生的`trace`文件生成图标的软件，这里提供了一个用于求吞吐量的`awk`脚本，也就是`throughout.awk`

     - 具体使用方法

       - 将脚本放在`waf`同级目录下

       - 存入数据`XXX`

         ```bash
         gawk -f throughout.awk xxx.tr > xxx
         #xxx.tr是脚本运行过程中生成的.tr文件
         ```

       - 使用`gnuplot`来画图

         - 进入`gnuplot`的`shell`

           ```bash
           gnuplot
           ```

         - 画图

           ```
           plot "xxx" with lines
           ```

2. #### 代码

   ```
   
   ```

   

3. #### 实践结果

   

------

## 总结

​	

