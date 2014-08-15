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
 0 |                          Block Type                           |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <QDataStream>

#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT BlockHeader
{
private:
  quint32 m_length;
  QByteArray m_rawData;
  quint32 m_type;

public:
  BlockHeader();
  ~BlockHeader();
  void Clear() {m_rawData.clear(); m_length = 0;}
  // returns the number of bits for the block
  quint32 GetLength() {return m_length;}
  quint32 GetType() {return m_type;}
  QByteArray GetRawData() {return m_rawData;}
  void ReadHeader(QDataStream &in);
  void SetLength(quint32 length) {m_length = length;}
  void SetRawData(QByteArray data) {m_rawData = data;}
  void SetType(quint32 type) {m_type = type;}
  void Swap();

};

// streaming methods
QDataStream& operator>>(QDataStream& in, BlockHeader& header);

#endif // BLOCKHEADER_H
