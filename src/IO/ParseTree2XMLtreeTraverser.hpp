/** ParseTree2XMLtreeTraverser.hpp
 *  Created on: 24.09.2011
 *      Author: Stefan   */

#ifndef PARSETREE2XMLTREETRAVERSER_HPP_
#define PARSETREE2XMLTREETRAVERSER_HPP_

//class ParseTreeTraverser::DirectingLeavesMultipleIterTraverser
//#include <Parse/DirectingLeavesMultipleIterTraverser.hpp>
//class ParseTreeTraverser::DirectingLeavesTraverser
#include <ParseTree/DirectingLeavesInSingleIterationTraverser.hpp>
//#include <string>
#include <data_structures/ByteArray.hpp> //class ByteArray

namespace ParseTreeTraverser
{
  class ParseTree2XMLtreeTraverser
    : public //DirectingLeavesMultipleIterTraverser
      DirectingLeavesInSingleIterationTraverser
  {
    std::vector<GrammarPartPointerAndParseLevel>
      m_std_vec_p_grammarpart_and_parselevelCurrentParseTreePath;
  public:
//    std::string m_std_strXML;
//    unsigned char * m_pchUTF8_XML;
    ByteArray m_byteArray;
    ParseTree2XMLtreeTraverser(
      const GrammarPart * p_grammarpartStartNode
      , const VTrans3::BottomUpParser &
      );
    virtual
    ~ParseTree2XMLtreeTraverser();

//    void AddToUTF8sequence(
//      const BYTE * const arby,
//      fastestUnsignedDataType utf8arraySizeInByte = 0);
    //Called from base class "DirectingLeavesMultipleIterTraverser" (they are
    //virtual in the base class).
    void CurrentNodeIsLastAddedRightChild() ;
    void LeaveFound();
    void ParseTreePathAdded();
    void ParseTreePathPopped();
  };

} /* namespace ParseTreeTraverser */
#endif /* PARSETREE2XMLTREETRAVERSER_HPP_ */
