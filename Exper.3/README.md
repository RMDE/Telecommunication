## 目录

[TOC]

------

## 利用NS3部署LTE网络



### 实验目的

- 理解无线蜂窝网络基本概念

### 实验环境

- Linux

### 实验语言

- c/c++

### 实验内容

- 利用NS3部署一个LTE网络，具体参数如表所示

  ![image](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\1.png)

- 绘制仿真的Radio Environment Map

- 统计用户当前接入基站的RSRP，SINR，吞吐量以及距离基站的距离

### 实验理论

- 含义：蜂窝网络，是一种移动通信硬件架构，分为模拟蜂窝网络和数字蜂窝网络。由于构成网络覆盖的各通信基地台的信号覆盖呈六边形，从而使整个网络像一个蜂窝而得名。采用蜂窝无线组网方式，在终端和网络设备之间通过无线通道连接起来，进而实现用户在活动中可相互通信。其主要特征是终端的移动性，并具有越区切换和跨本地网自动漫游功能。

- 组成：
  - 移动站 ：即网络终端设备，如手机或一些蜂窝工控设备 
  - 基站子系统 ：包括移动基站（大铁塔）、无线收发设备、专用网络（一般是光纤）、无数的数字设备等等，可把基站子系统看作是无线网络与有线网络之间的转换器。
  - 网络子系统 

- 优势：
  - 频率复用：有限的频率资源可以在一定的范围内被重复使用。
  - 小区分裂：当容量不够的时候，可以减小蜂窝的范围，划分出更多的蜂窝，进一步提高频率的利用效率。

- 结构：

  ![11](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\11.jpg)

- 协议栈

  ![12](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\12.jpg)

- 评测

  - RSRP

    RSRP (Reference Signal Receiving Power，参考信号接收功率) 是LTE网络中可以代表无线信号强度的关键参数以及物理层测量需求之一，是在某个符号内承载参考信号的所有RE(资源粒子)上接收到的信号功率的平均值。

    单位是： dBm。

    | RSRP(dBm)                    | 覆盖强度级别 | 备注                                                         |
    | ---------------------------- | ------------ | ------------------------------------------------------------ |
    | Rx<= -105                    | 6            | 覆盖较差。业务基本无法起呼。                                 |
    | -105<rx<=-95 p=""></rx<=-95> | 5            | 覆盖差。室外语音业务能够起呼，但呼叫成功率低，掉话率高。室内业务基本无法发起业务。 |
    | -95<rx<=-85 td=""></rx<=-85> | 4            | 覆盖一般，室外能够发起各种业务，可获得低速率的数据业务。但室内呼叫成功率低，掉话率高。 |
    | -85<rx<=-75 p=""></rx<=-75>  | 3            | 覆盖较好，室外能够发起各种业务，可获得中等速率的数据业务。室内能发起各种业务，可获得低速率数据业务。 |
    | -75<rx<=-65 p=""></rx<=-65>  | 2            | 覆盖好，室外能够发起各种业务可获得高速率的数据业务。室内能发起各种业务，可获得中等速率数据业务。 |
    | Rx>-65                       | 1            | 覆盖非常好。                                                 |

  - SINK

  - 吞吐量

