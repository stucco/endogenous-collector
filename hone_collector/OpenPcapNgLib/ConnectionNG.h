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

#ifndef CONNECTIONNG_H
#define CONNECTIONNG_H

#include <QDebug>
#include "PcapNgLibGlobal.h"
#include "ConnectionEventBlock.h"
#include "EnhancedPacketBlock.h"

class PCAPNGLIB_EXPORT ConnectionNG
{

private:
  QList<ConnectionEventBlock *> m_blocks;
  QList<EnhancedPacketBlock *> m_packets;
  ConnectionEventBlock::Event m_state;

public:
  ConnectionNG(void);
  ~ConnectionNG(void);

  void AddBlock(ConnectionEventBlock *block){m_blocks.append(block);}
  void AddPacket(EnhancedPacketBlock *packet){m_packets.append(packet);}
  QList<ConnectionEventBlock *> GetBlocks() const{return m_blocks;}
  quint32 GetConnectionID();
  QList<EnhancedPacketBlock *> GetPackets() const{return m_packets;}
  ConnectionEventBlock::Event GetState() const{return m_state;}
  void SetState(ConnectionEventBlock::Event state){m_state = state;}

};
typedef QList<ConnectionNG *> ConnectionNGList;
#endif //CONNECTIONNG_H
