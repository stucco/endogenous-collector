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
#ifndef SIMPLEPACKETBLOCK_H
#define SIMPLEPACKETBLOCK_H
#include <QDataStream>
#include "Block.h"
#include "OptionField.h"
#include "PacketNG.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT SimplePacketBlock :
  public Block
{
private:
  quint32 m_packetLen;
  PacketNG m_packet;
  QByteArray m_rawData;

public:
  SimplePacketBlock(void);
  ~SimplePacketBlock(void);
  void Clear(){m_rawData.clear();}
  QByteArray GetFullBlock(){return m_header.GetRawData().append(m_rawData.append(m_footer.GetRawData()));}
  quint32 GetPacketLength(){return m_packetLen;}
  PacketNG GetPacket(){return m_packet;}
  void ReadBody(QDataStream &in);
  void SetPacketLength(quint32 length){m_packetLen = length;}
  void SetPacket(PacketNG packet){m_packet = packet;}
};

QDataStream &operator>>(QDataStream &in, SimplePacketBlock &simplePacket);
QDataStream &operator>>(QDataStream &in, SimplePacketBlock *simplePacket);

#endif //SIMPLEPACKETBLOCK_H
