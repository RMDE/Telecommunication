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
#include <iostream>

using namespace std;
using namespace ns3;

// 日志设定
NS_LOG_COMPONENT_DEFINE ("StarWirelessNetwork"); // 使用NS_LOG定义一个日志模块

int main(int argc, char* argv[])
{
	// 启用日志
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_ALL);
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_ALL);// 使日志组件生效
	// LOG_ERROR -- 记录错误信息
	// LOG_WARN -- 记录警告信息
	// LOG_DEBUG -- 记录调试信息
	// LOG_INFO -- 记录程序相关信息
	// LOG_FUNCTION -- 记录函数调用信息
	// LOG_LOGIC -- 对于整体逻辑的描述
	// LOG_ALL -- 包含上述所有信息
	
	unsigned int number = 5; // STA节点数量
	// 命令行参数设置 用法：./waf --run "scratch/StarNet --n=5"
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
	
	
	// 创建star节点和AP节点
	NodeContainer nodes;
	nodes.Create(number);
	NodeContainer ap;
	ap.Create(1);
	
	// 配置通信信道和物理层信息
	YansWifiChannelHelper channel = YansWifiChannelHelper::Default (); // 设置默认通道
	YansWifiPhyHelper phy = YansWifiPhyHelper::Default (); // 配置phy助手
	phy.SetChannel (channel.Create ()); // 使每一个phy与Channel相关联
 
	// 配置WIFI 使用AARF速率控制算法
  	WifiHelper wifi; // 创建wifi助手,有助于创建WifiNetDevice对象
  	wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); // 设置wifi助手对象的速率控制算法类型：AARF算法
 
	// 配置MAC和SSID并且安装STAR和AP设备
  	WifiMacHelper mac; // 创建mac层
  	Ssid ssid = Ssid ("ns-3-ssid"); // 创建IEEE 802.11 SSID信息元素
  	mac.SetType ("ns3::StaWifiMac", // 设置mac类型为"ns3::StaWifiMac"
       		"Ssid", SsidValue (ssid), // 设置mac的"Ssid"属性的值为SsidValue (ssid)
               	"ActiveProbing", BooleanValue (false)); // 设置mac的"ActiveProbing"属性的值为BooleanValue (false)。如果为true，我们发送探测请求。
 
  	NetDeviceContainer staDevices; // 创建WifiStaNetDevice对象
  	staDevices = wifi.Install (phy, mac, nodes); // 根据phy、mac和节点集合返回一个NetDeviceContainer对象
 
  	mac.SetType ("ns3::ApWifiMac", // 设置mac类型为"ns3::ApWifiMac"
               	"Ssid", SsidValue (ssid));
 
  	NetDeviceContainer apDevices; // 创建WifiApNetDevice对象
	apDevices = wifi.Install (phy, mac, ap);
 
  	/* 添加移动模型，使sta节点处于移动状态，使ap节点处于静止状态 */
  	MobilityHelper mobility; // 创建MobilityHelper对象，将位置和移动模型分配给节点
 
  	// 设置位置分配器，用于分配MobilityModel :: Install中初始化的每个节点的初始位置。
 	mobility.SetPositionAllocator ("ns3::GridPositionAllocator", // 设置使用的移动模型的类型(在矩形2d网格上分配位置)
            	"MinX", DoubleValue (0.0), // 网格开始的x坐标
       		"MinY", DoubleValue (0.0), // 网格开始的y坐标
             	"DeltaX", DoubleValue (5.0), // 对象之间的x空间
              	"DeltaY", DoubleValue (10.0), // 对象之间的y空间
             	"GridWidth", UintegerValue (3), // 在一行中排列的对象数
         	"LayoutType", StringValue ("RowFirst")); // 布局类型
  	
	// 设置移动模型类型
 	mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", // 该类型：其中节点在随机方向上以随机速度围绕边界框移动
             	"Bounds", RectangleValue (Rectangle (-50, 50, -50, 50))); // 界限属性(矩形的范围)
  	mobility.Install (nodes); // 把该移动模型安装到wifiStaNodes节点上
 
  	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel"); // 固定位置的模型
  	mobility.Install (ap);
 
  	/* 安装协议栈 */
  	InternetStackHelper stack; // 用于将IP / TCP / UDP功能聚合到现有节点
 	stack.Install (ap);
 	stack.Install (nodes);
 
  	/* 给设备接口分配IP地址 */
 	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
  	Ipv4InterfaceContainer apInterfaces;  // apInterfaces用于获取ap节点的ip地址
  	apInterfaces = address.Assign (apDevices);
 	Ipv4InterfaceContainer staInterfaces; // staInterfaces用于获取sta节点的ip地址
  	staInterfaces = address.Assign (staDevices);
 
  	/* 创建clien和server应用 */
  	UdpEchoServerHelper echoServer (9); // 创建一个端口号为9的server
 
  	ApplicationContainer serverApps = echoServer.Install (ap.Get (0)); // 把该server安装到ap节点上
 	 serverApps.Start (Seconds (1.0)); // server开始于程序运行的第1秒中
  	serverApps.Stop (Seconds (10.0)); // server结束于程序运行的第10秒中
 
  	UdpEchoClientHelper echoClient (apInterfaces.GetAddress (0), 9); // 创建client，并为其指向ap节点的ip地址和端口号
  	echoClient.SetAttribute ("MaxPackets", UintegerValue (1)); // 应用程序发送的最大数据包数
  	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0))); // 数据包之间等待的时间
  	echoClient.SetAttribute ("PacketSize", UintegerValue (1024)); // 出站数据包中回波数据的大小
 
  	ApplicationContainer clientApps = echoClient.Install (nodes);
  	clientApps.Start (Seconds (2.0));
  	clientApps.Stop (Seconds (10.0));
 
  	/* 启用互联网络路由 */
  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
 
  	Simulator::Stop (Seconds (10.0));
 
	// 开启trace记录
  	if (tracing == true) 
    	{
      		phy.EnablePcap ("StarWifiNet", apDevices.Get (0));
      		phy.EnablePcap ("StarWifiNet", staDevices);
		AsciiTraceHelper trace;
		phy.EnableAsciiAll(trace.CreateFileStream("StarWifiNet.tr"));
		//包含两个方法调用。
		//CreateFileStream()用未命名的对象在协议栈中创建了一个文件流，并把这个文件流传递给了调用方法，即创建了一个对象代表着一个名为“first.tr”的文件，并传递给了NS3。

		//EnableAsciiAll()告诉helper你想要将ASCII tracing安装在仿真中的点到点设备上，并且你想要接收端以ASCII格式写出数据包移动信息。
    	}
 
  	AnimationInterface anim("StarWifiNet.xml");
     
  	// 启动模拟器
	Simulator::Run ();
 	Simulator::Destroy ();
  	return 0;
}
	