- 名词解释

  - UE ：用户

  - eNB ：是eNodeB的简写，它为用户提供空中接口（air interface），用户设备可以通过无线连接到eNB，也就是我们常说的基站，然后基站再通过有线连接到运营商的核心网。在这里注意，我们所说的无线通信，仅仅只是手机和基站这一段是无线的，其他部分例如基站与核心网的连接，基站与基站之间互相的连接，核心网中各设备的连接全部都是有线连接的。一台基站(eNB)要接受很多台UE的接入，所以eNB要负责管理UE，包括资源分配，调度，管理接入策略等等。

  - EPC ：一个全IP的分组核心网EPC（Evolved Packet Core），该系统的特点为仅有分组域而无电路域、基于全IP结构、控制与承载分离且网络结构扁平化，其中主要包含MME、SGW、PGW、PCRF等网元。其中SGW和PGW常常合设并被称为SAE-GW。

    - MME（Mobility Management Entity）

      是移动管理实体，是一个用于信令控制的网元，主要用作移动性的管理。此外还需要做会话相关的控制处理。功能列表：

      1）NAS信令处理，NAS信令安全保护

      2）3GPP内不同节点之间的移动性管理

      3）空闲移动终端的跟踪和可达TA List管理

      4）P-GW和S-GW选择

      5）MME和SGSN选择

      6）合法监听，漫游控制，安全认证，承载管理

    - SGW（Serving Gateway）

      核心网的服务网关，是数据面的网元，数据面可以理解为数据传输的处理通道，负责本地网络用户数据处理。功能列表：

      1）eNodeB之间切换的本地锚点

      2）E-UTRAN空闲模式下数据缓存，触发网络侧ServiceRequest流程

      3）数据包路由和转发

      4）上下行传输层数据包标记

    - PGW （PDN Gateway）

      PDN（Public Data Network）公共数据网, 充当外部数据连接的边界，核心网后端的网络系统。P-GW负责了UE的IP地址的分配工作，同时提供IP路由和转发的功能。功能列表：

      1）用户包过滤

      2）IP地址分配

      3）上下行传输层数据包标记

      4）Non-GBR基于AMBR的下行速率控制

      5）GBR基于MBR的下行速率控制

    - HSS（Home Subscriber Server）

      归属用户服务器，这是存在与核心网中的一个数据库服务器，里面存放着所有属于该核心网的用户的数据信息。当用户连接到MME的时候，用户提交的资料会和HSS数据服务器中的资料进行比对来进行鉴权。

    - PCRF（Policy and Charging Rules Function）

      策略与计费规则，它会根据不同的服务制定不同的PCC计费策略。

  - Femtocell ：飞蜂窝是近年来根据3G发展和移动宽带化趋势推出的超小型化移动基站。仅用于家庭网络覆盖。飞蜂窝使用IP协议，通过用户已有的ADSL、LAN等宽带电路连接，远端由专用网关实现从IP网到移动网的联通。相比于Wi-Fi，Femtocell不需要采用支持Wi-Fi的双模手机，用户不需更换手持终端就可得到标准化的和安全的接入。

    ![13](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\13.jpg)

  - 上下行 ：DL -- 下行，数据由基站发送往手机。 UL -- 上行，数据由手机发送往基站。

  - 承载 ：

    LTE是基于承载 Bearer的Qos策略设计，我们把UE到P-GW(PDN-Gateway)之间具有相同Qos的业务流称为一个EPS (Evolved Packet System)承载，EPS承载中UE到eNodeB空口之间的一段称为无线承载（RB， Radio Bearer）； eNodeB到S-GW (ServingGateway)之间的一段称为S1 承载。无线承载与S1 承载统称为E-RAB （Evolved RadioAccess Bearer）——即Uu口和S1承载合称。一个RB上可能同时传输多个业务流，但这些业务流的Qos等级是相同的。实际上，RB是eNodeB为UE分配的一系列协议实体及配置的总称，包括PDCP协议实体、RLC协议实体、MAC协议实体和PHY分配的一系列资源等。

    ​       无线承载根据承载的内容不同分为SRB (Signaling Radio Bearer)和DRB (Data RadioBearer)。SRB是系统的信令消息实际传输的通道，DRB是用户数据实际传输的通道。

    - SRB

      LTE中，SRB（signalling radio bearers—信令无线承载）作为一种特殊的无线承载（RB），仅仅用来传输RRC和NAS消息，在协议36.331中，定义了SRBs的传输信道：

      - SRB0用来传输RRC消息，在逻辑信道CCCH上传输；
      - SRB1用来传输RRC消息（也许会包含piggybacked NAS消息），在SRB2承载的建立之前，比SRB2具有更高的优先级。在逻辑信道DCCH上传输；
      - SRB2用来传输NAS消息，比SRB1具有更低的优先级，并且总是在安全模式激活之后才配置SRB2。在逻辑信道DCCH上传输。

    - 默认承载：一种满足默认QoS（服务质量）的数据的用户承载，用于数据量小且实时性低的数据业务。默认承载可以简单地理解为一种提供尽力而为的IP连接的承载，随着PDN连接的完成而建立，随着PDN的连接的释放而销毁，为用户提供永久在线的IP传输服务。

      专用承载：专用承载是在PDN连接的基础上建立的（也就是在默认承载基础上），是为了提供某种特定的QoS传输需求而建立的（默认承载无法满足的）。一般情况下专用承载的QoS比默认承载的QoS要求高

  - SRS ：

    上行探测参考信号。eNB使用SRS来估计不同频段的上行信道质量。分为周期性和非周期性。

  - random waypoint module

    在RWP中，初始状态时，结点在整个仿真区域内服从均匀分布，结点首先从二维仿真区域中随机选择一个结点作为目的地，然后从[Vmin, Vmax]中随机选择一个速度（服从均匀分布），结点以此速度向目的地运动。在到达目的地后，结点在[0, Pmax]中随机选择一段停留时间T，然后选择下一个目的地。

    另外，RWP中存在密度波(density wave)的现象，具体来说就是结点会随着时间的推移表现出非均匀分布，在仿真区域的中心处达到最大，而在边界处密度趋于0。

  - CSG：

    CSG是3GPP R8中引入的概念，定义为闭合用户组。每个CSG由一个CSG ID标识，同一用户可属于多个CSG，UE维护一张它所属CSG的CSG ID列表，在这个列表之外的其他CSG ID所对应的CSG小区对该UE而言是不可访问的。并且每个CSG小区广播一个CSG ID，这个CSG ID所标识的闭合用户群的成员可以访问该小区；CSG模式需要终端和核心网的支持，在R8之前的终端和核心网都无法使用CSG功能。说白了CSG小区就是为家庭基站服务的，家庭基站的用户只能访问开户的时候特定区域的CSG小区，其他小区不能访问。

