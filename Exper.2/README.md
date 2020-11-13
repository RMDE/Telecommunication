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

        ```text
        bulk-send-application
        on-off-application
        udp-client/server
        udp-echo-client/server
        ```

    - 信道

      - 网络中数据流过的媒介

      - 节点需要连接到信道上来进行数据交换

      - 用类Channel描述

        ```reStructuredText
        CsmaChannel
        PointToPointChannel
        Wi-FiChannel
        ```

    - 网络设备

      - 相当硬件设备和软件驱动的总和

      - 安装在节点上，与信道相对应

      - 用类`NetDevice`描述

        ```text
        CsmaNetDevice
        PointToPointNetDevice
        Wi-FiNetNevice
        ```

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

- 星型无线网络

  - 定义 ： 是指网络中的各节点设备通过一个网络集中设备（如集线器HUB或者交换机Switch）连接在一起，各节点呈星状分布的网络连接方式。这种拓扑结构主要应用于IEEE 802.2、IEEE 802.3标准的以太网中。

  - 结构图

    ![10](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\10.jpg)

  - 特点 ： 

    - 容易实现，但安装、维护工作量，成本较大：它所采用的传输介质一般都是采用通用的双绞线或同轴电缆。但是每个站点都要和中央网络集中设备直接连接，需要耗费大量的线缆，并且安装，维护的工作量也剧增。
    - 节点扩展、移动方便：节点扩展时只需要从集线器或交换机等集中设备中拉一条电缆即可，而要移动一个节点只需要把相应节点设备移到新节点即可。
    - 故障诊断和隔离容易：一个节点出现故障不会影响其它节点的连接，可任意拆走故障节点；
    - 中央节点的负担较重，易形成瓶颈；各站点的分布处理能力较低：中央节点一旦发生故障，则整个网络都受到影响。
    - 网络中任意两个节点之间进行通信都必须经过中心节点的转发

  - 优点 ：

    - 网络结构简单，便于管理、维护和调试。
    - 控制简单，添加或删除某个站点非常容易。
    - 集中管理，可方便地提供服务和网络重新配置。
    - 每个站点直接连到中央节点，容易检测和隔离故障

  - 缺点 ：

    - 线路利用率不高，一条线路只被该线路上的中央节点和一个节点使用。
    - 中央节点负荷太重，而且当中央节点产生故障时，全网将不能工作，对中央节点的可靠性和冗余度要求太高
    - 安装和维护费用高，需要大量电缆。

- 多跳无线网络

  - 定义 ：是一种无中心自组织的多跳无线网络，它不以任何已有的固定设施为基础而能随时随地组建临时性网络。它是具有特殊用途的对等式网络, 使用无线通信技术 ,网络中的节点互相作为其邻居（在其直接通信范围内的节点）的路由器 ,通过节点转发实现节点间的通信。 

  - 结构图　

    ![9](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\9.jpg)

  - 特点 ：
    - 自组织。这种网络可以不需要任何预先架设的无线通信基础设施，所有节点通过分层协议体系与分布式算法，来协调每个结点各自的行为。结点可以快速，自主和独立的组网
    - 对等结构 
    - 多跳路由 
    - 拓扑动态变化
    - 距离远的两个节点之间进行通信时，需要借助中间节点进行转发
  - 优点 ：
    - 快速部署和易于安装。安装Mesh节点非常简单，将设备从包装盒里取出来，接上电源就行了。用户可以很容易增加新的节点来扩大无线网路的覆盖范围和网路容量。
    - 非视距传输（NLOS），利用无线Mesh技术可以很容易实现配置，因此在室外和办公场所有着广泛的应用前景。
    - 健壮性，实现网路健壮性通常的方法是使用多路由传输数据。Mesh网路比单跳网路更加健壮，因为它不依赖于某一个单一节点的性能。在单跳网络中，如果某一节点出现故障，整个网路也就随之瘫痪。而在Mesh网路结构中，由于每个节点都有一条或几条传送数据的路径。如果最近的节点出现故障或者受到干扰，数据包将自动路由到备用路径继续进行传输，整个网路的运行不会受到影响。
    - 结构灵活，在单跳网络中，设备必须共享AP。如果几个设备要同时访问网络，就可能产生通信拥塞并导致系统的运行速度降低。而在多跳网络中，设备可以通过不同的节点同时连接网络，因此不会导致系统性能的降低。
    - 高带宽，无线通信的物理特性决定了通信传输的距离越短就越容易获得高带宽，因为随着无线传输距离的增加，各种干扰和其他导致数据丢失的因素随之增加。因此选择经多个短跳来传输数据将是获得更高网络带宽的一种有效方法。
  - 缺点 ：
    - 延迟，由于每次转发都需要一定延迟，多次转发之后延迟较高。
    - 网络容量，由于网络存在转发，每次转发之后速率都会降低，因此需要限制每个网络中节点的数量，所以网络节点数量尽量不要太多，否则会影响业务质量。

