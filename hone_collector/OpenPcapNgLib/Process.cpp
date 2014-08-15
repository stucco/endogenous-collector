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
#include "Process.h"
//------------------------------------------------------------------------------
// ProcessNG
//
Process::Process(void)
{
}
//------------------------------------------------------------------------------
// ~ProcessNG
//
Process::~Process(void)
{

}

//------------------------------------------------------------------------------
// SetFromBlock
//
void Process::SetFromBlock(ProcessEventBlock& procblock)
{

  m_argv = procblock.GetArgV();
  m_ppid = procblock.GetParentProcessID();
  m_path = procblock.GetPath();
  m_pid = procblock.GetProcessID();
  m_timestamp = procblock.GetTimestamp();
  m_user = procblock.GetUser();
  m_uid = procblock.GetUserID();

}
