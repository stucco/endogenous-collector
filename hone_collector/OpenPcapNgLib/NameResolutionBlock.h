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
#ifndef NAMERESOLUTIONBLOCK_H
#define NAMERESOLUTIONBLOCK_H
#include <QDataStream>
#include <QMap>
#include "Block.h"
#include "OptionField.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT NameResolutionBlock :
  public Block
{
private:
  QMap<quint16,OptionField> m_options;
  QByteArray m_rawData;
  QList<OptionField> m_records;

public:
  NameResolutionBlock(void);
  ~NameResolutionBlock(void);
  void Clear(){m_options.clear();m_records.clear();m_rawData.clear();}
  QMap<quint16,OptionField> GetOptions(){return m_options;}
  QList<OptionField> GetRecords(){return m_records;}
  void ReadBody(QDataStream &in);
  void SetOptions(QMap<quint16,OptionField> options){m_options = options;}
  void SetRecords(QList<OptionField> records){m_records = records;}
};

QDataStream &operator>>(QDataStream &in, NameResolutionBlock &nameResolution);
QDataStream &operator>>(QDataStream &in, NameResolutionBlock *nameResolution);

#endif //NAMERESOLUTIONBLOCK_H
