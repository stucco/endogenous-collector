//  Created By: Daniel Best

#include "Stucco.h"

#include <QtCore/QCoreApplication>
#include <QDateTime>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <qmath.h>

// light version
#include <PcapNGLightReader.h>
#include <Process.h>
#include <Section.h>
#include <Connection.h>

Stucco::Stucco(QObject *parent)
  : QObject(parent), m_divisor(1), m_resolution(10,6)
{

}

Stucco::~Stucco()
{

}

void Stucco::OnHandleNewTimeResolution(QPair<quint32, quint32> resolution)
{
//  qDebug() << "OnHandleNewTimeResolution";

  m_resolution = resolution;
  m_divisor = 1;
      if (m_resolution.first == 10)
      {
        // negative power of 10, we want 10^-6
        if (m_resolution.second > 6)
        {
          m_divisor = qPow(10, m_resolution.second - 6);
        }
        else if (m_resolution.second < 6)
        {
          m_divisor = qPow(10, -(6 - m_resolution.second));
        }
      }
      else
      {
        // to get microseconds
        // micro = (ticks / frequency) * 10^-6
        //       = frequency / 10^-6
        quint64 freq = qPow(2, m_resolution.second);
        m_divisor = freq / qPow(10, 6);
      }
}
void Stucco::OnHandleConnectionFinished(Connection c)
{
//  qDebug() << "OnHandleConnectionFinished";

  if(m_processes.contains(c.getProcessID()))
  {
    Process p = m_processes.value(c.getProcessID());
    *m_outstream <<
    p.GetUser() << "," <<
    p.GetUid() << "," <<
    p.GetProcessId() << "," <<
    p.GetPpid() << "," <<
    p.GetPath() << "," <<
    p.GetArgv() << "," <<
    c.GetConnectionID() << "," <<
    c.GetTimestamp() / m_divisor / 1000 << "," <<
    c.GetSourcePort() << "," <<
    c.GetDestPort() << "," <<
    c.getVersion() << "," <<
    c.GetSourceAddress().toString() << "," <<
    c.GetDestAddress().toString() << "," << 
    c.GetByteCount() << "," <<
    c.GetPacketCount() << "\n";
    m_outstream->flush();
  }
}

void Stucco::OnHandleError(const QString &message)
{
  qDebug() << "ERROR: " << message;
}

void Stucco::OnHandleProcess(Process p)
{
//  qDebug() << "OnHandleProcess";

  m_processes.insert(p.GetProcessId(), p);
}

void Stucco::ParseFile(QString input, QString output)
{

  QList<quint32> interfaceIDs;
  QPair<quint32, quint32> timeres;

  PCapNGLightReader reader;
  connect(&reader, SIGNAL(NewTimeResolution(QPair<quint32, quint32>)),SLOT(OnHandleNewTimeResolution(QPair<quint32, quint32>)));
  connect(&reader, SIGNAL(ProcessFound(Process)),SLOT(OnHandleProcess(Process)));
  connect(&reader, SIGNAL(ConnectionFinished(Connection)),SLOT(OnHandleConnectionFinished(Connection)));
  connect(&reader, SIGNAL(Error(QString)),SLOT(OnHandleError(QString)));

  QFile *outfile = NULL;
  m_outstream = NULL;
  if (output == "-") 
  {
    qDebug() << "Streaming to stdout";
    m_outstream = new QTextStream(stdout, QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered);
  }
  else
  {
    outfile = new QFile(output);
    if ( outfile->open(QIODevice::WriteOnly | QIODevice::Text) )
    {
      qDebug() << "Writing to " << output;
      m_outstream = new QTextStream(outfile);
    }
    else
    {
      qDebug() << "Could not open file '" << output << "'\n";
    }
  }

  if (m_outstream != NULL)
  {
    *m_outstream << "user,uid,proc_pid,proc_ppid,path,argv,conn_id,timestamp_epoch_ms,source_port,dest_port,ip_version,source_ip,dest_ip,byte_cnt,packet_cnt\n";
    m_outstream->flush();
    reader.ReadFile(input);
  }
  else
  {
    qDebug() << "Cannot output data";
  }

  if (outfile != NULL)
  {
    outfile->close();
    delete outfile;
  }
}

