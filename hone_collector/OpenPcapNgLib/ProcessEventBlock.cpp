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
 0 |                    Block Type = 0x00000101                    |
   +---------------------------------------------------------------+
 4 |                      Block Total Length                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 8 |                          Process ID                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
12 |                        Timestamp (High)                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
16 |                        Timestamp (Low)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                      Options (variable)                       /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Block Total Length                       |
   +---------------------------------------------------------------+

   options
   =================================================================
   1  : Comment
   2  : Event
   3: : Path
   4  : ArgV
   5  : PPID
   6  : UID
   7  : GID
   8  : User
   9  : Group
   10 : Lib
*/
#include "ProcessEventBlock.h"
//------------------------------------------------------------------------------
// ProcessEventBlock
//
ProcessEventBlock::ProcessEventBlock(void)
{
}
//------------------------------------------------------------------------------
// ~ProcessEventBlock
//
ProcessEventBlock::~ProcessEventBlock(void)
{
  qDeleteAll(m_options);
  m_options.clear();
}
ProcessEventBlock::Event ProcessEventBlock::GetEvent()
{
  if(m_options.contains(OPTION_EVENT))
  {
    quint32 procevent;
    QDataStream stream(m_options.value(OPTION_EVENT)->GetValue());
    stream.setByteOrder(m_options.value(OPTION_EVENT)->GetByteOrder());
    stream >> procevent;
    return (ProcessEventBlock::Event)procevent;
  }
  else
  {
    return ProcessEventBlock::START;
  }
}
//------------------------------------------------------------------------------
// ReadBody
//
void ProcessEventBlock::ReadBody(QDataStream &in)
{
  QMap<quint16,OptionField*> options;
  quint32 processID;
  quint32 timestampLow;
  quint32 timestampHigh;

  Clear();
  // store the raw bytes that were used to initialize the body
  // skipping header and footer
  //qDebug() << "ProcessEventBlock body length: " << GetHeader().GetLength();
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

  substream >> processID; SetProcessID(processID);

  substream >> timestampHigh;
  substream >> timestampLow;
  m_timestamp = timestampHigh;
  m_timestamp = (m_timestamp << 32) + timestampLow;


  // everything without options is 24 bytes
  if((GetHeader().GetLength() - 24) > 0)
  {
    // there are options
    OptionField *of;
    do{
      of = new OptionField();
      substream >> *of;
      options.insert(of->GetCode(), of);
    }while(of->GetCode() != OPTION_END_OF_OPTION);
    SetOptions(options);
  }
}
QString ProcessEventBlock::GetPath()
{
  if(this->m_options.contains(OPTION_PATH))
  {
    return QString(m_options.value(OPTION_PATH)->GetValue());
  }
  else
  {
    return QString();
  }
}
QString ProcessEventBlock::GetArgV()
{
  if(this->m_options.contains(OPTION_ARGV))
  {
    return QString(m_options.value(OPTION_ARGV)->GetValue());
  }
  else
  {
    return QString();
  }
}
quint32 ProcessEventBlock::GetParentProcessID()
{
  if(this->m_options.contains(OPTION_PPID))
  {
    return m_options.value(OPTION_PPID)->GetUIntegerValue(32);
  }
  else
  {
    return 0;
  }
}
bool ProcessEventBlock::HasUser()
{
  if(this->m_options.contains(OPTION_USER))
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool ProcessEventBlock::HasUserID()
{
  if(this->m_options.contains(OPTION_USERID))
  {
    return true;
  }
  else
  {
    return false;
  }
}
QString ProcessEventBlock::GetUser()
{
  if(this->m_options.contains(OPTION_USER))
  {
    return QString(m_options.value(OPTION_USER)->GetValue());
  }
  else
  {
    return QString();
  }
}
quint32 ProcessEventBlock::GetUserID()
{
  if(this->m_options.contains(OPTION_USERID))
  {
    return m_options.value(OPTION_USERID)->GetUIntegerValue(32);
  }
  else
  {
    return 0;
  }
}
//------------------------------------------------------------------------------
// >>
//
QDataStream &operator>>(QDataStream &in, ProcessEventBlock &processEvent)
{
  processEvent.ReadBody(in);
  return in;
}

QDataStream &operator>>(QDataStream &in, ProcessEventBlock *processEvent)
{
  processEvent->ReadBody(in);
  return in;
}
