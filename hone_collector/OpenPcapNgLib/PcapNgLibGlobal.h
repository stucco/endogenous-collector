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

#ifndef PCAPNGLIB_GLOBAL_H
#define PCAPNGLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef PCAPNGLIB_LIB
# define PCAPNGLIB_EXPORT Q_DECL_EXPORT
#else
# define PCAPNGLIB_EXPORT Q_DECL_IMPORT
#endif

#define SWAP32(x) (((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8)  | ((x & 0x000000FF) << 24))

const quint32 SECTION_HEADER_BLOCK = 0x0A0D0D0A;
const quint32 INTERFACE_DESCRIPTION_BLOCK = 0x00000001;
const quint32 ENHANCED_PACKET_BLOCK = 0x00000006;
const quint32 SIMPLE_PACKET_BLOCK = 0x00000003;
const quint32 NAME_RESOLUTION_BLOCK = 0x00000004;
const quint32 CONNECTION_EVENT_BLOCK = 0x00000102;
const quint32 PROCESS_EVENT_BLOCK = 0x00000101;
const quint32 SWAPPED_HOST_BYTE_ORDER = 0;
const quint32 HOST_BYTE_ORDER = 1;
const quint32 UNKNOWN_HOST_BYTE_ORDER = 2;
const quint32 SECTION_HEADER_BLOCK_MAGIC = 0x1A2B3C4D;
const quint64 SECTION_LENGTH_UNSPECIFIED = 0xFFFFFFFFFFFFFFFF;

const quint16 OPTION_END_OF_OPTION = 0;
const quint16 OPTION_COMMENT = 1;
const quint32 OPTION_EVENT = 2;
const quint32 OPTION_PATH = 3;
const quint32 OPTION_ARGV = 4;
const quint32 OPTION_PROCESS_ID = 258;
const quint32 OPTION_CONNECTION_ID = 257;
const quint32 OPTION_DIRECTION = 2;
const quint32 OPTION_SYSTEM_IDENTIFIER = 257;
const quint32 OPTION_TS_RESOLUTION = 9;
const quint32 OPTION_PPID = 5;
const quint32 OPTION_USER = 8;
const quint32 OPTION_USERID = 6;
const quint32 OPTION_HOST_ID_TYPE_GUID = 1;

#endif // PCAPNGLIB_GLOBAL_H
