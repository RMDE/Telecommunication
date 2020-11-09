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
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MystarScriptExample");

int 
main (int argc, char *argv[])
{
    //启用日志显示
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    
    
    
    //无线站点数量
    uint32_t nWifi = 5;
    
    
    CommandLine cmd;
//    cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
//    cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
//    cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
//    cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

    cmd.Parse (argc,argv);
    
    
    //生成sta节点
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create (nWifi);
    
    //生成AP节点
    NodeContainer wifiApNode;
    wifiApNode.Create(1);
    
    //生成wifi类型的信道
    //使用默认的信道模型
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
    //使用默认的phy模型
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
    //创建通道对象并把他关联到物理层对象管理器
    phy.SetChannel (channel.Create ());
    
    //建立wifi helper，并用AARF速率控制算法
    WifiHelper wifi;
    wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
    
    
    WifiMacHelper mac;
    
    //设置mac类型，并设置ssid
    Ssid ssid = Ssid ("ns-3-ssid");
    mac.SetType ("ns3::StaWifiMac",
                 "Ssid", SsidValue (ssid),
                 "ActiveProbing", BooleanValue (false));
    
    NetDeviceContainer staDevices;
    staDevices = wifi.Install (phy, mac, wifiStaNodes);
    
    //设置mac类型，并设置ap
    mac.SetType ("ns3::ApWifiMac",
                 "Ssid", SsidValue (ssid));
    
    NetDeviceContainer apDevices;
    apDevices = wifi.Install (phy, mac, wifiApNode);
    
    
    //建立移动模型，让sta和ap都固定
    MobilityHelper mobility;

    //对sta进行设置
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (0.0),
                                   "MinY", DoubleValue (0.0),
                                   "DeltaX", DoubleValue (5.0),
                                   "DeltaY", DoubleValue (10.0),
                                   "GridWidth", UintegerValue (3),
                                   "LayoutType", StringValue ("RowFirst"));

//    //设置sta的移动模式为随机移动，并这里移动范围
//    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
//                               "Bounds", RectangleValue (Rectangle (0, 0, 0, 0)));
//    mobility.Install (wifiStaNodes);

    
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (wifiStaNodes);
    
    //对ap进行设置
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (5.0),
                                   "MinY", DoubleValue (5.0),
                                   "DeltaX", DoubleValue (5.0),
                                   "DeltaY", DoubleValue (10.0),
                                   "GridWidth", UintegerValue (3),
                                   "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (wifiApNode);
    
    //a为ap和sta安装协议栈
    InternetStackHelper stack;
    stack.Install (wifiApNode);
    stack.Install (wifiStaNodes);
    
    Ipv4AddressHelper address;
    //地址分配
    address.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer stainterfaces;
    stainterfaces = address.Assign (staDevices);
    address.Assign (apDevices);
    
    
    //使用udp发送数据
    UdpEchoServerHelper echoServer (9);

    //将0号sta节点做为服务器端
    ApplicationContainer serverApps = echoServer.Install (wifiStaNodes.Get (0));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    UdpEchoClientHelper echoClient (stainterfaces.GetAddress (0), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (10000));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.00000001)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (wifiStaNodes.Get (nWifi - 1));
    clientApps.Start (Seconds (6.0));
    clientApps.Stop (Seconds (10.0));
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    
    
    
    //安装flowmon模块，用于计算吞吐量
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
    
    Simulator::Stop (Seconds (10.0));
    
    //生成xml语句
    AnimationInterface anim("mystar.xml");
    
    Simulator::Run ();
    
    monitor->CheckForLostPackets ();
    
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
        if ((t.sourceAddress=="10.1.3.5" && t.destinationAddress == "10.1.3.1"))
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
