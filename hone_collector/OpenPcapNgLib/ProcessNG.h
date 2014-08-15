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
#ifndef PROCESSNG_H
#define PROCESSNG_H

#include <QDebug>
#include "PcapNgLibGlobal.h"
#include "ProcessEventBlock.h"
#include "ConnectionNG.h"
#include "EnhancedPacketBlock.h"
#include "ConnectionEventBlock.h"

class PCAPNGLIB_EXPORT ProcessNG
{
private:
  QList<ProcessEventBlock *> m_blocks;
  QMap<quint32, ConnectionNGList*> m_connections;

public:
  ProcessNG(void);
  ~ProcessNG(void);

  void AddBlock(ProcessEventBlock *procevent){m_blocks.append(procevent);}
  void AddConnectionBlock(ConnectionEventBlock *connevent);
  void AddPacket(EnhancedPacketBlock *packet);
  QList<ProcessEventBlock *> GetBlocks(){return m_blocks;}
  ConnectionNGList GetConnections();
  QMap<quint32, ConnectionNGList*> &GetConnectionMap(){return m_connections;}
  
};
typedef QList<ProcessNG *> ProcessNGList;
#endif //PROCESSNG_H
