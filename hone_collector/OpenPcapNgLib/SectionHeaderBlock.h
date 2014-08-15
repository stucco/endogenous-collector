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
 0 |                   Block Type = 0x0A0D0D0A                     |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                      Byte-Order Magic                         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |          Major Version        |         Minor Version         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                                                               |
   |                          Section Length                       |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
24 /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#ifndef SECTIONHEADERBLOCK_H
#define SECTIONHEADERBLOCK_H
#include <QDataStream>
#include <QMap>
#include <QUuid>
#include "Block.h"
#include "OptionField.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT SectionHeaderBlock :
  public Block
{
private:
  QDataStream::ByteOrder m_byteOrder;
  quint32 m_byteOrderMagic;
  quint16 m_majorVersion;
  quint16 m_minorVersion;
  QMap<quint16,OptionField> m_options;
  QByteArray m_rawData;
  quint64 m_sectionLength;

private:
public:
  SectionHeaderBlock(QByteArray rawData);
  SectionHeaderBlock(void);
  ~SectionHeaderBlock(void);
  void Clear(){m_options.clear();m_rawData.clear();}
  QDataStream::ByteOrder GetByteOrder(){return m_byteOrder;}
  quint32 GetByteOrderMagic(){return m_byteOrderMagic;}
  quint32 GetHostByteOrder();
  QByteArray GetFullBlock();
  QUuid GetIdentifier();
  quint16 GetMajorVersion(){return m_majorVersion;}
  quint16 GetMinorVersion(){return m_minorVersion;}
  QMap<quint16,OptionField> GetOptions(){return m_options;}
  quint64 GetSectionLength(){return m_sectionLength;}
  void ReadBody(QDataStream &in);
  void SetByteOrder(QDataStream::ByteOrder byteOrder){m_byteOrder = byteOrder;}
  void SetByteOrderMagic(quint32 bom){m_byteOrderMagic = bom;}
  void SetMajorVersion(quint16 major){m_majorVersion = major;}
  void SetMinorVersion(quint16 minor){m_minorVersion = minor;}
  void SetOptions(QMap<quint16,OptionField> options){m_options = options;}
  void SetSectionLength(quint64 length){m_sectionLength = length;}
};

QDataStream &operator>>(QDataStream &in, SectionHeaderBlock &sectionHeader);
QDataStream &operator>>(QDataStream &in, SectionHeaderBlock *sectionHeader);

#endif //SECTIONHEADERBLOCK_H
