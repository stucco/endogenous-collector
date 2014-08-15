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
 0 |                    Block Type = 0x00000101                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                          Process ID                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                        Timestamp (High)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                        Timestamp (Low)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#ifndef PROCESSEVENTBLOCK_H
#define PROCESSEVENTBLOCK_H
#include <QDataStream>
#include <QMap>
#include "Block.h"
#include "OptionField.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT ProcessEventBlock :
  public Block
{
public:
  enum Event
  {
    START = 0x00000000,
    CHILD = 0x00000001,
    SHAREDLIB = 0x00000003,
    END = 0xFFFFFFFF
  };
private:
  QMap<quint16,OptionField*> m_options;
  quint32 m_processID;
  QByteArray m_rawData;
  quint64 m_timestamp;

public:
  ProcessEventBlock(void);
  ~ProcessEventBlock(void);
  void Clear(){m_options.clear();m_rawData.clear();}
  QString GetArgV();
  ProcessEventBlock::Event GetEvent();
  QByteArray GetFullBlock(){return m_header.GetRawData().append(m_rawData.append(m_footer.GetRawData()));}
  quint32 GetParentProcessID();
  QString GetPath();
  quint32 GetProcessID(){return m_processID;}
  QMap<quint16,OptionField*> &GetOptions(){return m_options;}
  quint64 GetTimestamp(){return m_timestamp;}

  bool HasUser();
  bool HasUserID();
  QString GetUser();
  quint32 GetUserID();
  void ReadBody(QDataStream &in);
  void SetProcessID(quint32 procid){m_processID = procid;}
  void SetOptions(QMap<quint16,OptionField*>  options){m_options = options;}
  void SetTimestamp(quint64 timestamp){m_timestamp = timestamp;}

};

QDataStream &operator>>(QDataStream &in, ProcessEventBlock &processEvent);
QDataStream &operator>>(QDataStream &in, ProcessEventBlock *processEvent);

#endif //PROCESSEVENTBLOCK_H
