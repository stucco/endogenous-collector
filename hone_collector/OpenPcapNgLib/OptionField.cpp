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
#include "OptionField.h"
//------------------------------------------------------------------------------
// OptionField
//
OptionField::OptionField(void)
{
}
//------------------------------------------------------------------------------
// ~OptionField
//
OptionField::~OptionField(void)
{
}
//------------------------------------------------------------------------------
// GetUIntegerValue
//
quint32 OptionField::GetUIntegerValue(int bits) const
{
  QDataStream stream(m_value);
  stream.setByteOrder(m_byteOrder);
  switch(bits)
  {
    case 16:
      quint16 v16;
      stream >> v16;
      return v16;
      break;
    case 32:
      quint32 v32;
      stream >> v32;
      return v32;
      break;
    case 64:
      quint64 v64;
      stream >> v64;
      return v64;
      break;
  }
  return 0;

}
//------------------------------------------------------------------------------
// Get32BitAlignedLength
//
quint32 OptionField::Get32BitAlignedLength()
{
  switch(GetLength() % 4)
  {
        case 1:
          return GetLength() + 3;
          break;
        case 2:
          return GetLength() + 2;
          break;
        case 3:
          return GetLength() + 1;
          break;
  }
  return GetLength();
}
//------------------------------------------------------------------------------
// >>
//
QDataStream &operator>>(QDataStream &in, OptionField &option)
{
  quint16 code;
  quint16 length;
  option.Clear();
  in >> code; option.SetCode(code);
  in >> length; option.SetLength(length);
  option.SetByteOrder(in.byteOrder());
  if(code != OPTION_END_OF_OPTION)
  {
    QByteArray data;
    data.resize(length);
    data.fill('0');
    //option.SetValue(data);
    if(length > 0){
      in.readRawData(data.data(), length);
      // skip padding bytes
      switch(length % 4)
      {
        case 1:
          in.skipRawData(3);
          break;
        case 2:
          in.skipRawData(2);
          break;
        case 3:
          in.skipRawData(1);
          break;
      }
      option.SetValue(data);
    }
  }
  else
  {
    if(length > 0)
    {
      in.skipRawData(length);
      //qDebug() << "End of option field with length " << length;
    }
  }
  //qDebug() << option.GetCode() << option.GetValue();
  return in;
}
