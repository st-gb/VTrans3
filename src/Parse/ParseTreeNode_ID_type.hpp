/* * File:   ParseTreeNode_ID_type.hpp
 * Author: sg
 * Created on 27. September 2019, 18:13 */

#ifndef PARSETREENODE_ID_TYPE_HPP
#define PARSETREENODE_ID_TYPE_HPP

#include <hardware/CPU/fastest_data_type.h>///parseTreeNodeIDtype

namespace VTrans3
{
///typdef'ing the parse tree node ID data type has the advantage to change it
/// at a central point for all occurances.
typedef WORD parseTreeNodeIDtype;
typedef parseTreeNodeIDtype PTN_IDtype;
typedef fastestUnsignedDataType indexDataType;
}

#endif /* PARSETREENODE_ID_TYPE_HPP */
