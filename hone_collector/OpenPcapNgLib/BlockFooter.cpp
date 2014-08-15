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

#include "BlockFooter.h"
//------------------------------------------------------------------------------
// BlockFooter
//
BlockFooter::BlockFooter(void)
{
}
//------------------------------------------------------------------------------
// ~BlockFooter
//
BlockFooter::~BlockFooter(void)
{
}
//------------------------------------------------------------------------------
// >>
//
QDataStream &operator>>(QDataStream &in, BlockFooter &footer)
{
  quint32 length;
  QByteArray data;

  footer.Clear();
  data.resize(4);
  int result = in.readRawData(data.data(), 4);
  if (result < 4)
  {
    qDebug() << "ERROR: Incomplete packet, not enough data to read footer";
    return in; //throw -1;
  }
  // set up the stream for the current position
  QDataStream substream(data);
  substream.setByteOrder(in.byteOrder());

  substream >> length; footer.SetLength(length);
  footer.SetRawData(data);
  return in;
}
