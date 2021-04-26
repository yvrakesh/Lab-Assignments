// Extended this from third.cc which is default file given under examples/tutorial
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include <cmath>
// For NetAnim Animation
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Wifi two way communication");

int main (int argc, char *argv[]){
    // Since we are unaware where its range run out
	for(int i=0;i<60;i++){

		bool verbose = true, tracing = true;
		uint32_t nWifi = 2;
		if (verbose)
		{
            // In order to take logs
            // Logs can be taken in different types as per user requirements
            // log_level_all, debug, error, function, info, logic, warn
            // U can select to what extend you need
			LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
			LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
		}
	
        // Create 2 base stationary nodes and 1 Access point node
		NodeContainer wifiStaNodes, wifiApNode;
		wifiStaNodes.Create (nWifi);
		wifiApNode.Create (1);
	
        // Choosing technology channel to communicate
		YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
		YansWifiPhyHelper phy;
		phy.SetChannel (channel.Create ());
	
		WifiHelper wifi;
		wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
	
		WifiMacHelper mac;
		Ssid ssid = Ssid ("ns-3-ssid");
		mac.SetType ("ns3::StaWifiMac","Ssid", SsidValue (ssid),"ActiveProbing", BooleanValue (false));
	
        // Install Channels on node
		NetDeviceContainer staDevices;
		staDevices = wifi.Install (phy, mac, wifiStaNodes);
		mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid));
	
		NetDeviceContainer apDevice;
		apDevice = wifi.Install (phy, mac, wifiApNode);
	

		MobilityHelper mobility;
		mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
										"MinX", DoubleValue (0.0),
										"MinY", DoubleValue (0.0),
										"DeltaX", DoubleValue (5.0),
										"DeltaY", DoubleValue (5.0),
										"GridWidth", UintegerValue (3),
										"LayoutType", StringValue ("RowFirst"));
		mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
		mobility.Install (wifiApNode);
		mobility.Install (wifiStaNodes);
	
		// Set up Internet stack on nodes
        InternetStackHelper stack;
		stack.Install (wifiApNode);
		stack.Install (wifiStaNodes);
	
        // Assign IP address to communicate
		Ipv4AddressHelper address;
		Ipv4InterfaceContainer wifiInterfaces, wifiApInterface;
	
		address.SetBase ("192.168.2.0", "255.255.255.0");
		wifiApInterface = address.Assign (apDevice);
		wifiInterfaces = address.Assign (staDevices);
	
		UdpEchoServerHelper echoServer (9);
		ApplicationContainer serverApps = echoServer.Install (wifiApNode.Get (0));
		serverApps.Start (Seconds (1.0));
		serverApps.Stop (Seconds (10.0));
	
		UdpEchoClientHelper echoClient (wifiApInterface.GetAddress (0), 9);
	
		echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
		echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
		echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
	
		ApplicationContainer clientApp1 = echoClient.Install (wifiStaNodes.Get (0));
		clientApp1.Start (Seconds (2.0));
		clientApp1.Stop (Seconds (10.0));
	
		ApplicationContainer clientApp2 = echoClient.Install (wifiStaNodes.Get (1));
		clientApp2.Start (Seconds (2.0));
		clientApp2.Stop (Seconds (10.0));
	
		Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
		Simulator::Stop (Seconds (10.0));
	
		AnimationInterface anim ("wifi.xml");
		anim.SetConstantPosition(wifiApNode.Get(0), 100.0, 100.0);
		anim.SetConstantPosition(wifiStaNodes.Get(0), 100.0+i, 100.0);
		anim.SetConstantPosition(wifiStaNodes.Get(1), 100.0-i, 100.0);
	
        if(tracing == true){
            phy.EnablePcap("wifi",apDevice);
        }
		printf("\n-------------------\n");
	
		Simulator::Run ();

		printf("Node 1 distance: %lf\n", sqrt(mobility.GetDistanceSquaredBetween (wifiStaNodes.Get(0), wifiApNode.Get(0))));
		printf("Node 2 distance: %lf\n", sqrt(mobility.GetDistanceSquaredBetween (wifiStaNodes.Get(1), wifiApNode.Get(0))));
		fflush(stdout);
	
		Simulator::Destroy ();
	}
	return 0;
}