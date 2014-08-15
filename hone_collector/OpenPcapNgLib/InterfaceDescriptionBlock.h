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
 0 |                    Block Type = 0x00000001                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |           LinkType            |           Reserved            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                            SnapLen                            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#ifndef INTERFACEDESCRIPTIONBLOCK_H
#define INTERFACEDESCRIPTIONBLOCK_H
#include <QDataStream>
#include <QMap>
#include "Block.h"
#include "OptionField.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT InterfaceDescriptionBlock :
  public Block
{
private:
  QDataStream::ByteOrder m_byteOrder;
  quint16 m_linkType;
  QMap<quint16,OptionField> m_options;
  QByteArray m_rawData;
  quint32 m_snapLen;

public:
  InterfaceDescriptionBlock(void);
  ~InterfaceDescriptionBlock(void);
  void Clear(){m_options.clear();m_rawData.clear();}
  QDataStream::ByteOrder GetByteOrder(){return m_byteOrder;}
  QByteArray GetFullBlock(){return m_header.GetRawData().append(m_rawData.append(m_footer.GetRawData()));}
  quint16 GetLinkType(){return m_linkType;}
  QMap<quint16,OptionField> GetOptions(){return m_options;}
  quint32 GetSnapLength(){return m_snapLen;}
  void ReadBody(QDataStream &in);
  void SetByteOrder(QDataStream::ByteOrder byteOrder){m_byteOrder = byteOrder;}
  void SetLinkType(quint16 type){m_linkType = type;}
  void SetOptions(QMap<quint16,OptionField> options){m_options = options;}
  void SetSnapLength(quint32 snap){m_snapLen = snap;}
};

QDataStream &operator>>(QDataStream &in, InterfaceDescriptionBlock &interfaceDescr);
QDataStream &operator>>(QDataStream &in, InterfaceDescriptionBlock *interfaceDescr);
#endif //INTERFACEDESCRIPTIONBLOCK_H
