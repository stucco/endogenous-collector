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
#include "ConnectionNG.h"
//------------------------------------------------------------------------------
// Connection
//
ConnectionNG::ConnectionNG(void)
{
}
//------------------------------------------------------------------------------
// ~Connection
//
ConnectionNG::~ConnectionNG(void)
{
  qDeleteAll(m_blocks);
  m_blocks.clear();
  qDeleteAll(m_packets);
  m_packets.clear();
}
quint32 ConnectionNG::GetConnectionID(){
  if(m_blocks.size() > 0){
    return m_blocks.at(0)->GetConnectionID();
  }
  return 0;
}