### 实验实践

1. #### 代码

   以`src/lte/examples/lena-dual-stripe.cc`为例，并更改一些参数

   - 小区个数：7

     ```c++
     static ns3::GlobalValue g_nMacroEnbSites ("nMacroEnbSites",
                             				"How many macro sites there are",      										     ns3::UintegerValue (7), 
        										 ns3::MakeUintegerChecker<uint32_t> ());
     ```

   - 基站间距：500 m

     ```c++
     static ns3::GlobalValue g_interSiteDistance ("interSiteDistance",
                                     "distance between two nearby macro cell sites",
                                      ns3::DoubleValue (500),
                                      ns3::MakeDoubleChecker<double> ());
     ```

   - 天线高度：32 m

     ```c++
     lteHexGridEnbTopologyHelper->SetAttribute ("SiteHeight", DoubleValue (32));
     ```

   - 天线波束宽度：70°

     ```c++
     lteHelper->SetEnbAntennaModelAttribute ("Beamwidth", DoubleValue (70));
     ```

   - 天线最大衰减：25 dB

     ```c++
     lteHelper->SetEnbAntennaModelAttribute ("MaxAttenuation", DoubleValue (25.0));
     ```

   - 基站发射功率：46 dBm

     ```c++
     static ns3::GlobalValue g_macroEnbTxPowerDbm ("macroEnbTxPowerDbm",
                                               "TX power [dBm] used by macro eNBs",
                                               ns3::DoubleValue (46.0), 
                                               ns3::MakeDoubleChecker<double> ());
     ```

   - 路径损耗模型：128.1+37.6log10R，where R in km

     ```c++
     lteHelper->SetAttribute ("PathlossModel", StringValue("ns3::HybridBuildingsPropagationLossModel"));
     ```

   - 用户设备高度：1.5 m

     ```c++
     double ueZ = 1.5;
     ```

   - 系统带宽：20 MHz

     在这里不能直接将值改为20.参考文件得知25对应的是5 MHz DL and UL bandwidth

     ```c++
     static ns3::GlobalValue g_macroEnbBandwidth ("macroEnbBandwidth",
                                             "bandwidth [num RBs] used by macroeNBs",
                                             ns3::UintegerValue (50), 
                                             ns3::MakeUintegerChecker<uint16_t> ());
     static ns3::GlobalValue g_homeEnbBandwidth ("homeEnbBandwidth",
                                               "bandwidth [num RBs] used by HeNBs",
                                               ns3::UintegerValue (50),
                                               ns3::MakeUintegerChecker<uint16_t>());
     ```

   - 基站调度方法：Round Robin

     ```c++
     lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
     ```

     轮询调度算法(Round-Robin Scheduling)

     轮询调度算法的原理是每一次把来自用户的请求轮流分配给内部中的服务器，从1开始，直到N(内部服务器个数)，然后重新开始循环。

