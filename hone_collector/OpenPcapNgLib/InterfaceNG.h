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

#ifndef INTERFACENG_H
#define INTERFACENG_H

#include <QDebug>
#include "PcapNgLibGlobal.h"
#include "ConnectionNG.h"
#include "InterfaceDescriptionBlock.h"
#include "SimplePacketBlock.h"
#include "ProcessNG.h"

class PCAPNGLIB_EXPORT InterfaceNG
{
private:
  InterfaceDescriptionBlock *m_block;
  QList<SimplePacketBlock *> m_simpleblocks;

public:
  InterfaceNG(void);
  ~InterfaceNG(void);

  void AddSimplePacket(SimplePacketBlock *packet){m_simpleblocks.append(packet);}
  InterfaceDescriptionBlock *GetBlock(){return m_block;}
  QList<SimplePacketBlock *> GetSimpleBlocks() const{return m_simpleblocks;}
  void SetBlock(InterfaceDescriptionBlock *block){m_block = block;}
};
#endif //INTERFACENG_H
