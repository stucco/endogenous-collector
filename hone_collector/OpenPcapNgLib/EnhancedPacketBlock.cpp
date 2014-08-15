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
/*
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +---------------------------------------------------------------+
 0 |                    Block Type = 0x00000006                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                         Interface ID                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                        Timestamp (High)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                        Timestamp (Low)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
20 |                         Captured Len                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
24 |                          Packet Len                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
28 /                                                               /
   /                          Packet Data                          /
   /             variable length, aligned to 32 bits               /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+

   options
   =================================================================
   1:   Comment
   2:   epb_flags
   257: Connection ID
   258: PID 
*/
#include <QDebug>
#include "EnhancedPacketBlock.h"
//------------------------------------------------------------------------------
// EnhancedPacketBlock
//
EnhancedPacketBlock::EnhancedPacketBlock(void): m_connID(0), m_pid(0)
{

}
//------------------------------------------------------------------------------
// EnhancedPacketBlock
//
EnhancedPacketBlock::EnhancedPacketBlock(QByteArray rawData, QDataStream::ByteOrder byteOrder)
{
    //qDebug() << "Big endian: " << QDataStream::BigEndian;
    //qDebug() << "Little endian: " << QDataStream::LittleEndian;
    //qDebug() << "Setting byte order to " << byteOrder;
    m_connID = 0;
    QDataStream data(rawData);
    data.setByteOrder(byteOrder);
    data >> m_header;
    data >> *this;
    data >> m_footer;
}
//------------------------------------------------------------------------------
// ~EnhancedPacketBlock
//
EnhancedPacketBlock::~EnhancedPacketBlock(void)
{
}
//------------------------------------------------------------------------------
// GetDirection
//
EnhancedPacketBlock::Direction EnhancedPacketBlock::GetDirection()
{
  if(m_options.contains(OPTION_DIRECTION))
  {
    quint8 direction;
    QDataStream stream(m_options.value(OPTION_DIRECTION).GetValue());
    stream.setByteOrder(m_options.value(OPTION_DIRECTION).GetByteOrder());
    stream >> direction;
    direction = direction & 0x0F;
    return (EnhancedPacketBlock::Direction)direction;
  }
  else
  {
    return EnhancedPacketBlock::UNKNOWN;
  }
}
//------------------------------------------------------------------------------
// GetConnectionID
//
quint32 EnhancedPacketBlock::GetConnectionID()
{

  return m_connID;
}
//------------------------------------------------------------------------------
// GetProcessID
//
quint32 EnhancedPacketBlock::GetProcessID()
{
  return m_pid;
}
//------------------------------------------------------------------------------
// ReadBody
//
void EnhancedPacketBlock::ReadBody(QDataStream &in)
{
  Clear();
  quint32 timestampHigh;
  quint32 timestampLow;
  // store the raw bytes that were used to initialize the body
  // skipping header and footer
  //qDebug() << "Byte oder is current set to " << in.byteOrder();
  //qDebug() << "EnhancedPacketBlock body length: " << GetHeader().GetLength();
  m_rawData.resize(GetHeader().GetLength() - 12);
  int result = in.readRawData(m_rawData.data(), GetHeader().GetLength() - 12);
  // make sure all the data is there to read in
  if (result < (int)(GetHeader().GetLength() - 12))
  {
    qDebug() << "ERROR: Incomplete packet, header length and read length do not match";
    return; //throw -1;
  }
  m_byteOrder = in.byteOrder();
  // set up the stream for the current position
  QDataStream substream(m_rawData);
  substream.setByteOrder(in.byteOrder());
  substream >> m_interfaceID;
  // timestamp stored as 2 32 bit integers
  substream >> timestampHigh;
  substream >> timestampLow;
  m_timestamp = timestampHigh;
  m_timestamp = (m_timestamp << 32) + timestampLow;

  substream >> m_captureLen;
  substream >> m_packetLen;

  quint32 bitAlignedLen = m_captureLen;
  QByteArray packet;
  packet.resize(m_captureLen);
  substream.readRawData(packet.data(), m_captureLen);
  //qDebug() << "length: " << m_captureLen << "packet: " << packet.toHex();
  m_packet.SetRawData(packet);

  // account for 32 bit allignment
  switch(m_captureLen % 4)
    {
      case 1:
        substream.skipRawData(3);
        bitAlignedLen += 3;
        break;
      case 2:
        substream.skipRawData(2);
        bitAlignedLen +=2;
        break;
      case 3:
        substream.skipRawData(1);
        bitAlignedLen += 1;
        break;
   }

  // everything without options is stored in nooption variable
  if((GetHeader().GetLength() - (bitAlignedLen + 32)) > 0)
  {
    // there are options
    OptionField of;
    do
    {
      substream >> of;
      m_options.insert(of.GetCode(), of);
    } while(of.GetCode() != OPTION_END_OF_OPTION);
  }

  if (m_options.contains(OPTION_CONNECTION_ID))
  {
    m_connID = m_options.value(OPTION_CONNECTION_ID).GetUIntegerValue(32);
  }
  if (m_options.contains(OPTION_PROCESS_ID))
  {
    m_pid = m_options.value(OPTION_PROCESS_ID).GetUIntegerValue(32);
  }
}
//------------------------------------------------------------------------------
// >>
//
QDataStream& operator>>(QDataStream& in, EnhancedPacketBlock& enhancedPacket)
{
    enhancedPacket.ReadBody(in);
    return in;

}
