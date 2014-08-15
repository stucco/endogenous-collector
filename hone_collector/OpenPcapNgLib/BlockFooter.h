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
    N depends on the length of the block

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 N |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#ifndef BLOCKFOOTER_H
#define BLOCKFOOTER_H

#include <QDataStream>

#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT BlockFooter
{
private:
  quint32 m_length;
  QByteArray m_rawData;

public:
  BlockFooter(void);
  ~BlockFooter(void);
  void Clear(){m_rawData.clear();}
  quint32 GetLength(){return m_length;}
  QByteArray GetRawData(){return m_rawData;}
  void SetLength(quint32 length){m_length = length;}
  void SetRawData(QByteArray data){m_rawData = data;}

};

// streaming methods
QDataStream &operator>>(QDataStream &in, BlockFooter &footer);

#endif // BLOCKFOOTER_H
