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
	cmd.Parse(argc,argv);
	while(number<5 || number>15)
	{
		cout<<"Error:The number of the star devices should between 5 and 15"<<endl;
		cout<<"Please enter the number again:";
		cin>>number;
	}
	
	// 创建star节点和AP节点
	NodeContainer nodes;
	nodes.Create(number);
	NodeContainer ap;
	ap.Create(1);

	// 物理连接计算机
	// 抽象：物理实体=网络设备+信道，两者一一对应。
  	PointToPointHelper pointToPoint;//点到点通信助手类，通过所包含方法能设置网络设备和信道属性
  	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps")); // 设置数据传输率
  	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms")); // 设置延迟时间

	


    //InternetStackHelper internet;
    //internet.Install(n);
    //Ptr<Node> node=n.Get(1);
    //Node::AggregateIterator iterator = node->GetAggregateIterator();
    //while(iterator.HasNext())
    //{
    //    Ptr<const Object> obj = iterator.Next();
    //    std::cout<<"InternetStackHelper::Install name="<<obj->GetInstanceTyp    eId().GetName()<<std::endl;
    //}
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}

