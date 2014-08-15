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
#include <QDebug>
#include "BlockHeader.h"
//------------------------------------------------------------------------------
// BlockHeader
//
BlockHeader::BlockHeader()
{
}
//------------------------------------------------------------------------------
// ~BlockHeader
//
BlockHeader::~BlockHeader()
{
}
//------------------------------------------------------------------------------
// Swap
//
void BlockHeader::Swap()
{
  qDebug() << "swapping: " << m_type << ", " << m_length;
  m_type = SWAP32(m_type);
  m_length = SWAP32(m_length);
  qDebug() << "result: " << m_type << ", " << m_length;
}
//------------------------------------------------------------------------------
// ReadHeader
//
void BlockHeader::ReadHeader(QDataStream &in)
{
   quint32 type;
  quint32 length;
  QByteArray data;

  Clear();

  data.resize(8);
  //char test[8];
  int read = in.readRawData(data.data(), 8); 
  if(read < 8)
  {
    throw -1;
  }
  SetRawData(data);
  // set up the stream for the current position
  QDataStream substream(data);
  substream.setByteOrder(in.byteOrder());

  substream >> type; SetType(type);
  substream >> length; SetLength(length);

}
//------------------------------------------------------------------------------
// >>
//
QDataStream& operator>>(QDataStream& in, BlockHeader& header)
{
  header.ReadHeader(in);
  return in;
}