- 吞吐量

  - 定义 ：在没有帧丢失的情况下，设备能够接收并转发的最大数据速率
  - 大小影响因素 ：主要由网络设备的内外网口硬件，及程序算法的效率决定
  - 测试方法 ：在测试中以一定速率发送一定数量的帧，并计算待测设备传输的帧，如果发送的帧与接收的帧数量相等，那么就将发送速率提高并重新测试；如果接收帧少于发送帧则降低发送速率重新测试，直至得出最终结果

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

   - 在运行时选择图形化运行

     ```bash
     ./waf --run scratch/xxx.cc --vis
     ```

   - 吞吐量工具

     - .cc文件内嵌代码

       ```c++
       FlowMonitorHelper flowmon;
       Ptr<FlowMonitor> monitor = flowmon.InstallAll();
       monitor->CheckForLostPackets ();
       Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
       std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
       for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
       {
           Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
           std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
           std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
           std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
           std::cout << "  Throughput: " << (i->second.rxBytes+i->second.txBytes )* 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
       }
       ```

     - 查看`udp`具体传输数据

       ```bash
       tcpdump -r xxx.pcap -nn -tt
       ```

2. #### 设定网络参数

   - 星型无线网络
     - 初始发送速率 ：6Mbps
     - 速率算法 ：Constant Rate （发包速率一直保持恒定速度）
     - 路由协议：Dijkstra SPF （虚构）
     - 测试流量 ：UdpEcho —— 由 10.1.1.6 - > 10.1.1.7 ; 10.1.1.3 - > 10.1.1.7
     - 节点距离 ：X10，Y10
   - 多跳无线网络
     - 初始发送速率 ：6Mbps
     - 速率算法 ：Constant Rate （发包速率一直保持恒定速度）
     - 路由协议 ：Dijkstra SPF （虚构)
     - 测试流量 ：UdpEcho —— 由10.1.1.3 - > 10.1.3.4
     - 节点距离 ：X15，Y15

