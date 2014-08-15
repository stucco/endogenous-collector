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
*/
#ifndef ENHANCEDPACKETBLOCK_H
#define ENHANCEDPACKETBLOCK_H
#include <QDataStream>
#include <QMap>
#include "Block.h"
#include "OptionField.h"
#include "PacketNG.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT EnhancedPacketBlock : public Block
{
public:
    enum Direction {INCOMING = 1, OUTGOING = 2, UNKNOWN = 0};

private:
  QDataStream::ByteOrder m_byteOrder;
  quint32 m_captureLen;
  quint32 m_interfaceID;
  qint64 m_internalConnID;
  quint64 m_connID;
  QMap<quint16,OptionField> m_options;
  PacketNG m_packet;
  quint32 m_packetLen;
  QByteArray m_rawData;
  quint64 m_timestamp;
  quint32 m_pid;
  //quint32 m_timestampHigh;
  //quint32 m_timestampLow;

public:
  EnhancedPacketBlock();
  EnhancedPacketBlock(QByteArray rawData, QDataStream::ByteOrder byteOrder);
  ~EnhancedPacketBlock();

  void Clear() {m_options.clear();if(!m_rawData.isNull()){m_rawData.clear();}}
  EnhancedPacketBlock::Direction GetDirection();
  quint32 GetCaptureLength() {return m_captureLen;}
  quint32 GetConnectionID();
  QByteArray GetFullBlock() {return m_header.GetRawData().append(m_rawData.append(m_footer.GetRawData()));}
  quint32 GetInterfaceID() {return m_interfaceID;}
  qint64 GetInternalConnectionID() {return m_internalConnID;}
  QMap<quint16,OptionField> GetOptions() {return m_options;}
  PacketNG& GetPacket() {return m_packet;}
  quint32 GetPacketLength() {return m_packetLen;}
  quint32 GetProcessID();
  quint64 GetTimestamp() {return m_timestamp;}
  QByteArray GetRawData() {return m_rawData;}
  void ReadBody(QDataStream &in);
  void SetByteOrder(QDataStream::ByteOrder order) {m_byteOrder = order;}
  void SetCaptureLength(quint32 length) {m_captureLen = length;}
  void SetInterfaceID(quint32 interfaceID) {m_interfaceID = interfaceID;}
  void SetInternalConnectionID(qint64 internalConnID) {m_internalConnID = internalConnID;}
  void SetOptions(QMap<quint16,OptionField> options) {m_options = options;}
  void SetPacket(PacketNG packet) {m_packet = packet;}
  void SetPacketLength(quint32 packetLen) {m_packetLen = packetLen;}
  void SetTimestamp(quint64 timestamp) {m_timestamp = timestamp;}

};
typedef QList<EnhancedPacketBlock*> EnhancedPacketList;
QDataStream& operator>>(QDataStream& in, EnhancedPacketBlock& enhancedPacket);
#endif //ENHANCEDPACKETBLOCK_H
