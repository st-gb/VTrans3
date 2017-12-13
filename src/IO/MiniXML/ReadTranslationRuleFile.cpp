/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef TEST_MINI_XML
  void MiniXMLconfigReader::ReadVocAttributeDefinitionFile(
    const std::string & cr_stdstrFilePath)
  {
	LOGN_DEBUG("begin")
//    VTrans3::MiniXML::VocAttributeDefintionHandler
//      vocAttributeDefintionHandler(m_translationController);
    ReadFile(cr_stdstrFilePath.c_str(), VTrans3::MiniXML::VocAttributeDefintionFile::sax_cb);
  }
  
  void MiniXMLconfigReader::ReadTranslationRuleFile(const std::string & cr_stdstrFilePath)
  {
    TranslationRuleFileReader translationRuleFileReader(
        s_p_translationController->m_translateparsebyrisetree
        , s_p_translationController->m_parsebyrise
        , * s_p_translationController
        , * this
        );
//    ReadFile(cr_stdstrFilePath.c_str(), VTrans3::MiniXML::ReadTranslationRuleFile::sax_cb);
    translationRuleFileReader.Process(cr_stdstrFilePath);
  }
#endif //#ifndef TEST_MINI_XML
