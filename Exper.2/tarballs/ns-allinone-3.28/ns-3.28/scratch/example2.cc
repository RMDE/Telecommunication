#include "ns3/core-module.h"

#include "ns3/network-module.h"

#include "ns3/applications-module.h"

#include "ns3/wifi-module.h"

#include "ns3/mobility-module.h"

#include "ns3/internet-module.h"

#include "ns3/yans-wifi-helper.h"

#include "ns3/ssid.h"

#include "ns3/netanim-module.h"
#define TIME 20.0
#define PORT 8000


using namespace ns3;



NS_LOG_COMPONENT_DEFINE("AdHocNetwork");



int main(int argc, char *argv[]) {



    // 设置时间分辨率

    Time::SetResolution(Time::NS);



    // 激活日志组件

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_ALL);

    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_ALL);



    // ad hoc 设备数量

    uint32_t number = 4;



    // 命令行参数

    CommandLine cmd;

    cmd.AddValue("nAdHoc", "Number of WiFi AD devices", number);

    cmd.Parse(argc, argv);



    // 节点

    NodeContainer nodes1;
	nodes1.Create(number);
	NodeContainer nodes2;
	nodes2.Add(nodes1.Get(0));
	nodes2.Create(number);
	NodeContainer nodes3;
	nodes3.Add(nodes2.Get(3));
	nodes3.Create(number);



    // 创建信道和物理信息

	YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default(); 
	YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default(); 		
	phy1.SetChannel (channel1.Create()); 				  
	YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default();
	YansWifiPhyHelper phy2 = YansWifiPhyHelper::Default(); 		 
	phy2.SetChannel (channel2.Create());
	YansWifiChannelHelper channel3 = YansWifiChannelHelper::Default(); 
	YansWifiPhyHelper phy3 = YansWifiPhyHelper::Default();
	phy3.SetChannel (channel3.Create());



    // 创建 WiFi

    WifiHelper wifi1; // 创建wifi助手,有助于创建WifiNetDevice对象
  	wifi1.SetRemoteStationManager("ns3::ConstantRateWifiManager",
				     "DataMode",StringValue("OfdmRate6Mbps"));
  	WifiHelper wifi2; 
  	wifi2.SetRemoteStationManager("ns3::ConstantRateWifiManager",
				     "DataMode",StringValue("OfdmRate6Mbps"));
  	WifiHelper wifi3; 
  	wifi3.SetRemoteStationManager("ns3::ConstantRateWifiManager",
				     "DataMode",StringValue("OfdmRate6Mbps"));



    // 设置 mac

    WifiMacHelper mac;
  	mac.SetType("ns3::AdhocWifiMac", 	 // 设置类型
                    "Slot",StringValue("1s"));   // 插槽值 
  	NetDeviceContainer Devices1,Devices2,Devices3; 
  	Devices1 = wifi1.Install(phy1,mac,nodes1);	 // 设置节点网卡
  	Devices2 = wifi2.Install(phy2,mac,nodes2);	 
  	Devices3 = wifi3.Install(phy3,mac,nodes3);



    // 位置信息

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



    // 安装 internet 协议栈

    InternetStackHelper stack; 
 	stack.Install(nodes1.Get(1));
 	stack.Install(nodes1.Get(2));
 	stack.Install(nodes1.Get(3));
 	stack.Install(nodes2);
 	stack.Install(nodes3.Get(1));
 	stack.Install(nodes3.Get(2));
 	stack.Install(nodes3.Get(3));
 	stack.Install(nodes3.Get(4));



    // 设置 ip 地址

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



    // 应用层

    UdpEchoServerHelper Server(PORT);
	ApplicationContainer SApp = Server.Install(nodes1.Get(2));
	SApp.Start(Seconds(1.0));
	SApp.Stop(Seconds(TIME));
	UdpEchoClientHelper Client(interface1.GetAddress(2),PORT);
	Client.SetAttribute ("MaxPackets", UintegerValue (100));
  	Client.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  	Client.SetAttribute ("PacketSize", UintegerValue (1024));
	ApplicationContainer CApp = Client.Install(nodes3.Get(3));
	CApp.Start(Seconds(1.0));
	CApp.Stop(Seconds(TIME));

    // 全局路由表

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();


    // 先停止模拟器

    Simulator::Stop(Seconds(15.0));


    // NetAnim

    AnimationInterface anim("hoc.xml");



    // 启动模拟器

    Simulator::Run();

    Simulator::Destroy();



    return 0;



}
