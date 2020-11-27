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

