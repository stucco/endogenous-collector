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
 0 |                    Block Type = 0x00000102                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                        Connection ID                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                          Process ID                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                        Timestamp (High)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
20 |                        Timestamp (Low)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#ifndef CONNECTIONEVENTBLOCK_H
#define CONNECTIONEVENTBLOCK_H
#include <QDataStream>
#include <QMap>
#include "Block.h"
#include "OptionField.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT ConnectionEventBlock :
  public Block
{
public:
  enum Event{OPEN = 0x00, CLOSE = 0xFFFFFFFF, UNKNOWN=1};
private:
  quint32 m_connectionID;
  QMap<quint16,OptionField> m_options;
  quint32 m_processID;
  QByteArray m_rawData;
  quint64 m_timestamp;

public:
  ConnectionEventBlock(void);
  ~ConnectionEventBlock(void);
  void Clear(){m_options.clear();m_rawData.clear();}
  quint32 GetConnectionID(){return m_connectionID;}
  ConnectionEventBlock::Event GetEvent();
  QByteArray GetFullBlock(){return m_header.GetRawData().append(m_rawData.append(m_footer.GetRawData()));}
  QMap<quint16,OptionField> GetOptions(){return m_options;}
  quint32 GetProcessID(){return m_processID;}
  quint64 GetTimestamp(){return m_timestamp;}
  void ReadBody(QDataStream &in);
  void SetConnectionID(quint32 connid){m_connectionID = connid;}
  void SetOptions(QMap<quint16,OptionField> options){m_options = options;}
  void SetProcessID(quint32 procid){m_processID = procid;}
  void SetTimestamp(quint64 timestamp){m_timestamp = timestamp;}
};

QDataStream &operator>>(QDataStream &in, ConnectionEventBlock &connectionEvent);
QDataStream &operator>>(QDataStream &in, ConnectionEventBlock *connectionEvent);
#endif //CONNECTIONEVENTBLOCK_H
