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
// PCapNGLightReader: This class parses a PCAP-NG file, holding only enough in
// memory to do correlation between hosts, processes, connections, and packets.
// Signals are used to return information to the intantiating class instead of 
// an object tree.  This allows parsing much larger files, however more responsiblity
// is placed upon the calling class.
#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "PcapNGLightReader.h"
#include "BlockFooter.h"
#include "BlockHeader.h"
#include "ConnectionEventBlock.h"
#include "EnhancedPacketBlock.h"
#include "InterfaceDescriptionBlock.h"
#include "NameResolutionBlock.h"
#include "ProcessEventBlock.h"
#include "SectionHeaderBlock.h"
#include "SectionNG.h"
#include "SimplePacketBlock.h"

//------------------------------------------------------------------------------
// PCapNGReader
//
// Constructor
PCapNGLightReader::PCapNGLightReader()
  : QObject(0),m_buffersize(10)
{
}
//------------------------------------------------------------------------------
// ~PCapNGReader
//
// Deconstructor
PCapNGLightReader::~PCapNGLightReader()
{

}
//------------------------------------------------------------------------------
// ReadFile
//
// Entry point for the class.  The reader will parse through the file and
// signal events for consumers of the class to handle.
//
// QString filename: PCAP-NG file path the read
//
// Emits:
//     Error: File issues
//     ProgressEnd: At the end of reading the file
//     ProgressStart: Before starting file parse, count is based on number of 
//                    bytes in the file
void PCapNGLightReader::ReadFile(QString filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
  {
    emit Error(file.errorString());
    return;
  }

  QFileInfo info(filename);
  emit ProgressStart((int)info.size());

  QList<SectionNG*> sections;
  BlockHeader bh;

  QDataStream in(&file);
  in.setVersion(QDataStream::Qt_4_7);
  in >> bh;
  while (!in.atEnd())
  {
    SectionNG* s = ReadSection(in, bh);
    sections.append(s);
  }
  file.close();
  emit ProgressEnd();
}
//------------------------------------------------------------------------------
// BitAlign
//
// At times blocks are not alligned to 32 bit boundaries.  However, processing
// the file (and the specification) requires that they are.  Double check
// boundaries and allow for continued parsing
//
// QDataStream& input: current input stream being read
// quint64 length: length of the block that was read
void PCapNGLightReader::BitAlign(QDataStream& input, quint64 length)
{
  switch(length % 4)
  {
    case 1:
      //qDebug() << "skipping 3 bytes";
      input.skipRawData(3);
      break;
    case 2:
      //qDebug() << "skipping 2 bytes";
      input.skipRawData(2);
      break;
    case 3:
      //qDebug() << "skipping 1 bytes";
      input.skipRawData(1);
      break;
  }
}
//------------------------------------------------------------------------------
// ConsumePacketBlock
// 
// Handle process event blocks keeping track of processes in case of child events
//
// ProcessEventBlock& procevent: Process event block to consume
//
// Emits:
//    ProcessFound: When a new process is identified (not an end event)
void PCapNGLightReader::ConsumeProcessBlock(ProcessEventBlock& procevent)
{
  if(procevent.GetEvent() != ProcessEventBlock::END)
  {
    // create the new process
    Process* p = new Process();
    p->SetFromBlock(procevent);
    if(procevent.GetEvent() == ProcessEventBlock::CHILD){
      // it was a child process grab some info
      if(m_processes.contains(procevent.GetParentProcessID())){
        Process* parent = m_processes.value(procevent.GetParentProcessID());
        p->SetArgv(parent->GetArgv());
        p->SetPath(parent->GetPath());
      }
    }
    if(m_processes.contains(p->GetProcessId()))
    {
      Process* remove = m_processes.take(p->GetProcessId());
      delete remove;
    }
    m_processes.insert(procevent.GetProcessID(), p);
    emit ProcessFound(*p);
  }
}
//------------------------------------------------------------------------------
// ConsumeConnectionBlock
// 
// Handle connection event blocks keeping track of connections.  Sends connections
// when a connection close event found
//
// ConnectionEventBlock& block: Connection event block to consume
void PCapNGLightReader::ConsumeConnectionBlock(ConnectionEventBlock& block)
{
  quint32 connid = block.GetConnectionID();
  if (m_connections.contains(connid))
  {
    if (block.GetEvent() == ConnectionEventBlock::OPEN)
    {
        // DMB 9/26/2013 changing to ignoring additional open connection events as per
        //    email conversation with Richard G.  We're only going to close when we get 
        //    a close event otherwise assume the process is just maintaining the connection
        /*
        // we got an open when we already have a connection, close the current
        // and record the new
        Connection* c = m_connections.take(connid);
        c->SetState(ConnectionEventBlock::CLOSE);
        SendConnection(c);

        Connection* newc = new Connection();
        newc->SetFromBlock(block);
        m_connections.insert(connid, newc);
        */
    }
    if (block.GetEvent() == ConnectionEventBlock::CLOSE)
    {
        // close the current connection and remove it
        Connection* c = m_connections.take(connid);
        c->SetState(ConnectionEventBlock::CLOSE);
        SendConnection(c);
    }
  }
  else
  {
    // not in the list yet
    if (block.GetEvent() == ConnectionEventBlock::OPEN)
    {
        // new or unknown, go ahead and open up the connection
        Connection* newc = new Connection();
        newc->SetFromBlock(block);
        m_connections.insert(connid, newc);
    }
  }
}
//------------------------------------------------------------------------------
// SendConnection
// 
// Sends connection information through a signal to the consumer of this class
//
// Connection* connection: Connection to emit
//
// Emits:
//    ConnectionFinished: When the connection buffer is full, the first connection
//                        is dequeued and send off.  The queue is used for packets
//                        that may occure after connection close due to asynchronous
//                        delivery
void PCapNGLightReader::SendConnection(Connection* connection)
{
  // we got a new connection, put it in the buffer
  if (m_connectionbuffer.size() >= m_buffersize)
  {
    // need to dequeue one and send it off
    Connection* c = m_connectionbuffer.dequeue();
    if (c->GetHasPacketData())
    {
      emit ConnectionFinished(*c);
    }
    delete c;
  }
  m_connectionbuffer.enqueue(connection);
}
//------------------------------------------------------------------------------
// ClearConnectionBuffer
// 
void PCapNGLightReader::ClearConnectionBuffer()
{
  while (!m_connectionbuffer.isEmpty())
  {
    Connection* c = m_connectionbuffer.dequeue();
    if (c->GetHasPacketData())
    {
      emit ConnectionFinished(*c);
    }
    delete c;
  }
  foreach (Connection* c, m_connections.values())
  {
    if (c->GetHasPacketData()) // only if packets exist
    {
      qDebug() << "Cleanup " << c->GetConnectionID();
      emit ConnectionFinished(*c);
    }
  }
  qDeleteAll(m_connections);
  m_connections.clear();
}
//------------------------------------------------------------------------------
// ConsumeEnhancedPacketBlock
// 
// Adds packet level information to the connection to make it more useful to
// consumers.  Full packet level data is not returned by this class
//
// EnhancedPacketBlock& block: packet information to associate with a connection
void PCapNGLightReader::ConsumeEnhancedPacketBlock(EnhancedPacketBlock& block)
{
  quint32 connid = block.GetConnectionID();
  quint32 pid = block.GetProcessID();
  if (m_connections.contains(connid))
  {
    // good it exists
    m_connections.value(connid)->AddPacketData(block);
  }
  else
  {
    bool found = false;
    // check the buffer
    for (int i = 0; i < m_connectionbuffer.size(); i++)
    {
      if (m_connectionbuffer.at(i)->GetConnectionID() == connid)
      {
        found = true;
        m_connectionbuffer.at(i)->AddPacketData(block);
      }
    }
    if (!found)
    {
      // well... shoot
    }
  }
}
//------------------------------------------------------------------------------
// ReadSection
//
// Reads the information for a given section, sections are blocks of information
// in a PCAP-NG file that come from a single source
//
// QDataStream& input: input stream to read the section from
// BlockHeader& bh: initial block header from the section
//
// Emits:
//        ProgressStep: After each block read
//        NewTimeResolution: When a new interface block is read
//        NewHost: When a new section header is read (or the first one sent in)
SectionNG* PCapNGLightReader::ReadSection(QDataStream& input, BlockHeader& bh)
{
  SectionNG* sect;
  //InterfaceNG* face;
  
  QDataStream::ByteOrder streamorder = input.byteOrder();
  BlockFooter bf;
  SectionHeaderBlock shb;
  InterfaceDescriptionBlock idb;
  ProcessEventBlock peb;
  ConnectionEventBlock ceb;
  SimplePacketBlock spb;
  EnhancedPacketBlock epb;
  NameResolutionBlock nrb;
  QPair<quint32, quint32> resolution;
  //quint32 nexttype;
  bool finished = false;

  while (!finished)
  {
    // get the next block type
    switch (bh.GetType())
    {
      case ENHANCED_PACKET_BLOCK:
        epb = EnhancedPacketBlock();
        epb.SetHeader(bh);
        // need the link type to know how to read the header information
        epb.GetPacket().SetLinkType(idb.GetLinkType());
        input >> epb;
        BitAlign(input, epb.GetHeader().GetLength());
        input >> bf;
        epb.SetFooter(bf);
        ConsumeEnhancedPacketBlock(epb);
        emit ProgressStep((int)epb.GetHeader().GetLength());
        break;
      case SIMPLE_PACKET_BLOCK:
        spb = SimplePacketBlock();
        spb.SetHeader(bh);
        input >> spb;
        BitAlign(input, spb.GetHeader().GetLength());
        input >> bf;
        spb.SetFooter(bf);

        //qDebug() << "Read SPB - header: " << spb.GetHeader().GetLength() << " footer: " << bf.GetLength() << " packet version: " << spb.GetPacket().GetVersion();
        //qDebug() <<"--------------------------------------------------------";
        //face->AddSimplePacket(spb);
        emit ProgressStep((int)spb.GetHeader().GetLength());
        break;
      case CONNECTION_EVENT_BLOCK:
        ceb = ConnectionEventBlock();
        ceb.SetHeader(bh);
        input >> ceb;
        BitAlign(input, ceb.GetHeader().GetLength());
        input >> bf;
        ceb.SetFooter(bf);
        ConsumeConnectionBlock(ceb);
        emit ProgressStep((int)ceb.GetHeader().GetLength());
        break;
      case PROCESS_EVENT_BLOCK:
        peb = ProcessEventBlock();
        peb.SetHeader(bh);
        input >> peb;
        BitAlign(input, peb.GetHeader().GetLength());
        input >> bf;
        peb.SetFooter(bf);
        //qDebug() << "PEB id: " << peb.GetProcessID() << " header: " << peb.GetHeader().GetLength() << " footer: " << bf.GetLength() << " options: " << peb.GetOptions().size();
        //qDebug() << "event: " << peb.GetEvent();
        //if(peb.GetPath() != "")
        //{
        //  qDebug() << "path: " << peb.GetPath();
        //}
        //qDebug() << "argv: " << peb.GetArgV();
        //qDebug() << "ppid: " << peb.GetParentProcessID();
        ConsumeProcessBlock(peb);
        emit ProgressStep((int)peb.GetHeader().GetLength());
        break;
      case INTERFACE_DESCRIPTION_BLOCK:
        idb = InterfaceDescriptionBlock();
        idb.SetHeader(bh);
        input >> idb;
        BitAlign(input, idb.GetHeader().GetLength());
        input >> bf;
        idb.SetFooter(bf);

        
        resolution.first = 10;
        resolution.second = 6;

        if(idb.GetOptions().contains(OPTION_TS_RESOLUTION))
        {
          quint32 optres = idb.GetOptions().value(OPTION_TS_RESOLUTION).GetUIntegerValue(32);
          if(optres & 0x8000)
          {
            // most significant bit is 1
            resolution.first = 2;
            resolution.second = (optres & 0x7FFF);
          }
          else
          {
            // most significant bit is 0
            resolution.second = (optres & 0x7FFF);
          }
        }
        emit NewTimeResolution(resolution);
        emit ProgressStep((int)idb.GetHeader().GetLength());
        break;
      case NAME_RESOLUTION_BLOCK:
        nrb = NameResolutionBlock();
        nrb.SetHeader(bh);
        input >> nrb;
        BitAlign(input, nrb.GetHeader().GetLength());
        input >> bf;
        nrb.SetFooter(bf);
        //qDebug() << "Read NRB - header: " << nrb->GetHeader().GetLength() << " footer: " << bf.GetLength();
        //qDebug() <<"--------------------------------------------------------";
        emit ProgressStep((int)nrb.GetHeader().GetLength());
        break;
      case SECTION_HEADER_BLOCK:
        shb = SectionHeaderBlock();
        sect = new SectionNG();
        shb.SetHeader(bh);
        input >> shb;
        BitAlign(input, shb.GetHeader().GetLength());
        input >> bf;
        shb.SetFooter(bf);
        //qDebug() << "SHB id: " << shb->GetIdentifier() << " header: " << shb->GetHeader().GetLength() << " footer: " << bf.GetLength();
        sect->SetBlock(&shb);

        // emit the current host identifier
        // todo: add host name if available
        emit NewHost(shb.GetIdentifier(), "");
        // emit that a section was read

        //qDebug() <<"--------------------------------------------------------";
        emit ProgressStep((int)shb.GetHeader().GetLength());
        break;
      default:
        qDebug() << "UNKNOWN BLOCK TYPE: " << bh.GetType();
        // skip the rest of the unknown block (8 bytes for the block header)
        input.skipRawData(bh.GetLength() - 8);
        emit ProgressStep((int)bh.GetLength());
    }
    // check if we're done
    if (input.atEnd())
    {
      ClearConnectionBuffer();
      return sect;
    }
    else
    {
      // we're not done, check the next block type
      QDataStream::ByteOrder currentorder = input.byteOrder();
      //qDebug() << "Byte order" << input.byteOrder();
      input >> bh;
      if (bh.GetType() == SECTION_HEADER_BLOCK)
      {
        if (currentorder != streamorder)
          bh.Swap();

        input.setByteOrder(streamorder);
        ClearConnectionBuffer();
        return sect;
      }
    }
  }

  ClearConnectionBuffer();
  return sect;
}