2. #### 模块介绍

   - 参数设定

     ```c++
     Ptr <LteHelper> lteHelper = CreateObject<LteHelper> ();
       // 路径损失模型设定 -- 混合建筑传播损失模型
       lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
       lteHelper->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0)); // 室外到室内通信的外墙穿透引起的阴影的标准偏差（默认为5.0）
       lteHelper->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (1)); // 室外节点阴影的标准偏差（默认值7.0）
       lteHelper->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (1.5)); // 室内节点阴影的标准偏差（默认为8.0）
       // use always LOS model
       lteHelper->SetPathlossModelAttribute ("Los2NlosThr", DoubleValue (1e6)); //阈值
       lteHelper->SetSpectrumChannelType ("ns3::MultiModelSpectrumChannel"); // 设置要在DL和UL中使用的频谱通道的类型
       lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler"); // 设置调度算法
     ```

   - 建立宏站点

     ```c++
     mobility.Install (macroEnbs);
       BuildingsHelper::Install (macroEnbs);
       Ptr<LteHexGridEnbTopologyHelper> lteHexGridEnbTopologyHelper = CreateObject<LteHexGridEnbTopologyHelper> ();
       lteHexGridEnbTopologyHelper->SetLteHelper (lteHelper);
       lteHexGridEnbTopologyHelper->SetAttribute ("InterSiteDistance", DoubleValue (interSiteDistance)); // 设置站点间距离
       lteHexGridEnbTopologyHelper->SetAttribute ("SiteHeight", DoubleValue (32)); // 设置天线高度
       lteHexGridEnbTopologyHelper->SetAttribute ("MinX", DoubleValue (interSiteDistance/2));
       lteHexGridEnbTopologyHelper->SetAttribute ("GridWidth", UintegerValue (nMacroEnbSitesX));
       Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (macroEnbTxPowerDbm));
       lteHelper->SetEnbAntennaModelType ("ns3::ParabolicAntennaModel"); // 基于主瓣辐射方向图抛物线近似的天线模型，被用于在蜂窝系统中去模拟小区扇区的辐射模式
       lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (70)); // 半功率角
       lteHelper->SetEnbAntennaModelAttribute ("MaxAttenuation",     DoubleValue (25.0)); // 最大衰减
       lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (macroEnbDlEarfcn)); // E-UTRA绝对射频频道号
       lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (macroEnbDlEarfcn + 18000));
       lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (macroEnbBandwidth));
       lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (macroEnbBandwidth));
       NetDeviceContainer macroEnbDevs = lteHexGridEnbTopologyHelper->SetPositionAndInstallEnbDevice (macroEnbs);
     ```

   - 建立宏用户

     ```c++
     mobility.SetMobilityModel ("ns3::SteadyStateRandomWaypointMobilityModel");
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::MinX", DoubleValue (macroUeBox.xMin));
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::MinY", DoubleValue (macroUeBox.yMin));
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::MaxX", DoubleValue (macroUeBox.xMax));
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::MaxY", DoubleValue (macroUeBox.yMax));
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::Z", DoubleValue (ueZ));
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::MaxSpeed", DoubleValue (outdoorUeMaxSpeed));
     Config::SetDefault ("ns3::SteadyStateRandomWaypointMobilityModel::MinSpeed", DoubleValue (outdoorUeMinSpeed));
     positionAlloc = CreateObject<RandomBoxPositionAllocator> ();
     mobility.SetPositionAllocator (positionAlloc);
     mobility.Install (macroUes);
     ```

   - 建立远程服务器

     ```c++
     NodeContainer remoteHostContainer;
     remoteHostContainer.Create (1);
     remoteHost = remoteHostContainer.Get (0);
     InternetStackHelper internet;
     internet.Install (remoteHostContainer);
     ```

   - 建立PDN和PWG之间的p2p连接

     ```c++
     PointToPointHelper p2ph;
     p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
     p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
     p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
     Ptr<Node> pgw = epcHelper->GetPgwNode (); // PDN网关节点
     NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
     Ipv4AddressHelper ipv4h;
     ipv4h.SetBase ("1.0.0.0", "255.0.0.0"); //pgw和pdn的局域网
     Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
     // in this container, interface 0 is the pgw, 1 is the remoteHost
     remoteHostAddr = internetIpIfaces.GetAddress (1);
     ```

   - 进行数据发送

     ```c++
     cout << "installing UDP DL app for UE " << u << endl;
     UdpClientHelper dlClientHelper (ueIpIfaces.GetAddress (u), dlPort);
     clientApps.Add (dlClientHelper.Install (remoteHost));
     PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
     serverApps.Add (dlPacketSinkHelper.Install (ue));
     ```

   - 生成REM图

     ```c++
     PrintGnuplottableBuildingListToFile ("buildings.txt");
     PrintGnuplottableEnbListToFile ("enbs.txt");
     PrintGnuplottableUeListToFile ("ues.txt");
     remHelper = CreateObject<RadioEnvironmentMapHelper> ();
     remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/0"));
     remHelper->SetAttribute ("OutputFile", StringValue ("lte.rem"));
     remHelper->SetAttribute ("XMin", DoubleValue (macroUeBox.xMin));
     remHelper->SetAttribute ("XMax", DoubleValue (macroUeBox.xMax));
     remHelper->SetAttribute ("YMin", DoubleValue (macroUeBox.yMin));
     remHelper->SetAttribute ("YMax", DoubleValue (macroUeBox.yMax));
     remHelper->SetAttribute ("Z", DoubleValue (1.5)); // 用户设备高度
     if (remRbId >= 0)
     {
         remHelper->SetAttribute ("UseDataChannel", BooleanValue (true));
         remHelper->SetAttribute ("RbId", IntegerValue (remRbId));
     }
     remHelper->Install ();
     ```

     

3. #### 实践结果

   - 运行

   ```bash
   ./waf --run=scratch/lte --vis
   ```

   ![res3_0](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res3_0.png)

   未连线的散列点为宏用户，正六边形上的七个点为宏基站左上角连线点为PGW，右下角连线点为远程终端，象征PDN。

   - 新建lte-plot

   ```bash
   set view map;
   set xlabel "X"
   set ylabel "Y"
   set cblabel "SINR(dB)"
   unset key
   plot "lte.rem" using ($1):($2):(10*log10($4)) with image
   ```

   - 图形化展示REM

   ```bash
   gnuplot -p enbs.txt ues.txt buildings.txt lte-plot
   ```

   ![res3_1](C:\Users\猫猫\Documents\XiaoMiNet\Upupoo\Docker\config\CLASS\网络通信\Telecommunication\pictures\res3_1.png)

   右上角黑色线框为建筑，散列的白点为用户，七个信号辐射中心为宏基站，建筑内的小范围辐射为家庭基站。

------

## 总结

​	虽然是在前人的基础上进行修改实现的，但是也看懂了LTE的结构框架，并且对每一行代码都进行了解读。

