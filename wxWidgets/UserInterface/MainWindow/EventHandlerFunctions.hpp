/* 
 * File:   EventHandlerFunctions.hpp
 * Author: stefan
 *
 * Created on May 9, 2013, 2:52 PM
 */

#ifndef EVENTHANDLERFUNCTIONS_HPP
#define	EVENTHANDLERFUNCTIONS_HPP

    void OnAddGrammarRules(wxCommandEvent & wxcmd);
    void OnChar( wxKeyEvent & e);
    void OnDecreaseParseTreePanelFontSize(wxCommandEvent & wxcmd);
    void OnIncreaseParseTreePanelFontSize(wxCommandEvent & wxcmd);
    void OnShowGrammarPartMemoryAddress(wxCommandEvent & wxcmd);
    void OnShowTranslatedWord(wxCommandEvent & wxcmd);
    void OnAddTransformationRules(wxCommandEvent & wxcmd);
    void OnAddTranslationRules(wxCommandEvent & wxcmd);
    void OnAddVocAttrDefs(wxCommandEvent & wxcmd);
    void OnClose(wxCloseEvent & wxcmd);
    void OnDrawParseTreeButton(wxCommandEvent & wxcmd);
    void OnInfoButton(wxCommandEvent & wxcmd);
    void OnLoadDictionaryButton(wxCommandEvent & wxcmd);
    void OnUpdateAfterTranslation(wxCommandEvent &);
    void OnReInitGrammarRulesButton(wxCommandEvent & wxcmd);
    void OnRemoveGrammarRules(wxCommandEvent & wxcmd);
    void OnRemoveTransformationRules(wxCommandEvent & wxcmd);
    void OnRemoveTranslationRules(wxCommandEvent & wxcmd);
    void OnRemoveVocAttrDefs(wxCommandEvent & wxcmd);
    void OnResolveSuperclassGrammarParts(wxCommandEvent & wxcmd);
    void OnResolve1ParseLevelButton(wxCommandEvent & wxcmd);
    void OnShowTokenIndex2GrammarPartButton(wxCommandEvent & wxcmd);
    void OnShowParseRulesButton(wxCommandEvent & wxcmd);
    void OnShowTranslationRulesButton(wxCommandEvent & wxcmd);
    void OnLoadDictionaryTimerEvent(wxTimerEvent &event);
    void OnLookupWord(wxCommandEvent & wxcmd);
    void OnSettings(wxCommandEvent & wxcmd );
    void OnShowDictionaryStatistics(wxCommandEvent & wxcmd);
    void OnTimerEvent(wxTimerEvent &event);
    void OnTranslateButton(wxCommandEvent & wxcmd);
    void OnTruncateLogFileButton( wxCommandEvent & wxcmd );
    void OnUnloadDictionary( wxCommandEvent & wxcmd );
    void UnloadDictionaryShowingStatus();
    void UnloadDictionaryShowingStatusAndSendCloseEvent();
  DECLARE_EVENT_TABLE()

#endif	/* EVENTHANDLERFUNCTIONS_HPP */

