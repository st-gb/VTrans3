/*
 * user_interface_control_actions.h
 *
 *  Created on: Sep 28, 2010
 *      Author: Stefan
 */

#ifndef USER_INTERFACE_CONTROL_ACTIONS_H_
#define USER_INTERFACE_CONTROL_ACTIONS_H_

//An enum guarantees a unique number for each element.
enum
{
  ID_Close = 1 ,
  ID_AddGrammarRules ,
  ID_AddTransformationRules ,
  ID_AddTranslationRules ,
  ID_AddVocAttrDefs ,
  ID_DrawParseTree,
  ID_LoadDictionary ,
  ID_ReInitGrammarRules ,
  ID_ShowTokenIndex2GrammarPart ,
  ID_RemoveGrammarRules ,
  ID_RemoveTransformationRules ,
  ID_RemoveTranslationRules ,
  ID_RemoveVocAttrDefs ,
  ID_ResolveSuperclass ,
  ID_Resolve1ParseLevel ,
  ID_Translate ,
  ID_TruncateLogFile,
  ID_Info
};


#endif /* USER_INTERFACE_CONTROL_ACTIONS_H_ */
