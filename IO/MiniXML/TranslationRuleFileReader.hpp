/*
 * TranslationRuleFileReader.hpp
 *
 *  Created on: 23.12.2013
 *      Author: mr.sys
 */

#ifndef TRANSLATIONRULEFILEREADER_HPP_
#define TRANSLATIONRULEFILEREADER_HPP_

#include <IO/rules/TranslationRuleFileReaderBase.hpp> //base class
#include <mxml.h> //mxml_node_t, mxml_sax_event_t

namespace VTrans3
{
  namespace MiniXML
  {
    namespace ReadTranslationRuleFile
    {
      /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
      void SAX_callback(
        mxml_node_t * node,
        mxml_sax_event_t event,
        void * data
        );
    }

    class TranslationRuleFileReader
      : public VTrans3::TranslationRuleFileReaderBase//<mxml_node_t *>
//        , public VTrans3::MiniXML::MiniXMLconfigReader
    {
    public:
      TranslationRuleFileReader(
        TranslateParseByRiseTree & r_translateparsebyrise ,
        ParseByRise & r_parsebyrise
        , I_UserInterface & r_userinterface
        , ConfigurationHandler_type & configurationReader
        );
      virtual
      ~TranslationRuleFileReader();

      /** Implementation with same method name seems to hide base class function
       *  So declare base class's "start" function here again.
      //see http://stackoverflow.com/questions/16005894/a-function-from-a-base-class-is-being-hidden-in-the-subclass-how-do-i-fix-this */
      /** If not declared: "error: no matching function for call to 'VTrans3::
      * MiniXML::TranslationRuleFileReader::EndTranslationRuleXMLelement()' "*/
      using VTrans3::TranslationRuleFileReaderBase//<mxml_node_t *>
        ::EndTranslationRuleXMLelement;
      void EndTranslationRuleXMLelement(mxml_node_t * node);
      void GetAttributeNameOrTranslationString(mxml_node_t * node);
      void GetChildNodeInsertionDirection(mxml_node_t * node);
      void GetParentNodeInsertionDirection(mxml_node_t * node);
      void GetStringTransformationFunctionName(mxml_node_t * node);
      void GetSyntaxTreePathWhereToInsertTranslation(mxml_node_t * node);
      void HandleTranslationRuleElementName( mxml_node_t * );
      void Process(const std::string & filePath);
      void ShowMessageToUser(const /*char * const*/ std::string & cpc_chMessage);
    };
//    TranslationRuleFileReader * g_p_translationRuleFileReader;
  } /* namespace MiniXML */
} /* namespace VTrans3 */

#endif /* TRANSLATIONRULEFILEREADER_HPP_ */
