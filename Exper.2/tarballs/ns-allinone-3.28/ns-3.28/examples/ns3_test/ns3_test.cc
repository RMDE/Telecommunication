
#include "ns3/internet-stack-helper.h"
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
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
        std::cout<<"InternetStackHelper::Install name="<<obj->GetInstanceTypeId().GetName()<<std::endl;
    }
}

