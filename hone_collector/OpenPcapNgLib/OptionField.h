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
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Option Code              |         Option Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                       Option Value                            /
/             variable length, aligned to 32 bits               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
#ifndef OPTIONFIELD_H
#define OPTIONFIELD_H

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT OptionField
{
private:
  quint16 m_code;
  quint16 m_length;
  QByteArray m_value;
  QDataStream::ByteOrder m_byteOrder;

public:
  OptionField(void);
  ~OptionField(void);
  void Clear(){m_value.clear();}
  QDataStream::ByteOrder GetByteOrder() const{return m_byteOrder;}
  quint16 GetCode(){return m_code;}
  quint16 GetLength(){return m_length;}
  QByteArray GetValue() const {return m_value;}
  quint32 GetUIntegerValue(int bits) const;
  quint32 Get32BitAlignedLength();
  void SetByteOrder(QDataStream::ByteOrder byteOrder){m_byteOrder = byteOrder;}
  void SetCode(quint16 code){m_code = code;}
  void SetLength(quint16 length){m_length = length;}
  void SetValue(QByteArray value){m_value = value;}

};

QDataStream &operator>>(QDataStream &in, OptionField &option);

#endif //OPTIONFIELD_H
