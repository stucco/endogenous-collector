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
 0 |                    Block Type = 0x00000102                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                        Connection ID                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                          Process ID                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                        Timestamp (High)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
20 |                        Timestamp (Low)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+

   options
   =================================================================
   2:   Event
*/
#include "ConnectionEventBlock.h"

//------------------------------------------------------------------------------
// ConnectionEventBlock
//
ConnectionEventBlock::ConnectionEventBlock(void): m_connectionID(0), m_processID(0), m_timestamp(0)
{
}
//------------------------------------------------------------------------------
// ~ConnectionEventBlock
//
ConnectionEventBlock::~ConnectionEventBlock(void)
{
}
ConnectionEventBlock::Event ConnectionEventBlock::GetEvent()
{
  if(m_options.contains(OPTION_EVENT))
  {
    quint32 connevent;
    QDataStream stream(m_options.value(OPTION_EVENT).GetValue());
    stream.setByteOrder(m_options.value(OPTION_EVENT).GetByteOrder());
    stream >> connevent;
    return (ConnectionEventBlock::Event)connevent;
  }
  else
  {
    return ConnectionEventBlock::OPEN; // According to standard if no event option then assumed to be open
  }
}
//------------------------------------------------------------------------------
// ReadBody
//
void ConnectionEventBlock::ReadBody(QDataStream &in)
{
  quint32 connectionID;
  QMap<quint16,OptionField> options;
  quint32 processID;
  quint32 timestampHigh;
  quint32 timestampLow;
  
  Clear();
  // store the raw bytes that were used to initialize the body
  // skipping header and footer
  //qDebug() << "ConnectionEventBlock body length: " << GetHeader().GetLength();
  m_rawData.resize(GetHeader().GetLength() - 12);
  int read = in.readRawData(m_rawData.data(), GetHeader().GetLength() - 12);
  if(read < (int)(GetHeader().GetLength() - 12))
  {
    // not all the data is there to read
    throw -1;
  }
  // set up the stream for the current position
  QDataStream substream(m_rawData);
  substream.setByteOrder(in.byteOrder());

  substream >> connectionID; SetConnectionID(connectionID);
  substream >> processID; SetProcessID(processID);

  substream >> timestampHigh;
  substream >> timestampLow;
  m_timestamp = timestampHigh;
  m_timestamp = (m_timestamp << 32) + timestampLow;

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
QDataStream &operator>>(QDataStream &in, ConnectionEventBlock &connectionEvent)
{
  connectionEvent.ReadBody(in);
  return in;
}
QDataStream &operator>>(QDataStream &in, ConnectionEventBlock *connectionEvent)
{
  connectionEvent->ReadBody(in);
  return in;
}
