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

#ifndef SECTIONNG_H
#define SECTIONNG_H

#include <QDebug>
#include "PcapNgLibGlobal.h"
#include "InterfaceNG.h"
#include "ProcessNG.h"
#include "SectionHeaderBlock.h"
#include "InterfaceDescriptionBlock.h"

class PCAPNGLIB_EXPORT SectionNG
{
private:
  SectionHeaderBlock* m_block;
  QList<InterfaceNG*> m_interfaces;
  QMap<quint32, ProcessNGList*> m_processes;

public:
  SectionNG(void);
  ~SectionNG(void);

  void AddConnectionBlock(ConnectionEventBlock *connevent);
  InterfaceNG* AddInterfaceBlock(InterfaceDescriptionBlock *interfaceDescrip);
  void AddPacketBlock(EnhancedPacketBlock *packet);
  void AddProcessBlock(ProcessEventBlock *procevent);

  SectionHeaderBlock* GetBlock() const {return m_block;}
  QList<InterfaceNG*> GetInterfaces() const {return m_interfaces;}
  const QMap<quint32, ProcessNGList*>& GetProcessMap() const {return m_processes;}
  QPair<quint32, quint32> GetTimeResolution();
  void SetBlock(SectionHeaderBlock* block) {m_block = block;}

private:
  bool AssignPacket(ProcessNGList* proclist, EnhancedPacketBlock* packet);
};
#endif // SECTIONNG_H
