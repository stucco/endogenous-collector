//  Created By: Daniel Best

#ifndef STUCCO_H
#define STUCCO_H

#include <QObject>
#include <QMap>

#include <Process.h>

class Stucco : public QObject
{
  Q_OBJECT

private:
  quint64 m_divisor;
  QTextStream* m_outstream;
  QMap<quint32, Process> m_processes;
  QPair<quint32, quint32> m_resolution;

public:
    Stucco(QObject *parent=0);
    ~Stucco();

    void ParseFile(QString input, QString output);

private:
    
  private slots:
        void OnHandleProcess(Process p);
        void OnHandleConnectionFinished(Connection c);        
        void OnHandleError(const QString &message);
        void OnHandleNewTimeResolution(QPair<quint32, quint32> resolution);
};

#endif // STUCCO_H
