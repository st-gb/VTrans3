/*
 * user_interface_control_actions.h
 *
 *  Created on: Sep 28, 2010
 *      Author: Stefan
 */

#ifndef USER_INTERFACE_CONTROL_ACTIONS_H_
#define USER_INTERFACE_CONTROL_ACTIONS_H_

/** IDs for the event table (assignment of event ID -> event handler function).
 *  Use an enum because it guarantees a unique number for each element. */
enum user_interface_control_actions
{
  ID_Close = 1 ,
  ID_AddGrammarRules ,
  ID_AddTransformationRules ,
  ID_AddTranslationRules ,
  ID_ShowTranslationRules,
  ID_ShowParseRules,
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
  ID_Settings,
  ID_Translate ,
  ID_LookupWord ,
  ID_ShowDictionaryStatistics,
  ID_UnloadDictionary,
  ID_TruncateLogFile,
  ID_Info,
  ID_Timer,
  ID_InputText,
  ID_ShowGrammarPartMemoryAddress ,
  ID_ShowTranslatedWord,
  ID_DecreaseFontSize,
  ID_IncreaseFontSize,
  ID_LogLevel_Warning,
  ID_LogLevel_Debug,
  ID_Translate_On_Text_Changes
};


#endif /* USER_INTERFACE_CONTROL_ACTIONS_H_ */
