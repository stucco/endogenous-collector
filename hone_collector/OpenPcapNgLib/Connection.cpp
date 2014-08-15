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
#include "Connection.h"

//------------------------------------------------------------------------------
// Connection
//
Connection::Connection(void):
  m_updated(false), 
  m_connectionID(0),
  m_destport(0),
  m_sourceport(0),
  m_timestamp(0),
  m_version(0),
  m_bytecount(0),
  m_packetcount(0),
  m_processID(0),
  m_linkType(0),
  m_protocol(0),
  m_isIncoming(false),
  m_lastpackettime(0)
{
}
//------------------------------------------------------------------------------
// Connection
//
Connection::Connection(Connection& other)
{
  m_connectionID = other.m_connectionID;
  m_destaddress = other.m_destaddress;
  m_destport = other.m_destport;
  m_sourceaddress = other.m_sourceaddress;
  m_sourceport = other.m_sourceport;
  m_state = other.m_state;
  m_timestamp = other.m_timestamp;
  m_updated = other.m_updated;
  m_version = other.m_version;
  m_bytecount = other.m_bytecount;
  m_packetcount = other.m_packetcount;
  m_processID = other.m_processID;
  m_linkType = other.m_linkType;
  m_protocol = other.m_protocol;
  m_isIncoming = other.m_isIncoming;
  m_lastpackettime = other.m_lastpackettime;
}

//------------------------------------------------------------------------------
// ~Connection
//
Connection::~Connection(void)
{
  
}

void Connection::SetFromBlock(ConnectionEventBlock& connblock)
{
  m_state = connblock.GetEvent();
  m_timestamp = connblock.GetTimestamp();
  m_connectionID = connblock.GetConnectionID();
  m_processID = connblock.GetProcessID();
}


void Connection::SetPacketLevelInfo(EnhancedPacketBlock& packetblock)
{
  if (! m_updated)
  {
    PacketNG packet = packetblock.GetPacket();
    m_sourceport = packet.GetSourcePort();
    m_destport = packet.GetDestPort();
    m_version = packet.GetVersion();
    m_sourceaddress = packet.GetSourceAddress();
    m_destaddress = packet.GetDestAddress();
    m_linkType = packet.GetLinkType();
    m_protocol = packet.GetProtocol();
    m_lastpackettime = packetblock.GetTimestamp();

    if (packetblock.GetDirection() == EnhancedPacketBlock::INCOMING)
    {
      m_isIncoming = true;
    }
    else
    {
      m_isIncoming = false;
    }
    m_updated = true;
  }
}

void Connection::AddPacketData(EnhancedPacketBlock& packetblock)
{
  m_packetcount +=1;
  m_bytecount += packetblock.GetPacketLength();
  SetPacketLevelInfo(packetblock);
  if (packetblock.GetTimestamp() > m_lastpackettime)
  {
    m_lastpackettime = packetblock.GetTimestamp();
  }
}
