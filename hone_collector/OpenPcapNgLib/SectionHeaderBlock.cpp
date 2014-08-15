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
#include <QDebug>
#include "SectionHeaderBlock.h"

SectionHeaderBlock::SectionHeaderBlock(QByteArray rawData)
{
    QDataStream data(rawData);
    data >> m_header;
    data >> this;
    data >> m_footer;
}
//------------------------------------------------------------------------------
// SectionHeaderBlock
//
SectionHeaderBlock::SectionHeaderBlock(void)
{
}
//------------------------------------------------------------------------------
// ~SectionHeaderBlock
//
SectionHeaderBlock::~SectionHeaderBlock(void)
{
}
//------------------------------------------------------------------------------
// GetByteOrder
//
quint32 SectionHeaderBlock::GetHostByteOrder()
{
  if(GetByteOrderMagic() == SECTION_HEADER_BLOCK_MAGIC)
  {
    //qDebug() << "host byte order";
    return HOST_BYTE_ORDER;
  }
  else if(GetByteOrderMagic() == SWAP32(SECTION_HEADER_BLOCK_MAGIC))
  {
    //qDebug() << "swapped host byte order";
    return SWAPPED_HOST_BYTE_ORDER;
  }
  return UNKNOWN_HOST_BYTE_ORDER;
}
QByteArray SectionHeaderBlock::GetFullBlock()
{
  QByteArray result = m_header.GetRawData();
  result.append(m_rawData);
  result.append(m_footer.GetRawData());
  return result;
}
//------------------------------------------------------------------------------
// GetIdentifier
//
QUuid SectionHeaderBlock::GetIdentifier()
{
  if (this->m_options.contains(OPTION_SYSTEM_IDENTIFIER))
  {
    QUuid id;
    quint32 type;
    QDataStream stream(m_options.value(OPTION_SYSTEM_IDENTIFIER).GetValue());
    stream.setByteOrder(m_options.value(OPTION_SYSTEM_IDENTIFIER).GetByteOrder());
    stream >> type;
    if (type == OPTION_HOST_ID_TYPE_GUID)
    {
      stream >> id;
      return id;
    }
    else
      return QUuid();
  }
  else
  {
    return QUuid();
  }
}
//------------------------------------------------------------------------------
// ReadBody
//
void SectionHeaderBlock::ReadBody(QDataStream &in)
{
  quint32 bom;
  quint16 major;
  quint16 minor;
  QMap<quint16,OptionField> options;
  quint64 length;
  //quint32 lengthTop;
  //quint32 lengthBottom;

  Clear();

  // move the bytes into a byte array
  in >> bom; SetByteOrderMagic(bom);

  QDataStream::ByteOrder current = in.byteOrder();
  if(GetHostByteOrder() == SWAPPED_HOST_BYTE_ORDER)
  {
    GetHeader().Swap();
    bom = SWAP32(bom);
    if(current == QDataStream::BigEndian)
    {
      SetByteOrder(QDataStream::LittleEndian);
      in.setByteOrder(QDataStream::LittleEndian);
    }
    else
    {
      SetByteOrder(QDataStream::BigEndian);
      in.setByteOrder(QDataStream::BigEndian);
    }
  }
  else
  {
    SetByteOrder(current);
  }
  

  // store the raw bytes that were used to initialize the body
  // skipping bom (4 bytes), header (8 bytes), and footer (4 bytes)
  QByteArray remaining;
  remaining.resize(GetHeader().GetLength() - 16);
  int read = in.readRawData(remaining.data(), GetHeader().GetLength() - 16);
  if (read < (int)(GetHeader().GetLength() - 16))
  {
    // not all the data is there to read
    throw -1;
  }

  // set up a stream to read back in the bom in to the raw data as 4 bytes
  QDataStream rawrestream(&m_rawData, QIODevice::WriteOnly);
  rawrestream.setByteOrder(in.byteOrder());
  rawrestream << bom;
  m_rawData.append(remaining);

  // create a stream based on the raw data
  QDataStream substream(m_rawData);
  substream.setByteOrder(in.byteOrder());

  // because we're basing this stream off of the raw data which includes
  // the bom, we need to read it back out (ignoring it of course)
  quint32 bomagain;
  substream >> bomagain;
  substream >> major; SetMajorVersion(major);
  substream >> minor; SetMinorVersion(minor);

  substream >> length; SetSectionLength(length);
  // everything without options is 28 bytes
  if((GetHeader().GetLength() - 28) > 0)
  {
    // there are options
    OptionField of;
    do{
      substream >> of;
      options.insert(of.GetCode(), of);
    }while(of.GetCode() != OPTION_END_OF_OPTION);
    SetOptions(options);
  }
}
//------------------------------------------------------------------------------
// >>
//
QDataStream &operator>>(QDataStream &in, SectionHeaderBlock &sectionHeader)
{
  sectionHeader.ReadBody(in);
  return in;
}
QDataStream &operator>>(QDataStream &in, SectionHeaderBlock *sectionHeader)
{
  sectionHeader->ReadBody(in);
  return in;
}
