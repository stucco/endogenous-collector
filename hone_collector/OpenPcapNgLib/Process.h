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

#ifndef PROCESS_H
#define PROCESS_H

#include <QDebug>
#include "PcapNgLibGlobal.h"
#include "ProcessEventBlock.h"
#include "Connection.h"
#include "EnhancedPacketBlock.h"
#include "ConnectionEventBlock.h"

class PCAPNGLIB_EXPORT Process
{
private:
  quint32 m_pid;
  QString m_argv;
  QString m_comment;
  quint32 m_gid;
  QString m_group;
  QString m_lib;
  QString m_path;
  quint32 m_ppid;
  quint64 m_timestamp;
  quint32 m_uid;
  QString m_user;



public:
  Process(void);
  ~Process(void);

  void AddPacket(EnhancedPacketBlock *packetblock);
  QString GetArgv(){return m_argv;}
  QString GetComment(){return m_comment;}
  ConnectionList GetConnections();
  quint32 GetGid(){return m_gid;}
  QString GetGroup(){return m_group;}
  QString GetLib(){return m_lib;}
  QString GetPath() const {return m_path;} 
  quint32 GetPpid(){return m_ppid;}
  quint32 GetProcessId(){return m_pid;}
  quint32 GetUid(){return m_uid;}
  QString GetUser(){return m_user;}

  void SetArgv(QString argv){m_argv = argv;}
  void SetFromBlock(ProcessEventBlock& procblock);
  void SetPath(QString path){m_path = path;}

};
typedef QList<Process *> ProcessList;
#endif //PROCESS_H
