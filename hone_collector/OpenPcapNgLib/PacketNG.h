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

#ifndef PACKETNG_H
#define PACKETNG_H
#include <QDataStream>
#include <QHostAddress>
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT PacketNG
{
private:
  quint32 m_linkType;
  QByteArray m_rawData;
  enum LinkType {LINKTYPE_ETHERNET = 1, LINKTYPE_RAW = 101};
  QHostAddress m_destAddress;
  quint16 m_destPort;
  quint8 m_protocol;
  QHostAddress m_srcAddress;
  quint16 m_srcPort;
  quint16 m_totalLen;
  quint8 m_version;
  QString m_tcpstring;

public:
  PacketNG(void);
  ~PacketNG(void);
  void Clear() {m_rawData.clear(); m_tcpstring.clear();}
  QHostAddress GetDestAddress() {return m_destAddress;}
  quint16 GetDestPort() {return m_destPort;}
  quint32 GetLinkType() {return m_linkType;}
  quint8 GetProtocol() {return m_protocol;}
  QHostAddress GetSourceAddress() {return m_srcAddress;}
  quint16 GetSourcePort() {return m_srcPort;}
  QString GetTcpDumpString();
  quint16 GetTotalLength() {return m_totalLen;}
  quint8 GetVersion() {return m_version;}
  bool IsEmpty() {return m_rawData.isEmpty();}
  void SetLinkType(quint32 link) {m_linkType = link;}
  void SetRawData(QByteArray data) {Clear();m_rawData = data; SetValues();}
  void SkipToHeader(QDataStream &stream);

private:
  void ParseIPv4Data();
  void ParseIPv6Data();
  void SetValues();
};

#endif //PACKETNG_H
