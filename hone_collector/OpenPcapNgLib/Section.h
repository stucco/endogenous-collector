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

#ifndef SECTION_H
#define SECTION_H

#include <QObject>
#include <QDebug>

#include "PcapNgLibGlobal.h"
#include "Process.h"
#include "SectionHeaderBlock.h"
#include "InterfaceDescriptionBlock.h"

class PCAPNGLIB_EXPORT Section  : public QObject
{  
  Q_OBJECT

private:

  QPair<quint32, quint32> m_timeResolution;

public:
  Section(void);
  ~Section(void);


  QPair<quint32, quint32> GetTimeResolution();
  void SetTimeResolution(InterfaceDescriptionBlock *iface);
};

#endif // SECTIONNG_H
