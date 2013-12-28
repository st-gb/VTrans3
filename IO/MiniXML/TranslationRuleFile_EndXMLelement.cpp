/*
 * TranslationRuleFile_EndXMLelement.cpp
 *
 *  Created on: 23.12.2013
 *      Author: mr.sys
 */

#include <IO/MiniXML/TranslationRuleFileReader.hpp>
#include <mxml.h> //mxml_node_t, mxml_sax_event_t
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)

namespace VTrans3
{
  namespace MiniXML
  {
    /** @brief This callback function is needed because a translation rule can be
    * added at first when all inner possible conditions were got. */
    void TranslationRuleFileReader::EndTranslationRuleXMLelement(mxml_node_t * node)
    {
//      const char * const strTranslationRule =
//        ::mxmlElementGetAttr(node, "translation_rule");
      EndTranslationRuleXMLelement();
    }
  }
}
