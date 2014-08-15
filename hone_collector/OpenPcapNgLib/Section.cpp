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
#include "Section.h"

Section::Section(void): m_timeResolution(10,6)
{
}


Section::~Section(void)
{
}

//------------------------------------------------------------------------------
// GetTimeResolution
//
// Note: Assume that all of the interfaces have the same resolution for now
QPair<quint32, quint32> Section::GetTimeResolution()
{
  return m_timeResolution;
}

//------------------------------------------------------------------------------
// SetTimeResolution
//
// Note: Assume that all of the interfaces have the same resolution for now
void Section::SetTimeResolution(InterfaceDescriptionBlock *iface)
{
  QPair<quint32, quint32> resolution;
  resolution.first = 10;
  resolution.second = 6;

  if(iface->GetOptions().contains(OPTION_TS_RESOLUTION))
  {
    quint32 optres = iface->GetOptions().value(OPTION_TS_RESOLUTION).GetUIntegerValue(32);
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
}
