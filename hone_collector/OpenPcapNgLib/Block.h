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
#ifndef BLOCK_H
#define BLOCK_H
#include <QDataStream>
#include "BlockHeader.h"
#include "BlockFooter.h"
#include "PcapNgLibGlobal.h"

class PCAPNGLIB_EXPORT Block
{

protected:
  BlockHeader m_header;
  BlockFooter m_footer;

public:
  Block(void);
  ~Block(void);
  BlockHeader &GetHeader(){return m_header;}
  BlockFooter &GetFooter(){return m_footer;}
  void SetHeader(BlockHeader header){m_header = header;}
  void SetFooter(BlockFooter footer){m_footer = footer;}
};
#endif //BLOCK_H
