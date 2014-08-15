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
 0 |                    Block Type = 0x00000003                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                          Packet Len                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 /                                                               /
   /                          Packet Data                          /
   /             variable length, aligned to 32 bits               /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#include "SimplePacketBlock.h"
//------------------------------------------------------------------------------
// SimplePacketBlock
//
SimplePacketBlock::SimplePacketBlock(void)
{
}
//------------------------------------------------------------------------------
// ~SimplePacketBlock
//
SimplePacketBlock::~SimplePacketBlock(void)
{
}
//------------------------------------------------------------------------------
// ReadBody
//
void SimplePacketBlock::ReadBody(QDataStream &in)
{
  Clear();
  //qDebug() << "SimplePacketBlock body length: " << GetHeader().GetLength();
  // store the raw bytes that were used to initialize the body
  // skipping header and footer
  m_rawData.resize(GetHeader().GetLength() - 12);
  int read = in.readRawData(m_rawData.data(), GetHeader().GetLength() - 12);
  if (read < (int)(GetHeader().GetLength() - 12))
  {
    // not all the data is there to read
    throw -1;
  }
  // set up the stream for the current position
  QDataStream substream(m_rawData);
  substream.setByteOrder(in.byteOrder());

  substream >> m_packetLen;
  QByteArray packet;
  packet.resize(m_packetLen);
  substream.readRawData(packet.data(), m_packetLen);
  m_packet.SetRawData(packet);
  // account for 32 bit allignment
  switch(m_packetLen % 4)
    {
      case 1:
        substream.skipRawData(3);
        break;
      case 2:
        substream.skipRawData(2);
        break;
      case 3:
        substream.skipRawData(1);
        break;
   }
}
//------------------------------------------------------------------------------
// >>
//
QDataStream &operator>>(QDataStream &in, SimplePacketBlock &simplePacket)
{
  simplePacket.ReadBody(in);
  return in;
}
QDataStream &operator>>(QDataStream &in, SimplePacketBlock *simplePacket)
{
  simplePacket->ReadBody(in);
  return in;
}
