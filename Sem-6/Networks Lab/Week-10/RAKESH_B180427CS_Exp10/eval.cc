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
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/on-off-helper.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Wifi two way communication");


void experiment (bool enableCtsRts, std::string wifiManager)
{
  // 0. Enable or disable CTS/RTS
  UintegerValue ctsThr = (enableCtsRts ? UintegerValue (100) : UintegerValue (2200));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

  // 1. Create 3 nodes
  NodeContainer nodes;
  nodes.Create (3);

  // 2. Place nodes somehow, this is required by every wireless simulation
  for (uint8_t i = 0; i < 3; ++i)
    {
      nodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }

  nodes.Get(0)->GetObject<MobilityModel> ()->SetPosition(Vector (100,0,0));
  nodes.Get(1)->GetObject<MobilityModel> ()->SetPosition(Vector (0,0,0));
  nodes.Get(2)->GetObject<MobilityModel> ()->SetPosition(Vector (-100,0,0));

  // 3. Create propagation loss matrix
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (200); // set default loss to 200 dB (no link)
  lossModel->SetLoss (nodes.Get (0)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 0 <-> 1 to 50 dB
  lossModel->SetLoss (nodes.Get (2)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 2 <-> 1 to 50 dB

  // 4. Create & setup wifi channel
  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel> ();
  wifiChannel->SetPropagationLossModel (lossModel);
  wifiChannel->SetPropagationDelayModel (CreateObject <ConstantSpeedPropagationDelayModel> ());

  // 5. Install wireless devices
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::" + wifiManager + "WifiManager");
  YansWifiPhyHelper wifiPhy;
  wifiPhy.SetChannel (wifiChannel);
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac"); // use ad-hoc MAC
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  // uncomment the following to have athstats output
  // AthstatsHelper athstats;
  // athstats.EnableAthstats(enableCtsRts ? "rtscts-athstats-node" : "basic-athstats-node" , nodes);

  // uncomment the following to have pcap output
  // wifiPhy.EnablePcap (enableCtsRts ? "rtscts-pcap-node" : "basic-pcap-node" , nodes);


  // 6. Install TCP/IP stack & assign IP addresses
  InternetStackHelper internet;
  internet.Install (nodes);
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.0.0.0");
  ipv4.Assign (devices);

  // 7. Install applications: two CBR streams each saturating the channel
  ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.0.0.2"), cbrPort));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1400));
  onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  // flow 1:  node 0 -> node 1
  onOffHelper.SetAttribute ("DataRate", StringValue ("3000000bps"));
  onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.000000)));
  cbrApps.Add (onOffHelper.Install (nodes.Get (0)));

  // flow 2:  node 2 -> node 1
  /** \internal
   * The slightly different start times and data rates are a workaround
   * for \bugid{388} and \bugid{912}
   */
  onOffHelper.SetAttribute ("DataRate", StringValue ("3001100bps"));
  onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.001)));
  cbrApps.Add (onOffHelper.Install (nodes.Get (2)));

  /** \internal
   * We also use separate UDP applications that will send a single
   * packet before the CBR flows start.
   * This is a workaround for the lack of perfect ARP, see \bugid{187}
   */
  uint16_t  echoPort = 9;
  UdpEchoClientHelper echoClientHelper (Ipv4Address ("10.0.0.2"), echoPort);
  echoClientHelper.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClientHelper.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClientHelper.SetAttribute ("PacketSize", UintegerValue (10));
  ApplicationContainer pingApps;

  // again using different start times to workaround Bug 388 and Bug 912
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.001)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (0)));
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.006)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (2)));

  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // 9. Run simulation for 10 seconds
  Simulator::Stop (Seconds (10));
  Simulator::Run ();

  // 10. Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      // first 2 FlowIds are for ECHO apps, we don't want to display them
      //
      // Duration for throughput measurement is 9.0 seconds, since
      //   StartTime of the OnOffApplication is at about "second 1"
      // and
      //   Simulator::Stops at "second 10".
      if (i->first > 2)
        {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "Number of Packets Transmitted: " << i->second.txPackets << "\n";
          //std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
        //   std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
          std::cout << "Number of Packets Received: " << i->second.rxPackets << "\n";
        //   std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout << "Packet Loss Ratio: "<<(double)(i->second.txPackets-i->second.rxPackets)/(i->second.txPackets)<<"\n";
          std::cout << "Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n\n";
        }
    }
    std::cout<<"\n";
  // 11. Cleanup
  Simulator::Destroy ();
}

