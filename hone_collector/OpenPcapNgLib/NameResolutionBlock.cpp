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
 0 |                    Block Type = 0x00000004                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |      Record Type              |         Record Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 /                       Record Value                            /
   /             variable length, aligned to 32 bits               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   .                                                               .
   .                  . . . other records . . .                    .
   .                                                               .
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Record Type == end_of_recs   |  Record Length == 00          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+
*/
#include "NameResolutionBlock.h"
//------------------------------------------------------------------------------
// NameResolutionBlock
//
NameResolutionBlock::NameResolutionBlock(void)
{
}
//------------------------------------------------------------------------------
// ~NameResolutionBlock
//
NameResolutionBlock::~NameResolutionBlock(void)
{
}
//------------------------------------------------------------------------------
// ReadBody
//
void NameResolutionBlock::ReadBody(QDataStream &in)
{
  Clear();
  // store the raw bytes that were used to initialize the body
  // skipping header and footer
  //qDebug() << "NameResolutionBlock body length: " << GetHeader().GetLength();
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

  quint32 totalRecordLen = 0;
    // there are options
    OptionField record;
    do{
      substream >> record;
      m_records.append(record);
      totalRecordLen += record.Get32BitAlignedLength();
    }while(record.GetCode() != OPTION_END_OF_OPTION);


    // everything without options is stored in nooption variable
  if((GetHeader().GetLength() - (totalRecordLen + 12) > 0))
  {
    // there are options
    OptionField of;
    do{
      substream >> of;
      m_options.insert(of.GetCode(), of);
    }while(of.GetCode() != OPTION_END_OF_OPTION);
  }
}
//------------------------------------------------------------------------------
// >>
//
QDataStream &operator>>(QDataStream &in, NameResolutionBlock &nameResolution)
{
  nameResolution.ReadBody(in);
  return in;
}
QDataStream &operator>>(QDataStream &in, NameResolutionBlock *nameResolution)
{
  nameResolution->ReadBody(in);
  return in;
}
