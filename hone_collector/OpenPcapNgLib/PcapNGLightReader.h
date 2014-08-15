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
#ifndef PCAPNGLIGHTREADER_H
#define PCAPNGREADER_H


#include <QDataStream>
#include <QObject>
#include <QQueue>

#include "PcapNgLibGlobal.h"
#include "SectionNG.h"
#include "BlockHeader.h"
#include "Connection.h"
#include "Process.h"

class PCAPNGLIB_EXPORT PCapNGLightReader : public QObject
{
  Q_OBJECT;
private:
    QMap<quint32, Process*> m_processes;
    QMap<quint32, Connection*> m_connections;
    QQueue<Connection*> m_connectionbuffer;
    quint32 m_buffersize;

public:
  PCapNGLightReader();
  ~PCapNGLightReader();

  void ReadFile(QString filename);

private:
  void BitAlign(QDataStream& input, quint64 length);
  SectionNG* ReadSection(QDataStream& input, BlockHeader& bh);
  void ConsumeProcessBlock( ProcessEventBlock& block);
  void ConsumeConnectionBlock( ConnectionEventBlock& block);
  void ConsumeEnhancedPacketBlock( EnhancedPacketBlock& block);
  void SendConnection(Connection* connection);
  void ClearConnectionBuffer();

signals:
  void ConnectionFinished(Connection c);
  void NewHost(QUuid guid, QString name);
  void NewTimeResolution(QPair<quint32, quint32> resolution);
  void ProcessFound(Process p);  
  void Error(const QString& error);
  void ProgressStart(int count);
  void ProgressStep(int step);
  void ProgressEnd();


};
#endif //PCAPNGREADER_H
