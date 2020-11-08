/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
//161520211 贺星宇

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("AdHocNetwork");

int main (int argc, char *argv[])
{
    //启用日志显示
    LogComponentEnable ("AdHocNetwork", LOG_LEVEL_INFO);
    LogComponentEnable ("PacketSink", LOG_LEVEL_ALL);
    
    
    
    //无线站点数量
    uint32_t nWifi = 6;
    
    
    CommandLine cmd;
    cmd.Parse (argc,argv);
    
    
//    //生成sta节点
//    NodeContainer wifiStaNodes;
//    wifiStaNodes.Create (nWifi);
    
    //生成AP节点
    NodeContainer wifiApNodes;
    wifiApNodes.Create(nWifi);
    

    //建立移动模型，让sta和ap都固定
    MobilityHelper mobility;

 
    //对ap进行设置
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (5.0),
                                   "MinY", DoubleValue (5.0),
                                   "DeltaX", DoubleValue (5.0),
                                   "DeltaY", DoubleValue (10.0),
                                   "GridWidth", UintegerValue (3),
                                   "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (wifiApNodes);
    

    //使用点对点技术连接两个ap
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    
    NetDeviceContainer devices;
    NodeContainer node0 = NodeContainer(wifiApNodes.Get(1), wifiApNodes.Get(0));
    NodeContainer node1 = NodeContainer(wifiApNodes.Get(2), wifiApNodes.Get(1));
    NodeContainer node2 = NodeContainer(wifiApNodes.Get(3), wifiApNodes.Get(2));
    NodeContainer node3 = NodeContainer(wifiApNodes.Get(4), wifiApNodes.Get(3));
    NodeContainer node4 = NodeContainer(wifiApNodes.Get(5), wifiApNodes.Get(4));
//    NodeContainer node5 = NodeContainer(wifiApNodes.Get(6), wifiApNodes.Get(5));
    
    //a为ap和sta安装协议栈
    InternetStackHelper stack;
    stack.Install (wifiApNodes);
    //    stack.Install (wifiStaNodes);
    
    Ipv4AddressHelper address;
    //    //地址分配
    //    address.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces;
    //    interfaces = address.Assign(apDevices);
    //    stainterfaces = address.Assign (staDevices);
    //    address.Assign (apDevices);
    //
    
    devices = pointToPoint.Install (node0);
//    stack.Install (node0);
    address.SetBase("10.12.1.0", "255.255.255.0");
    interfaces = address.Assign(devices);
    
    devices = pointToPoint.Install (node1);
 //   stack.Install (node1);
    address.SetBase("10.12.1.1", "255.255.255.0");
    address.Assign(devices);
    
    devices = pointToPoint.Install (node2);
 //   stack.Install (node2);
    address.SetBase("10.12.2.0", "255.255.255.0");
    address.Assign(devices);
    
    devices = pointToPoint.Install (node3);
 //   stack.Install (node3);
    address.SetBase("10.12.3.0", "255.255.255.0");
    address.Assign(devices);
    
    devices = pointToPoint.Install (node4);
  //  stack.Install (node4);
    address.SetBase("10.12.3.0", "255.255.255.0");
    address.Assign(devices);
//    devices = pointToPoint.Install (node5);
//    address.SetBase("10.12.6.0", "255.255.255.0");
//    address.Assign(devices);
    
    //使用udp发送数据
    UdpEchoServerHelper echoServer (9);

    //将0号节点做为服务器端
    ApplicationContainer serverApps = echoServer.Install (wifiApNodes.Get (0));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (10000));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.00000001)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (wifiApNodes.Get (nWifi - 1));
    clientApps.Start (Seconds (6.0));
    clientApps.Stop (Seconds (10.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    
    //安装flowmon模块，用于计算吞吐量
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
    
    Simulator::Stop (Seconds (10.0));
    
    //生成xml语句
    AnimationInterface anim("mymulhoc.xml");
    Simulator::Run ();

    
    
    monitor->CheckForLostPackets ();
    
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
        if ((t.sourceAddress=="10.12.5.1" && t.destinationAddress == "10.12.1.2"))
        {
            std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
            std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
            std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
            std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
        }
    }




    Simulator::Destroy ();
    
   
    return 0;
}
