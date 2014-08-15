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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QDebug>
#include "PcapNgLibGlobal.h"
#include "ConnectionEventBlock.h"
#include "EnhancedPacketBlock.h"

class PCAPNGLIB_EXPORT Connection: public QObject
{  
  Q_OBJECT

private:
  bool m_isIncoming;
  quint32 m_connectionID;
  QHostAddress m_destaddress;
  quint16 m_destport;
  quint32 m_linkType;
  QHostAddress m_sourceaddress;
  quint16 m_sourceport;
  ConnectionEventBlock::Event m_state;
  quint64 m_timestamp;
  quint64 m_lastpackettime;
  bool m_updated;
  quint8 m_version;
  quint64 m_bytecount;
  quint64 m_packetcount;
  quint32 m_processID;
  quint32 m_protocol;

public:
  Connection(void);
  Connection(Connection& other);
  ~Connection(void);

  quint64 GetByteCount() {return m_bytecount;}
  quint32 GetConnectionID() {return m_connectionID;}
  QHostAddress GetDestAddress() {return m_destaddress;}
  quint16 GetDestPort() {return m_destport;}
  bool GetHasPacketData(){return m_updated;}
  bool GetIsIncoming(){return m_isIncoming;}
  quint64 GetLastPacketTimestamp(){return m_lastpackettime;}
  quint32 GetLinkType() {return m_linkType;}
  QHostAddress GetSourceAddress() {return m_sourceaddress;}
  quint16 GetSourcePort() {return m_sourceport;}
  ConnectionEventBlock::Event GetState() const {return m_state;}
  quint64 GetPacketCount() {return m_packetcount;}
  quint32 getProcessID(){return m_processID;}
  quint32 getProtocol(){return m_protocol;}
  quint64 GetTimestamp() {return m_timestamp;}
  bool GetUpdated() {return m_updated;}
  quint8 getVersion() {return m_version;}
  void SetFromBlock(ConnectionEventBlock& connblock);
  void SetState(ConnectionEventBlock::Event state){m_state = state;}
  void SetPacketLevelInfo(EnhancedPacketBlock& packetblock);
  void AddPacketData(EnhancedPacketBlock& packetblock);

};
typedef QList<Connection *> ConnectionList;
#endif //CONNECTIONNG_H
