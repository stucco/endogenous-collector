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
#include "ProcessNG.h"
//------------------------------------------------------------------------------
// ProcessNG
//
ProcessNG::ProcessNG(void)
{
}
//------------------------------------------------------------------------------
// ~ProcessNG
//
ProcessNG::~ProcessNG(void)
{
  qDeleteAll(m_blocks);
  m_blocks.clear();
  qDeleteAll(m_connections);
  m_connections.clear();
}
//------------------------------------------------------------------------------
// AddConnectionBlock
//
void ProcessNG::AddConnectionBlock(ConnectionEventBlock *connevent)
{
  if(connevent->GetConnectionID() == 0) return;

  if(! m_connections.contains(connevent->GetConnectionID()))
  {
    ConnectionNG *c = new ConnectionNG();
    c->SetState(connevent->GetEvent());
    c->AddBlock(connevent);
    ConnectionNGList *list = new ConnectionNGList();
    list->append(c);
    m_connections.insert(connevent->GetConnectionID(), list);
  }
  else
  {
    if(m_connections.value(connevent->GetConnectionID())->last()->GetState() == ConnectionEventBlock::CLOSE)
    {
      // the connection is already closed
      if(connevent->GetEvent() == ConnectionEventBlock::OPEN)
      {
        // new connection with the same connection id
        ConnectionNG *c = new ConnectionNG();
        c->SetState(connevent->GetEvent());
        c->AddBlock(connevent);
        m_connections.value(connevent->GetConnectionID())->append(c);
      }
      else
      {
        // just add the block to the last one
        m_connections.value(connevent->GetConnectionID())->last()->AddBlock(connevent);
        m_connections.value(connevent->GetConnectionID())->last()->SetState(connevent->GetEvent());
      }
    }
    else
    {
      // the connection is currently open, add the block and set the state (opens just keep the connection open)
      m_connections.value(connevent->GetConnectionID())->last()->AddBlock(connevent);
      m_connections.value(connevent->GetConnectionID())->last()->SetState(connevent->GetEvent());
    }
  }
}
//------------------------------------------------------------------------------
// AddPacket
//
void ProcessNG::AddPacket(EnhancedPacketBlock *packet)
{
    if(m_connections.contains(packet->GetConnectionID()))
    {
      // always just add to the last connection since we can get packets after
      // we see a connection close event
      m_connections.value(packet->GetConnectionID())->last()->AddPacket(packet);
    }
    else
    {
      //qDebug() << "WARNING: packet without associated connection";
    }
}
//------------------------------------------------------------------------------
// GetConnections
//
ConnectionNGList ProcessNG::GetConnections()
{
  ConnectionNGList all;
  foreach(ConnectionNGList *list, m_connections.values())
  {
    all << *list;
  }
  return all;
}
