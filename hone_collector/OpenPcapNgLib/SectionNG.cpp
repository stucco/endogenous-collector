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
#include "SectionNG.h"
//------------------------------------------------------------------------------
// SectionNG
//
SectionNG::SectionNG(void)
{
}
//------------------------------------------------------------------------------
// ~SectionNG
//
SectionNG::~SectionNG(void)
{
  delete m_block;
  qDeleteAll(m_interfaces);
  m_interfaces.clear();

  qDeleteAll(m_processes);
  m_processes.clear();
}
//------------------------------------------------------------------------------
// AddConnectionBlock
// Add a connection to the process it belongs to
//
void SectionNG::AddConnectionBlock(ConnectionEventBlock *connevent)
{
  if(m_processes.contains(connevent->GetProcessID()))
  {
    // if it's a close we need to find the right process
    if(connevent->GetEvent() == ConnectionEventBlock::CLOSE)
    {
      ProcessNGList *proclist = m_processes.value(connevent->GetProcessID());
      if(!proclist->isEmpty())
      {
        // walk backward to find the most current
        int proclistsize = proclist->size();
        for(int i = proclistsize-1; i >= 0; i--)
        {
          if(proclist->at(i)->GetConnectionMap().contains(connevent->GetConnectionID()))
          {
            proclist->at(i)->AddConnectionBlock(connevent);
            //qDebug() << "Added connection " << connevent->GetConnectionID() << " pid " << connevent->GetProcessID();
            break;
          }
        }
      }
    }
    else
    {
      // add the connection to the last process available
      m_processes.value(connevent->GetProcessID())->last()->AddConnectionBlock(connevent);
      //qDebug() << "Added connection " << connevent->GetConnectionID() << " pid " << connevent->GetProcessID();
    }
  }
  else
  {
    //qDebug() << "WARNING: connection without associated process";
  }
}
//------------------------------------------------------------------------------
// AddInterfaceBlock
//
InterfaceNG *SectionNG::AddInterfaceBlock(InterfaceDescriptionBlock *interfaceDescrip)
{
  InterfaceNG *i = new InterfaceNG();
  i->SetBlock(interfaceDescrip);
  m_interfaces.append(i);
  return i;
}
//------------------------------------------------------------------------------
// AddPacketBlock
// Add a packet to the connection it belongs to
//
void SectionNG::AddPacketBlock(EnhancedPacketBlock *packet)
{
  //bool placed = false;
  ProcessNGList *proclist;
  if(m_processes.contains(packet->GetProcessID()))
  {
    proclist = m_processes.value(packet->GetProcessID());
    AssignPacket(proclist, packet);
  }
  else
  {
    // try to find the right connection
    if(!m_processes.isEmpty())
    {
      foreach(ProcessNGList *proclist, m_processes.values())
      {
        if(AssignPacket(proclist, packet))
        {
          break;
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// AddProcessBlock
// Add a process to this interface
//
void SectionNG::AddProcessBlock(ProcessEventBlock *procevent)
{
  switch(procevent->GetEvent())
  {
  case ProcessEventBlock::CHILD:
    if(m_processes.contains(procevent->GetProcessID()))
    {
      // copy the parent argv and path
      ProcessNG *parent = m_processes.value(procevent->GetParentProcessID())->last();
      if(parent->GetBlocks().first()->GetOptions().contains(OPTION_PATH))
      {
        // add the parent option to the child
        procevent->GetOptions().insert(OPTION_PATH, parent->GetBlocks().first()->GetOptions().value(OPTION_PATH));
      }
      if(parent->GetBlocks().first()->GetOptions().contains(OPTION_ARGV))
      {
        // add the parent option to the child
        procevent->GetOptions().insert(OPTION_ARGV, parent->GetBlocks().first()->GetOptions().value(OPTION_ARGV));
      }
    }
    break;
  case ProcessEventBlock::END:
    // we should probably at least do something with the block
    break;
  case ProcessEventBlock::START:
    // we just add it below
    break;
  case ProcessEventBlock::SHAREDLIB:
    // added for completeness for compiler not sure what to do if actually found
    break;
  }
  ProcessNG *p = new ProcessNG();
  p->AddBlock(procevent);
  if(!m_processes.contains(procevent->GetProcessID()))
  {
    // haven't seen this process id yet
    ProcessNGList *proclist = new ProcessNGList();
    m_processes.insert(procevent->GetProcessID(), proclist);
  }
  m_processes.value(procevent->GetProcessID())->append(p);
}

//------------------------------------------------------------------------------
// AssignPacket
//
// Note: Assume that all of the interfaces have the same resolution for now
QPair<quint32, quint32> SectionNG::GetTimeResolution()
{
  QPair<quint32, quint32> resolution;
  resolution.first = 10;
  resolution.second = 6;

  if(!m_interfaces.isEmpty())
  {
    if(m_interfaces.size() > 0)
    {
      foreach (InterfaceNG *inter, m_interfaces)
      {
        if(inter->GetBlock()->GetOptions().contains(OPTION_TS_RESOLUTION))
        {
          quint32 optres = inter->GetBlock()->GetOptions().value(OPTION_TS_RESOLUTION).GetUIntegerValue(32);
          if(optres & 0x8000)
          {
            // most significant bit is 1
            resolution.first = 2;
            resolution.second = (optres & 0x7FFF);
          }
          else
          {
            // most significant bit is 0
            resolution.second = (optres & 0x7FFF);
          }
          break;
        }
      }
    }
  }
  return resolution;
}
//------------------------------------------------------------------------------
// AssignPacket
//
bool SectionNG::AssignPacket(ProcessNGList *proclist, EnhancedPacketBlock *packet)
{
  QMap<quint32, ConnectionNGList*> connmap;
  for(int i = proclist->size() - 1; i >= 0; i--)
  {
    // walk backward and find the most recent connection if possible
    connmap = proclist->at(i)->GetConnectionMap();
    if(! connmap.isEmpty())
    {
      foreach(quint32 key, connmap.keys())
      {
        ConnectionNGList* connlist = connmap.value(key);
        for(int j = connmap.value(key)->size() - 1; j >= 0; j--)
        {
          ConnectionNG* conn = connlist->at(j);
          // walk the connections for this process newest first
          if(!conn->GetBlocks().isEmpty())
          {
            if(conn->GetBlocks().last()->GetConnectionID() == packet->GetConnectionID())
            {
              //qDebug() << "Connmap id: " << connmap.value(key)->at(j)->GetConnectionID() << " packet id: " << packet->GetConnectionID();
              conn->AddPacket(packet);
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}