void experiment1 (bool enableCtsRts, std::string wifiManager)
{
  // 0. Enable or disable CTS/RTS
  UintegerValue ctsThr = (enableCtsRts ? UintegerValue (100) : UintegerValue (2200));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

  // 1. Create 3 nodes
  NodeContainer nodes;
  nodes.Create (3);

  // 2. Place nodes somehow, this is required by every wireless simulation
  for (uint8_t i = 0; i < 3; ++i)
    {
      nodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }

  nodes.Get(0)->GetObject<MobilityModel> ()->SetPosition(Vector (100,0,0));
  nodes.Get(1)->GetObject<MobilityModel> ()->SetPosition(Vector (0,0,0));
  nodes.Get(2)->GetObject<MobilityModel> ()->SetPosition(Vector (-100,0,0));


  // 3. Create propagation loss matrix
  Ptr<FriisPropagationLossModel> lossModel = CreateObject<FriisPropagationLossModel> ();
  lossModel->SetMinLoss (50);


  /*Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (200); // set default loss to 200 dB (no link)
  lossModel->SetLoss (nodes.Get (0)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 0 <-> 1 to 50 dB
  lossModel->SetLoss (nodes.Get (2)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 2 <-> 1 to 50 dB*/

  // 4. Create & setup wifi channel
  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel> ();
  wifiChannel->SetPropagationLossModel (lossModel);
  wifiChannel->SetPropagationDelayModel (CreateObject <ConstantSpeedPropagationDelayModel> ());

  // 5. Install wireless devices
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::" + wifiManager + "WifiManager");
  YansWifiPhyHelper wifiPhy;
  wifiPhy.SetChannel (wifiChannel);
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac"); // use ad-hoc MAC
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  // uncomment the following to have athstats output
  // AthstatsHelper athstats;
  // athstats.EnableAthstats(enableCtsRts ? "rtscts-athstats-node" : "basic-athstats-node" , nodes);

  // uncomment the following to have pcap output
  // wifiPhy.EnablePcap (enableCtsRts ? "rtscts-pcap-node" : "basic-pcap-node" , nodes);


  // 6. Install TCP/IP stack & assign IP addresses
  InternetStackHelper internet;
  internet.Install (nodes);
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.0.0.0");
  ipv4.Assign (devices);

  // 7. Install applications: two CBR streams each saturating the channel
  ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.0.0.2"), cbrPort));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1400));
  onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  // flow 1:  node 0 -> node 1
  onOffHelper.SetAttribute ("DataRate", StringValue ("3000000bps"));
  onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.000000)));
  cbrApps.Add (onOffHelper.Install (nodes.Get (0)));

  // flow 2:  node 2 -> node 1
  /** \internal
   * The slightly different start times and data rates are a workaround
   * for \bugid{388} and \bugid{912}
   */
  onOffHelper.SetAttribute ("DataRate", StringValue ("3001100bps"));
  onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.001)));
  cbrApps.Add (onOffHelper.Install (nodes.Get (2)));

  /** \internal
   * We also use separate UDP applications that will send a single
   * packet before the CBR flows start.
   * This is a workaround for the lack of perfect ARP, see \bugid{187}
   */
  uint16_t  echoPort = 9;
  UdpEchoClientHelper echoClientHelper (Ipv4Address ("10.0.0.2"), echoPort);
  echoClientHelper.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClientHelper.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClientHelper.SetAttribute ("PacketSize", UintegerValue (10));
  ApplicationContainer pingApps;

  // again using different start times to workaround Bug 388 and Bug 912
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.001)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (0)));
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.006)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (2)));

  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // 9. Run simulation for 10 seconds
  Simulator::Stop (Seconds (10));
  Simulator::Run ();

  // 10. Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      // first 2 FlowIds are for ECHO apps, we don't want to display them
      //
      // Duration for throughput measurement is 9.0 seconds, since
      //   StartTime of the OnOffApplication is at about "second 1"
      // and
      //   Simulator::Stops at "second 10".
      if (i->first > 2)
        {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "Number of Packets Transmitted: " << i->second.txPackets << "\n";
          //std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
        //   std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
          std::cout << "Number of Packets Received: " << i->second.rxPackets << "\n";
        //   std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout << "Packet Loss Ratio: "<<(double)(i->second.txPackets-i->second.rxPackets)/(i->second.txPackets)<<"\n";
          std::cout << "Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n\n";
        }
    }
    std::cout<<"\n";
  // 11. Cleanup
  Simulator::Destroy ();
}

int main (int argc, char *argv[]){
  // Since we are unaware where its range run out
      std::string wifiManager ("Arf");
      CommandLine cmd (__FILE__);
      cmd.AddValue ("wifiManager", "Set wifi rate manager (Aarf, Aarfcd, Amrr, Arf, Cara, Ideal, Minstrel, Onoe, Rraa)", wifiManager);
      cmd.Parse (argc, argv);
      std::cout << "At Day time (RTS/CTS unicast messages enabled):\n" << std::flush;
      experiment (true, wifiManager);
      std::cout << "--------------------------------------------------------------------\n";
      std::cout << "\nAt night time(RTS/CTS unicast messages disabled):\n";
      experiment (false, wifiManager);

      std::cout<<"\nTrying with different loss mmodel\n\n";
      std::cout << "At Day time (RTS/CTS unicast messages enabled):\n" << std::flush;
      experiment1 (true, wifiManager);
      std::cout << "--------------------------------------------------------------------\n";
      std::cout << "\nAt night time(RTS/CTS unicast messages disabled):\n";
      experiment1 (false, wifiManager);
      return 0;
}