3. #### 代码

   - 星型无线网络

     ```c++
     /*****************************************************************************
     ** Author : Ye Xi
     ** ID     : 161720110
     ** Class  : 1617204
     ** Github : https://github.com/Telecommunication/Exper.2/star.cc 
     *****************************************************************************/
     #include "ns3/internet-stack-helper.h"
     #include "ns3/core-module.h"
     #include "ns3/point-to-point-module.h"
     #include "ns3/network-module.h"
     #include "ns3/applications-module.h"
     #include "ns3/wifi-module.h"
     #include "ns3/mobility-module.h"
     #include "ns3/csma-module.h"
     #include "ns3/internet-module.h"
     #include "ns3/netanim-module.h"
     #include "ns3/flow-monitor-module.h"
     #include <iostream>
     #define TIME 20.0
     #define PORT 8000
     using namespace std;
     using namespace ns3;
     
     // 日志设定
     NS_LOG_COMPONENT_DEFINE ("StarWirelessNetwork"); // 使用NS_LOG定义一个日志模块
     
     int main(int argc,char* argv[])
     {
     	
     	unsigned int number = 6; // STA节点数量
     	// 命令行参数设置 用法：./waf --run "scratch/star --n=5"
     	CommandLine cmd;
     	// 设定star节点数
     	cmd.AddValue("n","Number of star devices",number);
     	while(number<5 || number>15)
     	{
     		cout<<"Error:The number of the star devices should between 5 and 15"<<endl;
     		cout<<"Please enter the number again:";
     		cin>>number;
     	}
     	// 设置 trace
     	bool tracing = true;
     	cmd.AddValue("t","Wether turn on pcap tracing",tracing);
     	cmd.Parse(argc,argv);
     	
     	// 启用日志
     	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_ALL);
     	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_ALL); // 使日志组件生效
     	// LOG_ERROR -- 记录错误信息
     	// LOG_WARN -- 记录警告信息
     	// LOG_DEBUG -- 记录调试信息
     	// LOG_INFO -- 记录程序相关信息
     	// LOG_FUNCTION -- 记录函数调用信息
     	// LOG_LOGIC -- 对于整体逻辑的描述
     	// LOG_ALL -- 包含上述所有信息
     	
     	// 创建star节点和AP节点
     	NodeContainer nodes;
     	nodes.Create(number);
     	NodeContainer ap;
     	ap.Create(1);
     	
     	// 配置通信信道和物理层信息
     	YansWifiChannelHelper channel = YansWifiChannelHelper::Default(); // 设置默认通道
     	YansWifiPhyHelper phy = YansWifiPhyHelper::Default(); 		  // 配置phy助手
     	phy.SetChannel (channel.Create()); 				  // 使每一个phy与Channel相关联
      
     	// 远程基站管理 使用固定速率
       	WifiHelper wifi; // 创建wifi助手,有助于创建WifiNetDevice对象
       	wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                          "DataMode",StringValue("OfdmRate6Mbps"));
      
     	// 配置MAC和SSID并且安装STAR和AP设备
     	// 创建IEEE 802.11服务集标识符（SSID）对象，用来设置MAC层的“SSID”属性值
       	WifiMacHelper mac;
       	Ssid ssid = Ssid("ns3-ssid"); 
       	mac.SetType("ns3::StaWifiMac", 			  // 设置类型
            		    "Ssid",SsidValue(ssid), 		  // 设置ssid属性
                         "ActiveProbing",BooleanValue(false)); // 是否主动探测
       	NetDeviceContainer starDevices; 
       	starDevices = wifi.Install(phy,mac,nodes);	  // 设置star节点网卡
       	mac.SetType("ns3::ApWifiMac","Ssid",SsidValue(ssid)); 
       	NetDeviceContainer apDevices; 
     	apDevices = wifi.Install(phy,mac,ap);
       	
     	// 设置节点位置
     	//  添加移动模型，使sta节点处于移动状态，使ap节点处于静止状态 */
       	MobilityHelper mobility; 
       	// 设置位置分配器，用于分配初始化的每个节点的初始位置。
      	mobility.SetPositionAllocator("ns3::GridPositionAllocator",           // 设置移动模型的类型(在矩形2d网格上分配位置)
                 			       "MinX",DoubleValue(0.0),               // 网格开始的x坐标
            				       "MinY",DoubleValue(0.0),               // 网格开始的y坐标
     		               	       "DeltaX",DoubleValue(10.0),            // 对象之间的x间隔
                   			       "DeltaY",DoubleValue(10.0),           // 对象之间的y间隔
                  			       "GridWidth",UintegerValue(3),          // 在一行中排列的对象数
              			       "LayoutType",StringValue("RowFirst")); // 布局类型（竖排）
      	mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",            	       	   // 节点在随机方向上以随机速度围绕边界框移动
                  			   "Bounds",RectangleValue(Rectangle(-50,50,-50,50))); // 界限属性(矩形的范围)
       	mobility.Install(nodes); // star节点随机移动
       	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel"); // 固定位置的模型
       	mobility.Install(ap); // ap节点位置固定
      
       	// 安装协议栈 
       	InternetStackHelper stack; 
      	stack.Install(ap);
      	stack.Install(nodes);
      
       	// 给设备接口分配IP地址 
      	Ipv4AddressHelper address;
     	address.SetBase("10.1.1.0","255.255.255.0");
       	Ipv4InterfaceContainer starIP;      
       	starIP = address.Assign(starDevices);
      	Ipv4InterfaceContainer apIP;    
       	apIP = address.Assign(apDevices);
      
       	// 创建clien和server应用 
       	UdpEchoServerHelper Server(PORT);			     // 服务器端口绑定8000
       	ApplicationContainer SApps = Server.Install(ap.Get (0));     // 把该server安装到ap节点上
      	SApps.Start(Seconds(1.0)); 				     // 服务端在运行1s开始
       	SApps.Stop(Seconds(TIME)); 				     // 服务端在运行一段时间后结束
       	UdpEchoClientHelper Client(apIP.GetAddress(0),PORT);       // 设置客户端IP
       	Client.SetAttribute("MaxPackets",UintegerValue(5000));       // 应用程序发送的最大数据包数
       	Client.SetAttribute("Interval",TimeValue(Seconds(0.01)));     // 数据包之间等待的时间
       	Client.SetAttribute("PacketSize",UintegerValue(1024));       // 出站数据包中回波数据的大小
       	ApplicationContainer CApps1 = Client.Install(nodes.Get(number-1));
       	CApps1.Start(Seconds(2.0));
       	CApps1.Stop(Seconds(TIME));
       	ApplicationContainer CApps2 = Client.Install(nodes.Get(2));
       	CApps2.Start(Seconds(2.0));
       	CApps2.Stop(Seconds(TIME));
      
       	// 启用互联网络路由 
       	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
     	
     	FlowMonitorHelper flowmon;
     	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
      
       	Simulator::Stop(Seconds(TIME)); // 运行一段时间后自动关闭模拟器
     	// 开启trace记录
       	if(tracing==true) 
         	{
           		phy.EnablePcap("StarWifiNet",apDevices.Get(0));
           		phy.EnablePcap("StarWifiNet",starDevices);
       		AnimationInterface anim("StarWifiNet.xml"); // 动画保存，用于后期复现
     		AsciiTraceHelper trace;
     		phy.EnableAsciiAll(trace.CreateFileStream("StarWifiNet.tr"));
     		//包含两个方法调用。
     		//CreateFileStream()用未命名的对象在协议栈中创建了一个文件流,并把这个文件流传递给了调用方法,即创建了一个对象代表着一个名为“first.tr”的文件
     		//EnableAsciiAll()告诉helper你想要将ASCII tracing安装在仿真中的点到点设备上,并且你想要接收端以ASCII格式写出数据包移动信息。
         	}
     
     	Simulator::Run();
     	
     	monitor->CheckForLostPackets ();
         
         Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
         std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
         for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
         {
             Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
                 std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
                 std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
                 std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
                 std::cout << "  Throughput: " << (i->second.rxBytes+i->second.txBytes )* 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
         }
     	
      	Simulator::Destroy();
       	return 0;
     }
     
     ```

     - 图形化运行结果

       ```bash
       ./waf --run scratch/star --vis
       ```

       ![res1_0](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res1_0.png)

     - 查看udp日志

       ```bash
       tcpdump -r StarWifiNet-0-0.pcap -nn -tt
       ```

       ![res1_1](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res1_1.png)

     - 图形化复现过程

       ```bash
       ./NetAnim
       ## 选择StarWifiNet.xml
       ```

       ![res1_2](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res1_2.png)

     - 吞吐量

       ```bash
       gawk -f throughout.awk StarWifiNet.tr > starlog
       gnuplot
       >>> plot "starlog" with lines
       ```

       ![res1_3](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res1_3.png)

       当只有两个star节点与中心节点进行通信时，没有发生丢包。

       然而当再增加两个通信节点时，由于中心节点处理能力有限，会发生丢包现象

       ![res1_3_1](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res1_3_1.png)

   - 多跳无线网络

     ```c++
     /*****************************************************************************
     ** Author : Ye Xi
     ** ID     : 161720110
     ** Class  : 1617204
     ** Github : https://github.com/Telecommunication/Exper.2/adhoc.cc 
     *****************************************************************************/
     #include "ns3/internet-stack-helper.h"
     #include "ns3/core-module.h"
     #include "ns3/point-to-point-module.h"
     #include "ns3/network-module.h"
     #include "ns3/applications-module.h"
     #include "ns3/wifi-module.h"
     #include "ns3/mobility-module.h"
     #include "ns3/csma-module.h"
     #include "ns3/internet-module.h"
     #include "ns3/netanim-module.h"
     #include "ns3/yans-wifi-helper.h"
     #include "ns3/ssid.h"
     #include "ns3/flow-monitor-module.h"
     #include <iostream>
     #define TIME 20.0
     #define PORT 8000
     using namespace std;
     using namespace ns3;
     
     // 日志设定
     NS_LOG_COMPONENT_DEFINE ("AdhocWirelessNetwork"); // 使用NS_LOG定义一个日志模块
     
     int main(int argc,char* argv[])
     {
     	Time::SetResolution(Time::NS); // 设置时间分辨率	
     
     	unsigned int number = 4; // STA节点数量
     	// 命令行参数设置 用法：./waf --run "scratch/star --n=5"
     	CommandLine cmd;
     	// 设定star节点数
     	cmd.AddValue("n","Number of star devices",number);
     	while(number<3 || number>15)
     	{
     		cout<<"Error:The number of the star devices should between 5 and 15"<<endl;
     		cout<<"Please enter the number again:";
     		cin>>number;
     	}
     	// 设置 trace
     	bool tracing = true;
     	cmd.AddValue("t","Wether turn on pcap tracing",tracing);
     	cmd.Parse(argc,argv);
     	
     	// 启用日志
     	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_ALL);
     	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_ALL); // 使日志组件生效
     	
     	// 创建节点
     	NodeContainer nodes1;
     	nodes1.Create(number);
     	NodeContainer nodes2;
     	nodes2.Add(nodes1.Get(0));
     	nodes2.Create(number);
     	NodeContainer nodes3;
     	nodes3.Add(nodes2.Get(3));
     	nodes3.Create(number);
     	
     	
     	// 配置通信信道和物理层信息
     	YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default(); 
     	YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default(); 		
     	phy1.SetChannel (channel1.Create()); 				  
     	YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default();
     	YansWifiPhyHelper phy2 = YansWifiPhyHelper::Default(); 		 
     	phy2.SetChannel (channel2.Create());
     	YansWifiChannelHelper channel3 = YansWifiChannelHelper::Default(); 
     	YansWifiPhyHelper phy3 = YansWifiPhyHelper::Default();
     	phy3.SetChannel (channel3.Create()); 				  
      
     	// 远程基站管理 使用ARF速率控制算法
       	WifiHelper wifi1; // 创建wifi助手,有助于创建WifiNetDevice对象
     	wifi1.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                          "DataMode",StringValue("OfdmRate6Mbps"));
       	WifiHelper wifi2; 
     	wifi1.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                          "DataMode",StringValue("OfdmRate6Mbps"));
       	WifiHelper wifi3; 
     	wifi1.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                          "DataMode",StringValue("OfdmRate6Mbps"));
      
     	// 配置MAC与网卡
       	WifiMacHelper mac;
       	mac.SetType("ns3::AdhocWifiMac", 	 // 设置类型
                         "Slot",StringValue("1s"));   // 插槽值 
       	NetDeviceContainer Devices1,Devices2,Devices3; 
       	Devices1 = wifi1.Install(phy1,mac,nodes1);	 // 设置节点网卡
       	Devices2 = wifi2.Install(phy2,mac,nodes2);	 
       	Devices3 = wifi3.Install(phy3,mac,nodes3);	 
       	
     	// 设置节点位置
       	MobilityHelper mobility; 
       	// 设置位置分配器，用于分配初始化的每个节点的初始位置。
      	mobility.SetPositionAllocator("ns3::GridPositionAllocator",          
                 			      "MinX",DoubleValue(0.0),             
            				      "MinY",DoubleValue(0.0),              
     		               	      "DeltaX",DoubleValue(15.0),        
                   			      "DeltaY",DoubleValue(15.0),       
                  			      "GridWidth",UintegerValue(5),         
              			      "LayoutType",StringValue("RowFirst"));
      	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel"); 
       	mobility.Install(nodes1); 
       	mobility.Install(nodes2); 
       	mobility.Install(nodes3); 
      
       	// 安装协议栈 
       	InternetStackHelper stack; 
      	stack.Install(nodes1.Get(1));
      	stack.Install(nodes1.Get(2));
      	stack.Install(nodes1.Get(3));
      	stack.Install(nodes2);
      	stack.Install(nodes3.Get(1));
      	stack.Install(nodes3.Get(2));
      	stack.Install(nodes3.Get(3));
      	stack.Install(nodes3.Get(4));
      
       	// 给设备接口分配IP地址 
      	Ipv4AddressHelper address;
     	address.SetBase("10.1.1.0","255.255.255.0");
       	Ipv4InterfaceContainer interface1;      // 用于获取节点的ip地址
       	interface1 = address.Assign(Devices1);
     	address.SetBase("10.1.2.0","255.255.255.0");
       	Ipv4InterfaceContainer interface2;      // 用于获取节点的ip地址
       	interface2 = address.Assign(Devices2);
     	address.SetBase("10.1.3.0","255.255.255.0");
       	Ipv4InterfaceContainer interface3;      // 用于获取节点的ip地址
       	interface3 = address.Assign(Devices3);
     
     	UdpEchoServerHelper Server(PORT);
     	ApplicationContainer SApp = Server.Install(nodes1.Get(2));
     	SApp.Start(Seconds(1.0));
     	SApp.Stop(Seconds(TIME));
     	UdpEchoClientHelper Client(interface1.GetAddress(2),PORT);
     	Client.SetAttribute ("MaxPackets", UintegerValue (5000));
       	Client.SetAttribute ("Interval", TimeValue (Seconds (0.01)));
       	Client.SetAttribute ("PacketSize", UintegerValue (1024));
     	ApplicationContainer CApp = Client.Install(nodes3.Get(3));
     	CApp.Start(Seconds(1.0));
     	CApp.Stop(Seconds(TIME));
      
     	NS_LOG_INFO("Create Applications"); // 日志结点
     	// On,Off随机替换，OFF模式无流量，ON模式产生恒定速率流量
     	// ontime = 1,offtime = 0表示一直发送
       	
     	// 启用互联网络路由 
       	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
      
      	FlowMonitorHelper flowmon;
         	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
      
       	Simulator::Stop(Seconds(TIME)); // 运行一段时间后自动关闭模拟器
     	// 开启trace记录
       	if(tracing==true) 
         	{
           		phy1.EnablePcap("AdhocWifiNet1",Devices1);
           		phy2.EnablePcap("AdhocWifiNet2",Devices2);
           		phy3.EnablePcap("AdhocWifiNet3",Devices3);
       		AnimationInterface anim("AdhocWifiNet.xml"); // 动画保存，用于后期复现
     		AsciiTraceHelper trace;
     		phy1.EnableAsciiAll(trace.CreateFileStream("AdhocWifiNet1.tr"));
     		phy2.EnableAsciiAll(trace.CreateFileStream("AdhocWifiNet2.tr"));
     		phy3.EnableAsciiAll(trace.CreateFileStream("AdhocWifiNet3.tr"));
     		//包含两个方法调用。
     		//CreateFileStream()用未命名的对象在协议栈中创建了一个文件流,并把这个文件流传递给了调用方法,即创建了一个对象代表着一个名为“first.tr”的文件
     		//EnableAsciiAll()告诉helper你想要将ASCII tracing安装在仿真中的点到点设备上,并且你想要接收端以ASCII格式写出数据包移动信息。
         	}
          
     	Simulator::Run();
     	
     	monitor->CheckForLostPackets ();
         
         	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
         	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
         	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
         	{
             	Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
                 	std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
                 	std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
                 	std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
                 	std::cout << "  Throughput: " << (i->second.rxBytes+i->second.txBytes) * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
             }
     	
      	Simulator::Destroy();
       	return 0;
     }
     ```

     - 图形化运行结果

       ```bash
       ./waf --run scratch/adhoc --vis
       ```

       ![res2_0](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_0.png)

       该图为路由交换时的数据流量图，可以明显地分辨出最上面一排节点在网络10.1.1.0中，第一排最后一个节点与第二排所有节点在网络10.1.2.0中，第二排最后一个节点与第三排所有节点在网络10.1.3.0中。

       ![res2_1](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_1.png)

       可以看出，从10.1.1.3出发的数据包先经过10.1.1.1（10.1.2.1）转发，来到10.1.2.0网络；再经过10.1.2.4（10.1.3.1）转发，来到10.1.3.0网络；最后传输到目标节点10.1.3.4中。

     - 查看udp日志

       ```bash
       tcpdump -r AdhocWifiNet1-0-0.pcap -nn -tt
       ```

       ![res2_2](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_2.png)

       可以看出，数据包确实是在10.1.1.3与10.1.3.4之间传输

     - 图形化复现过程

       ```bash
       ./NetAnim
       ## 选择AdhocWifiNet.xml
       ```

       ![res2_3](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_3.png)

       ​                                                                     网络10.1.3.0

       ![res2_2_1](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_3_1.png)

       ​                                                                 网络10.1.2.0

       ![res2_3_2](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_3_2.png)

       ​                                                                      网络10.1.1.0

     - 吞吐量

       ![res2_5](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res2_5.png)

------

## 总结

​	还是第一次接触ns3工具，感觉还不是很了解各方面的功能。不过对于星型网络以及多跳网络已经了解很多了。而且也知道了仿真网络的大致步骤，对于网络结构也更加了解。

