//-------------------------------------------------------------------------------
//  This material was prepared as an account of work sponsored by an agency of
//  the United States Government.  Neither the United States Government nor the
//  United States Department of Energy, nor the Contractor, nor any or their
//  employees, nor any jurisdiction or organization that has cooperated in the
//  development of these materials, makes any warranty, express or implied, or
//  assumes any legal liability or responsibility for the accuracy, completeness,
//  or usefulness or any information, apparatus, product, software, or process
//  disclosed, or represents that its use would not infringe privately owned
//  rights.
//
//  Created By: Daniel Best
//              Lance Irvine
//
#include <QDebug>
#include <QStringBuilder>
#include "PacketNG.h"
//------------------------------------------------------------------------------
// Packet
//
PacketNG::PacketNG(void)
{
}
//------------------------------------------------------------------------------
// ~Packet
//
PacketNG::~PacketNG(void)
{
}



QString PacketNG::GetTcpDumpString()
{
  return m_tcpstring;
}

//------------------------------------------------------------------------------
// SkipToHeader
//
void PacketNG::SkipToHeader(QDataStream &stream)
{
    switch(m_linkType)
    {
      case LINKTYPE_ETHERNET:
        stream.skipRawData(14);
        break;
      case LINKTYPE_RAW:
        // do nothing
        break;
    }
}

void PacketNG::ParseIPv4Data()
{
    quint32 seqno;
    quint16 length;
    quint32 ackno;


    quint16 offResControl;
    quint16 controlbits;
    quint8 dataoff;

    quint16 window;
    quint16 checksum;
    quint16 urgent;
    QString flags = "";

    QDataStream streamData(m_rawData);
    streamData.setByteOrder(QDataStream::BigEndian);
    SkipToHeader(streamData);
    quint8 headerLen;

    streamData >> headerLen;
    m_version = headerLen >> 4;
    headerLen = headerLen & 0x0F;

    streamData.skipRawData(1); // skip differentiated services code point and congestion notification
    streamData >> m_totalLen;
    streamData.skipRawData(5); // skip identification, flags, fragment offset, and time to live
    streamData >> m_protocol;
    streamData.skipRawData(2); // skip header checksum

    // source
    quint32 srcIpint;
    streamData >> srcIpint;
    m_srcAddress.setAddress(srcIpint);

    // destintation
    quint32 dstIpint;
    streamData >> dstIpint;
    m_destAddress.setAddress(dstIpint);

    // skip to tcp / udp header
    if(headerLen > 5)
    {
      streamData.skipRawData((headerLen - 5) * 4);
    }
    switch(m_protocol)
    {
    case 6:
      // tcp
      streamData >> m_srcPort;
      streamData >> m_destPort;
      streamData >> seqno;
      streamData >> ackno;
      streamData >> offResControl;
      streamData >> window;
      streamData >> checksum;
      streamData >> urgent;

      dataoff = offResControl >> 12; // just get the data offset portion
      controlbits = offResControl & 0x1FF; // just get the control bits portion

      // the first bit of the reserved is not there since I pushed dataoff into a 16 bit integer
      // check the flags using only 2 bits as 0 at the beginning
      if(controlbits & 0x20)
      {
        flags.append("U");
      }
      if(controlbits & 0x10)
      {
        flags.append("A");
      }
      if(controlbits & 0x8)
      {
        flags.append("P");
      }
      if(controlbits & 0x4)
      {
        flags.append("R");
      }
      if(controlbits & 0x2)
      {
        flags.append("S");
      }
      if(controlbits & 0x1)
      {
        flags.append("F"); 
     }
      // use stringbuilder by using % instead of + for concatination
      m_tcpstring =  GetSourceAddress().toString() % "." % QString::number(m_srcPort) % " > " % GetDestAddress().toString() % "." % QString::number(m_destPort) % " Flags [" % flags % "], seq " % QString::number(seqno) % ", ack " % QString::number(ackno) % ", win " % QString::number(window);
      break;
    case 17:
      // udp
      streamData >> m_srcPort;
      streamData >> m_destPort;
      streamData >> length;
      // use stringbuilder by using % instead of + for concatination
      m_tcpstring = GetSourceAddress().toString() % "." % QString::number(m_srcPort) % " > " % GetDestAddress().toString() % "." % QString::number(m_srcPort) % ": udp " % QString::number(length);
      break;
    case 1:
      // icmp
      quint8 type;
      quint8 code;
      streamData >> type;
      streamData >> code;
      m_srcPort = 0;
      m_destPort = 0;
      m_tcpstring = GetSourceAddress().toString() % " > " % GetDestAddress().toString() % ": icmp " % QString::number(type);
      break;
    }

}

