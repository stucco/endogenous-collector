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

   options
   =================================================================
   1  : Comment
   2  : Name
   3: : Description
   4  : IPv4 address
   5  : IPv6 address
   6  : MAC address
   7  : EUI address
   8  : Speed
   9  : TS resolution
   10 : Time zone
   11 : Filter
   12 : OS
   13 : FCS Length
   14 : TS offset
*/
#include <QDebug>
#include "InterfaceDescriptionBlock.h"
//------------------------------------------------------------------------------
// InterfaceDescriptionBlock
//
InterfaceDescriptionBlock::InterfaceDescriptionBlock(void)
{
}
//------------------------------------------------------------------------------
// ~InterfaceDescriptionBlock
//
InterfaceDescriptionBlock::~InterfaceDescriptionBlock(void)
{
}
//------------------------------------------------------------------------------
// ReadBody
//
void InterfaceDescriptionBlock::ReadBody(QDataStream &in)
{
  quint16 linkType;
  QMap<quint16,OptionField> options;
  quint32 snapLen;

  Clear();
  // store the raw bytes that were used to initialize the body
  // skipping header and footer
  //qDebug() << "InterfaceDescriptionBlock body length: " << GetHeader().GetLength();
  m_byteOrder = in.byteOrder();
  m_rawData.resize(GetHeader().GetLength() - 12);
  int read = in.readRawData(m_rawData.data(), GetHeader().GetLength() - 12);
  if (read < (int)(GetHeader().GetLength() - 12))
  {
    // not all the data is there to read
    throw -1;
  }
  // set up the stream for the current position
  QDataStream substream(m_rawData);
  substream.setByteOrder(in.byteOrder());

  substream >> linkType; SetLinkType(linkType);
  substream.skipRawData(2); // reserved space
  substream >> snapLen; SetSnapLength(snapLen);

  //qDebug() << "Interface description block length: " << GetHeader().GetLength();
  // everything without options is 20 bytes
  if((GetHeader().GetLength() - 20) > 0)
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
QDataStream &operator>>(QDataStream &in, InterfaceDescriptionBlock &interfaceDescr)
{
  interfaceDescr.ReadBody(in);
  return in;
}
QDataStream &operator>>(QDataStream &in, InterfaceDescriptionBlock *interfaceDescr)
{
  interfaceDescr->ReadBody(in);
  return in;
}
