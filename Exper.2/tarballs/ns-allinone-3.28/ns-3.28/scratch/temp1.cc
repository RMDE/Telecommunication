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