void PacketNG::ParseIPv6Data()
{
  //Unused quint16 sourceport;
  //Unused quint16 destport;
  quint32 seqno;
  quint32 ackno;

  quint16 offResControl;
  quint16 controlbits;
  quint8 dataoff;

  quint16 window;
  quint16 checksum;
  quint16 urgent;
  QString ret = "";
  QString flags = "";
  quint16 udplen;

  quint8 nheader = 0;
  quint8 length = 0;

  if(m_rawData.isNull() || m_rawData.isEmpty()){return;}

  QDataStream streamData(m_rawData);
  streamData.setByteOrder(QDataStream::BigEndian);
  SkipToHeader(streamData);

  //streamData.skipRawData(4); // version, traffic class, flow label
  quint32 firstline;

  streamData >> firstline;
  //qDebug() << "version: " << (firstline  >> 28);
  //qDebug() << "class: " << ((firstline << 4) >> 20);

  streamData >> m_totalLen;
  //qDebug() << "length: " << m_totalLen;
  streamData >> nheader;

  // skip to the start of the IP addresses
  quint8 hoplimit;
  streamData >> hoplimit;
  //qDebug() << "hop limit: " << hoplimit;
  //streamData.skipRawData(1);  // hop limit

  Q_IPV6ADDR source;
  for(int i = 0; i < 16; i++)
  {
    streamData >> source[i];
  }
  m_srcAddress.setAddress(source);

  Q_IPV6ADDR dest;
  for(int i = 0; i < 16; i++)
  {
    streamData >> dest[i];
  }
  m_destAddress.setAddress(dest);
  //qDebug() << "starting through the headers, currently with " + QString::number(nheader);
  // walk through the headers and look for TCP, UDP, or ICMP
  while(nheader != 6 && nheader != 17 && nheader != 58)
  {
    //qDebug() << "headers, currently with " + QString::number(nheader);
    if(nheader == 50 || nheader == 59)
    {
      // encryption or end of headers, no protocol found
      return;
    }
    if(nheader == 44)
    {
      // fragment
      streamData >> nheader;
      streamData.skipRawData(7);
    }
    else
    {
      // not sure if correct...
      if(streamData.atEnd()){
        //qDebug() << "At the end of IPv6 stream data";
        return;
      }
      streamData >> nheader;
      streamData >> length;
      streamData.skipRawData(length - 1);
    }
  }
  //qDebug() << "Found header for protocol " + QString::number(nheader);
  // we should be at the beginning of the protocol section

  switch(nheader)
  {
  case 6:
     // tcp
      streamData >> m_srcPort;
      //qDebug() << "source port: " << m_srcPort;
      streamData >> m_destPort;
      //qDebug() << "dest port: " << m_destPort;
      streamData >> seqno;
      //qDebug() << "sequence no: " << seqno;
      streamData >> ackno;
      //qDebug() << "ack no: " << ackno;
      streamData >> offResControl;
      streamData >> window;
      streamData >> checksum;
      streamData >> urgent;

      dataoff = offResControl >> 12; // just get the data offset portion
      controlbits = offResControl & 0x1FF; // just get the control bits portion
      //qDebug() << "control bits: " << controlbits;

      // the first bit of the reserved is not there since I pushed dataoff into a 16 bit integer
      // check the flags using only 2 bits as 0 at the beginning
      if(controlbits & 0x20)
      {
        flags.append("U");
      }
      if(controlbits & 0x10)
      {
        flags.append("A");
      }
      if(controlbits & 0x8)
      {
        flags.append("P");
      }
      if(controlbits & 0x4)
      {
        flags.append("R");
      }
      if(controlbits & 0x2)
      {
        flags.append("S");
      }
      if(controlbits & 0x1)
      {
        flags.append("F"); 
     }
      // use stringbuilder by using % instead of + for concatination
      m_tcpstring =  GetSourceAddress().toString() % "." % QString::number(m_srcPort) % " > " % GetDestAddress().toString() % "." % QString::number(m_destPort) % " Flags [" % flags % "], seq " % QString::number(seqno) % ", ack " % QString::number(ackno) % ", win " % QString::number(window);
    break;
  case 17:
      // udp
      streamData >> m_srcPort;
      streamData >> m_destPort;
      streamData >> udplen;
      // use stringbuilder by using % instead of + for concatination
      m_tcpstring = GetSourceAddress().toString() % "." % QString::number(m_srcPort) % " > " % GetDestAddress().toString() % "." % QString::number(m_srcPort) % ": udp " % QString::number(length);
    break;
  case 58:
    // ICMP
    quint8 type;
    quint8 code;
    streamData >> type;
    streamData >> code;
    m_srcPort = 0;
    m_destPort = 0;
    m_tcpstring = GetSourceAddress().toString() % " > " % GetDestAddress().toString() % ": icmp " % QString::number(type);
    break;
  }
  m_protocol = nheader;
}
void PacketNG::SetValues()
{
  QByteArray destAddress;
  QByteArray srcAddress;
  m_version = 0;
  if (!m_rawData.isNull() && !m_rawData.isEmpty())
  {
    QDataStream versionData(m_rawData);
    versionData.setByteOrder(QDataStream::BigEndian);
    SkipToHeader(versionData);
    quint8 version;
    versionData >> version;
    // get rid of the extra 4 bits
    m_version = version >> 4;
  }
  if(m_version == 4)
  {
    ParseIPv4Data();
  }
  else if(m_version == 6)
  {
    ParseIPv6Data();
  }
}
