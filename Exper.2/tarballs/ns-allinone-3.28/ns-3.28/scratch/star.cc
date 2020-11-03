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
#define TIME 20.0
#define PORT 8000
using namespace std;
using namespace ns3;

// 日志设定
NS_LOG_COMPONENT_DEFINE ("StarWirelessNetwork"); // 使用NS_LOG定义一个日志模块

int main(int argc,char* argv[])
{
	
	unsigned int number = 6; // STA节点数量
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
 
	// 远程基站管理 使用AARF速率控制算法
  	WifiHelper wifi; // 创建wifi助手,有助于创建WifiNetDevice对象
  	wifi.SetRemoteStationManager("ns3::AarfWifiManager"); // 设置wifi助手对象的速率控制算法类型：AARF算法
	// AarfcdWifiManager : 在判断是否增加速率之前，该算法会打开RTS/CTS机制，当数目可变的传输尝试成功之后会再次关闭RTS/CTS机制。
	//		       AARF-CD算法保留了ARF中的定时器，提高了传输多媒体资源时的性能。
	//		       该算法最大的贡献就是通过碰撞检测即加入RTS/CTS机制解决了传输时的资源竞争问题。
	// AarfWifiManager : 速率控制算法。在ARF基础之上演变，旨在提高稳定环境下的性能（采取自适应的阈值更新）。
	// 		     在稳定的信道环境中扩大了增加速率时的时间间隔，产生更小的速率波动。
	// AmrrWifiManager : 使用二进制指数退避（BEB）技术来适应用于改变速率和传输计数参数值的采样周期的长度（阈值）。
	// 		     使用探测包，并根据其传输状态自适应地改变阈值。 
	//		     适应机制通过不由倒退机制指定的更高速率来确保更少的故障传输/重传和更高的吞吐量。
	//		     采用启发式方法，通过明智地设置速率和传输计数参数来捕获信道的短期变化。
	// AparfWifiManager : 用于决定数据速率或发射功率变化的阈值是动态调整的。 
	// ArfWifiManager : 以一个非常低的基础的数据速率（2Mbps）开始，然后触发一个定时器
	// 		    当任意一个定时器停止或发送机成功的进行了N次连续的传输后，发射器从旧数据速率rold增到一个新值rnew，并重置定时器，
	//		    每次增加固定值，若增加后第一次传输就失败，则退回到旧速率rold，如果再次发送失败，则依固定的值递减。
	//		    ARF将帧丢失率视为信道条件的指标。根据连续成功的传输次数调整速率。
	// CaraWifiManager : 和ARF类似。对传输成功的次数进行计数，连续传输失败的计数器将会被重置。
	//		     当连续成功的次数达到指定的阀值后，就会增加速率。
	// ConstantRateWifiManager : 总是使用相同的速率发送一个包
	// IdealWifiManager : 理想状态下，用于进行实验对比
	// MinstrelHtWifiManager : 是基于Minstrel的802.11n/ac标准的速率适配机制，并且基于探测频道以动态学习可支持的工作速率的方法。
	//			   为实现多速率重试（MRR）链的高延迟设备而设计。
	// MinstrelWifiManager : 根据收集成功传播概率的统计资料，探索环境，调整速度。
	//			 将速率调整到其认为成功的最高速率，并花费其一小部分时间通过尝试其他速率“周围环顾”。
	// OnoeWifiManager : 被用作madwifi驱动程序的默认速率控制算法。
	//		     其中信用值根据成功发送的数量和在确定的采样周期上积累的错误传输和重传的数量来确定。 
	// 		     对于以特定速率的成功传输，Onoe不断提高其信用额度，达到某些阈值时，当前的传输速率将提高到下一个更高的速率。
	// ParfWifiManager : 功率控制自动速率回退技术。基于发射功率和数据速率控制而提出的一种自我调节技术。 
	//		     尝试最小化基于自动速率回退（ARF）的相邻AP之间的干扰
	// RraaWifiManager : 鲁棒速率适应算法有两个机制：速率选择器（RRAA-BASIC）和自适应RTS（ARts）。
	//		     速率选择器计算在一个观测窗口内，发送失败传输的次数，在该窗口结束的时候，计算丢包率，如果丢包率比给定的阀值PMTL大，速率就减少。
	// RrpaaWifiManager : 实现了基于帧丢失率的功率和速率控制机制。 尝试使用尽可能低的功率，而不会降低链路的性能。
	//		      先尝试在当前信道条件下找到最大功率的最佳速率，然后如果损耗稳定开始降低功率。
 
	// 配置MAC和SSID并且安装STAR和AP设备
	// SSID : 无线局域网名称
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
  	Ipv4InterfaceContainer interface;      // 用于获取ap节点的ip地址
  	interface = address.Assign(starDevices);
 	//Ipv4InterfaceContainer starIp;    // staInterfaces用于获取sta节点的ip地址
  	address.Assign(apDevices);
 
  	// 创建clien和server应用 
  	UdpEchoServerHelper Server(PORT);			     // 服务器端口绑定8000
  	ApplicationContainer SApps = Server.Install(ap.Get (0));     // 把该server安装到ap节点上
 	SApps.Start(Seconds(1.0)); 				     // 服务端在运行1s开始
  	SApps.Stop(Seconds(TIME)); 				     // 服务端在运行一段时间后结束
  	UdpEchoClientHelper Client(interface.GetAddress(0),8000);       // 设置客户端IP
  	Client.SetAttribute("MaxPackets",UintegerValue(5000));       // 应用程序发送的最大数据包数
  	Client.SetAttribute("Interval",TimeValue(Seconds(1.0)));     // 数据包之间等待的时间
  	Client.SetAttribute("PacketSize",UintegerValue(1024));       // 出站数据包中回波数据的大小
  	ApplicationContainer CApps = Client.Install(nodes.Get(number-1));
  	CApps.Start(Seconds(2.0));
  	CApps.Stop(Seconds(TIME));
 
  	// 启用互联网络路由 
  	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
 
 
 
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
 	Simulator::Destroy();
  	return 0;
}
