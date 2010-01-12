Infos (well-documented):

bei MSCV Fehler C1010: auch in den Datei-Eigenschaften der Quelldatei
	unter "C/C++"->"vorkompilierte Header"->"PCH..." den Pfad zur "stdafx.h"
	eintragen

bei "#include" kopiert der Präprozessor den gesamten Inhalt der angegebenen 
Datei an diese Stelle. Wenn die 2. Datei, die von der 1. Datei per "#include" 
eingebunden wird, die 1. Datei per "#include" einbindet, kommt es zu einer 
unendlichen Rekursion und deshalb zu einem Compiler-Fehler.

bei einem Laufzeitfehler dieses Computerprogramms durch Aufruf einer
Funktion kann es sein, dass auf nicht-existente Steuerelemente 
zugegriffen wird

Bei der Debug-Version, oder wenn das Makro "_DEBUG" in der StdAfx.h definiert ist
("#define") auch in der Release-Version, insofern die Dateien, in denen eine 
Ausgabe erfolgt, die Datei "StdAfx.h" einbinden, erfolgt eine Ausgabe nützlicher
Informationen in die Datei 
"log.txt" im Verzeichnis VTrans statt, wenn VTrans von Visual C++ aus gestartet wird,
andererseits im gleichen Verzeichnis wie die exe-Datei.

viele Funktionen (besonders in den Dateien Translate.h und Translate.cpp) werden
zur Zeit nicht genutzt. Ich werde sie sicherlich noch aus dem Code herausnehmen.

In vielen (oder allen) Headerdateien wird abgefragt, ob bestimmte Makros 
NICHT definiert sind (#ifndef) und danach wird exakt dieses Makro 
definiert (#define). Die Abfrage (#ifndef) geht dann bis zum Ende der 
Datei, wo dann ein #endif steht.
Das dient dazu, Mehrfacheinbindungen (#include) zu vermeiden, die einen 
Compiler- oder Linker-Fehler verursachen. Man nennt dieses Verfahren
auch Include-Wächter.

const DATENTYP & BEZEICHNER:
(Bsp.: const int & i)
diese Variable soll vor dem versehentlichen Verändern geschützt werden (const)
und zusätzlich als Referenz (&) dienen, um speichersparend zu sein, und
nicht als Kopie den doppelten Speicherplatz zu belegen
const DATENTYP & BEZEICHNER wird in meinen Funktionen oft als Parameter verwendet

Ausgaben mittels printf() finden für die Fehlersuche (debugging) in die Datei 
"log.txt" statt, weil das
Makro TRACE(), das eigentlich dafür gedacht ist, meiner Meinung nach nicht 
synchron abläuft und einige Ausgaben nicht stattfinden (vermutlich weil die
Priorität zu niedrig ist)
Diese Ausgaben sollte für die Kompilation in Makros verpackt (#ifdef _DEBUG ... 
#endif) werden, damit die Release-Version nicht zu langsam läuft.

Namensgebung: ein 'm_' als Präfix zeigt, dass es sich um eine Membervariable
handelt. ein 'p' zeigt, dass der Bezeichner ein Zeiger ist.
Beispiele:
 CString * m_Var; // eine Membervariable vom Typ CString
 CString * pVar; // ein Zeiger auf ein CString
 CString * m_pVar; // eine Membervariable, die ein Zeiger auf ein CString ist

Die Bezeichner (Namen für Variablen und Konstanten) und die Funktions-
namen sind teilweise sehr lang gehalten. Da mir kein kürzerer Name eingefallen
ist, und auch weil dieser dann bestimmt zu kryptisch gewesen wäre, habe ich
sie teilweise voll ausgeschrieben, so dass die Verwendung gleich offensichtlich
ist.

Die Hilfedatei wird aus der Datei "AfxCore.rtf" generiert. Bis jetzt konnte ich
neue Indizes nur mit Microsoft Word erstellen, die Erstellung mit OpenOffice
schlug insofern fehl, dass nach einschliesslich des neu eingefügten Index und 
danach keine Indizes mehr in der Liste "Index" aufraten. Wenn man neue 
Zeichenketten im Index haben möchte, muss man eine folgende Fußnote einfügen:"#K"
Wenn in mindestens zwei Kennungen ("K"), die den Text für den Index darstellen,
ein Doppelpunkt steht und der Text davor identisch ist, dann taucht in den nach-
folgenden Texten, deren Änfange identisch sind, im Index nur noch der Teil des 
Textes nach dem Doppelpunkt auf.

Der Text im oberen oder unteren Teil des Kindfensters wird in den Funktionen
CEnglishView::OnChar, CEnglishView::OnKeyDown, in CMainFrame::OnEditPaste und
CMainFrame::OnEditCut geändert

Zeitformen
----------

im Englischen                                 im Deutschen
simple present              I go.             Präsens                          Ich gehe.
simple present              I must go.        Präsens mit modalem Hilfsverb    Ich muss gehen.
simple past                 I went.           Präteritum                       Ich ging.
simple past                 I had to go.      Präteritum mit modalem Hilfsverb Ich musste gehen.
present progressive         I am going.       Präsens                          Ich gehe (gerade).
past progressive            I was going.      Präteritum                       Ich ging (gerade).
present perfect             I was gone.       Perfekt                          Ich bin gegangen.
present perfect             I must have gone  Perfekt mit modalem Hilfsverb    I muss gegangen sein.
present perfect progressive I have been gone. Perfekt                          Ich bin gegangen.
simple future               I will go.        Zukunft                          Ich werde gehen.
future progressive          I will be going.  Zukunft                          Ich werde gehen.

Passiv
------

im Englischen                                       im Deutschen
simple present      Energy is used.                 Präsens    Energie wird benutzt.
simple present      Energy can be used.             Präsens    Energie kann benutzt werden.
simple past         Energy was used.                Präteritum Energie wurde benutzt.
simple past         Energy could was used.          Präteritum Energie konnte benutzt werden.
present progressive Energy is being used.           Präsens    Energie wird (gerade) benutzt.
past progressive    Energy was being used.          Präteritum Energie wurde (gerade) benutzt.
present perfect     Energy has been used.           Perfekt    Energie ist benutzt worden.
present perfect     Energy has been given a chance. Perfekt    Energie ist eine Chance gegeben worden.
simple future       Energy will be used.            Zukunft    Energie wird benutzt werden.

bei present progressive und past progressive ist die deutsche Übersetzung 
vielleicht doch anders:

present progressive Energy is being used.  Präsens    Energie ist benutzt worden.
past progressive    Energy was being used. Präteritum Energie war benutzt worden.

Ich weiß es nicht genau und werde mich sicherlich noch erkundigen.

Fragesätze
----------

Zeitform            Satz          deutsche Übersetzung
simple present      Do I go?      Gehe ich?
simple present      Can I go?     Kann ich gehen?
simple past         Did I go?     Ging ich?
simple past         Could I go?   Konnte ich gehen?
present perfect     Have I gone?  Bin ich gegangen?
present progressive Am I going?   Gehe ich?
past progressive    Was I going?  Ging ich?
will-future         Will I go?    Werde ich gehen?

Fragesätze mit Verneinung
-------------------------

Zeitform            Fragesatz             Kurzform        in Deutsch
simple present      Do not I go?          Don't I go?     Gehe ich nicht?
simple present      Can not I go?         Can't I go?     Kann ich nicht gehen?
simple past         Did not I go?         Didn't I go?    Ging ich nicht?
simple past         Could not I go?       Couldn't I go?  Konnte ich nicht gehen?
present perfect     Have not I gone?      Haven't I gone? Bin ich nicht gegangen?
present progressive Am not I going?              -        Gehe ich nicht?
past progressive    Was not I going?      Wasn't I going? Ging ich nicht?
will-future         Will not I go?        Won't I go?     Werde ich nicht gehen?
going-to-future     Am not I going to go?        -        Habe ich nicht die Absicht zu gehen?

Fragesätze, die nach einem Subjekt fragen
-----------------------------------------

Zeitform                    Satz                    deutsche Übersetzung
simple present              Who goes?               Wer geht?
simple present              Who can go?             Wer kann gehen?
simple past                 Who went?               Wer ging?
simple past                 Who was able to go?     Wer konnte gehen?
simple past perfect         Who was gone?           Wer war gegangen?
simple present perfect      Who has gone?           Wer ist gegangen?
present progressive         Who is going?           Wer geht (gerade)?
past progressive            Who was going?          Wer ging (zu diesem Zeitpunkt gerade)?
present perfect progressive Who has been going?     Wer ist (zu diesem Zeitpunkt gerade) gegangen?
will-future                 Who will go?            Wer wird gehen?
future perfect              Who will have finished? Wer wird beendet haben?

Fragesätze, die nach einem Subjekt fragen mit Verneinung
--------------------------------------------------------

Zeitform            Fragesatz               Kurzform               in Deutsch
simple present      Who does not go?        Who doesn't go?        Wer geht nicht?
simple present      Who can not go?         Who can't go?          Wer kann nicht gehen?
simple past         Who did not go?         Who didn't go?         Wet ging nicht?
simple past         Who was not able to go? Who wasn't able to go? Wer konnte nicht gehen?
present perfect     Who has not gone?       Who hasn't gone?       Wer ist nicht gegangen?
present progressive Who is not going?       Who isn't going?       Wer geht (gerade) nicht?
past progressive    Who was not going?      Who wasn't going?      Wer ging (zu diesem Zeitpunkt gerade) nicht?
will-future         Who will not go?        Who won't go?          Wer wird nicht gehen?
going-to-future     Who is not going to go? Who isn't going to go? Wer hat nicht die Absicht zu gehen?

passive Fragesätze
------------------

Zeitform            Fragesatz                    in Deutsch
simple present      Am I used?                   Werde ich benutzt?
simple present      Can I be used?               Kann ich benutzt werden?
simple past         Was I used?                  Werde ich benutzt?
simple present      Could I be used?             Konnte ich benutzt werden?
present progressive Am I being used?             Werde ich benutzt?
past progressive    Was I being used?            Wurde ich benutzt?
present perfect     Have I been used?            Bin ich benutzt worden?
present perfect     Can I have been used?        Kann ich benutzt worden sein?
present perfect     Have I been given a dog?     Ist mir ein Hund gegeben worden?
present perfect     Can I have been given a dog? Kann mir ein Hund gegeben worden sein?
will-future         Will I be used?              Werde ich benutzt werden?

passive Fragesätze mit Verneinung
---------------------------------

Zeitform            Fragesatz                        Kurzform                       Übersetzung                                  Zeitform im Deutschen
simple present      Am not I used?                            -                     Werde ich nicht benutzt?                     Präsens (Gegenwart)
simple present      Can not I be used?               Can't I be used?               Kann ich nicht benutzt werden?               Präsens (Gegenwart) mit modalem Hilfsverb
simple past         Was not I used?                  Wasn't I used?                 Wurde ich nicht benutzt?                     Präteritum (Vergangenheit)
simple present      Could not I be used?             Couldn't I be used?            Konnte ich nicht benutzt werden?             Präteritum (Vergangenheit) mit modalem Hilfsverb
present progressive Am I being used?                          -                     Werde ich nicht benutzt?                     Präsens (Gegenwart)
past progressive    Was not I being used?            Wasn't I being used?           Wurde ich nicht benutzt?                     Präteritum (Vergangenheit)
present perfect     Have not I been used?            Haven't I been used?           Bin ich nicht benutzt worden?                Perfekt
present perfect     Can not I have been used?        Can't I have been used?        Kann ich nicht benutzt worden sein?          Perfekt mit modalem Hilfsverb
present perfect     Have not I been given a dog?     Haven't I been given a dog?    Ist mir nicht ein Hund gegeben worden?       Perfekt
present perfect     Can not I have been given a dog? Can't I have been given a dog? Kann mir nicht ein Hund gegeben worden sein? Perfekt mit modalem Hilfsverb
will-future         Will not I be used?              Won't I be used?               Werde ich nicht benutzt werden?              Zukunft

passive Fragesätze, die nach einem Subjekt fragen
-------------------------------------------------

Zeitform            Fragesatz                      in Deutsch
simple present      Who is used?                   Wer wird benutzt?
simple present      Who can be used?               Wer kann benutzt werden?
simple past         Who was used?                  Wer wird benutzt?
simple present      Who was able to be used?       Wer konnte benutzt werden?
present progressive Who is being used?             Wer wird (gerade) benutzt?
past progressive    Who was being used?            Wer wurde (zu diesem Zeitpunkt) benutzt?
present perfect     Who has been used?             Wer ist benutzt worden?
present perfect     Who can have been used?        Wer kann benutzt worden sein?
present perfect     Who has been given a dog?      Wem ist ein Hund gegeben worden?
present perfect     Who can have been given a dog? Wem kann ein Hund gegeben worden sein?
will-future         Who will be used?              Wer wird benutzt werden?

passive Fragesätze, die nach einem Subjekt fragen mit Verneinung
----------------------------------------------------------------

Zeitform            Fragesatz                          Kurzform                         Übersetzung                                    Zeitform im Deutschen
simple present      Who is not used?                   Who isn't used?                  Wer wird nicht benutzt?                        Präsens (Gegenwart)
simple present      Who can not be used?               Who can't be used?               Wer kann nicht benutzt werden?                 Präsens (Gegenwart) mit modalem Hilfsverb
simple past         Who was not used?                  Who wasn't used?                 Wer wurde nicht benutzt?                       Präteritum (Vergangenheit)
simple present      Who was not able to be used?       Who wasn't able to be used?      Wer konnte ich nicht benutzt werden?           Präteritum (Vergangenheit) mit modalem Hilfsverb
present progressive Who is not being used?                          -                   Wer wird (gerade) nicht benutzt?               Präsens (Gegenwart)
past progressive    Who was not being used?            Who wasn't being used?           Wer wurde (zu diesem Zeitpunkt) nicht benutzt? Präteritum (Vergangenheit)
present perfect     Who has not been used?             Who hasn't been used?            Wer ist nicht benutzt worden?                  Perfekt
present perfect     Who can not have been used?        Who can't have been used?        Wer kann nicht benutzt worden sein?            Perfekt mit modalem Hilfsverb
present perfect     Who has not been given a dog?      Who hasn't been given a dog?     Wem ist kein Hund gegeben worden?              Perfekt
present perfect     Who can not have been given a dog? Who can't have been given a dog? Wem kann kein Hund gegeben worden sein?        Perfekt mit modalem Hilfsverb
will-future         Who will not be used?              Who won't be used?               Wer wird nicht benutzt werden?                 Zukunft

ZEITFORMEN NACH TELEKOLLEG I ENGLISCH:
Band 2:
Past Perfect Continuous Seite 100
I had been sitting there for five minutes when an old schoolfriend 
of mine came in.->Ich hatte dort ... gesessen als ein alter ....
"had been" + Partizip Präsens

Band 3 Seite 38:
Future Continuous
I'll be working late.->Ich werde spät arbeiten.
I won't be working tomorrow.->Ich werde morgen nicht arbeiten.
will+be+ing-Form des Verbs

Band 3 Seite 40: Future Perfect
He'll have finished the article by 11 o'clock.->Er wird...beendet haben.
will+Perfektform des Verbs

Übungsband:
Simple Present
Jane works in an office.
She makes breakfast every morning.
Do you work on Sundays?
Does Russell smoke?
I don't work on Sundays.
No, he doesn't.

Present Continuous
She is working.
He is reading the paper at the moment.

Simple Past
When did he lose his trumpet?
He lost his trumpet last Friday.

Past Continuous
He was reading the newspaper when I entered the room.
Vergangenheitsform von to be+ing-Form des Verbs

Present Perfect
have/has + Partizip Perfekt
Prices have gone up again.
He's just bought a new car.

Present Perfect Continuous
Übungsband Seite 24:
have/has been + ing-Form des Verbs
How long has he been travelling?
He's been travelling for two weeks now.
Band 2 Seite 91:
I have been living there for half a year.->Ich wohne 
seit einem halben Jahr dort.

Past Perfect
had + Partizip Perfekt
When he got to the station, the train had already left.
I had known him for three years before you met him.

Future Continuous
will (shall) be + ing-Form des Verbs
I'll be working late.
This time tomorrow I'll be sitting on the terrace of my hotel.

Future Perfect
will (shall) have + Partizip Perfekt
He'll have finished the report by 11 o'clock.

Die Verlaufsform ist im Passiv nur für Present und Past Tense möglich. (S. 48)

Future
I'm flying to New York next week.
She's going to 

Wort des ersten Teils des Prädikats (MIT gebeugter Verbform)
-(modales) Hilfsverb
-(gebeugtes) Vollverb
-Präteritum
1. "not" (Verneinung)

Reihenfolge von Wörtern des Teil des Prädikats OHNE gebeugte Verbform:
2. Partizip Perfekt : von "be" bei "had been sitting" (Past Perfect Continuous)
oder Infinitiv von "be" bei "will be working" (Future Continuous)
oder Infinitiv von "have" bei "will have finished" (Future Perfect)
oder Partizip Präsens zum Beispiel bei "is working" (Present Continuous)
oder Infinitiv zum Beispiel bei "Did he lose it?" (Simple Past)
oder Partizip Perfekt zum Beispiel bei "He has bought it." (Present Perfect)
3. Partizip Präsens zum Beispiel bei "I had been sitting." (Past Perfect Continuous)
oder Partizip Perfekt zum Beispiel bei "He'll have finished." (Future Perfect)

Übersetzen eines Satzes
-----------------------

Es werden die Funktionen, die aufgerufen werden, angezeigt.
Anstatt einer Parameterliste werden aber nur die runden Klammern angegeben.
Die Versetzung nach rechts bedeutet, dass eine Funktion die Funktionen, die
nach rechts versetzt sind, (eventuell) aufruft. 

ParseSentence()
  ParseSubSentence()
    ParseSubject()
	  ParseEnumerationElement()
	    ParseENR()
		ParseDefiniteArticleNoun()
		ParseGerund()
	ParseMiddle()
	ParseObject()
	  ParseObjectEnumerationElement
	    ParseENR()
		ParseDefiniteArticleNoun()
		ParseGerund()
	ParseObject()
	  ParseObjectEnumerationElement
	    ParseENR()
		ParseDefiniteArticleNoun()
		ParseGerund()
	ParseManner()
	  ParseMannerEnumerationElement()
	ParsePlace()

Sätze, die übersetzt werden können sollten
------------------------------------------

Hier sind einige Sätze um VTrans auf Funktionsfähigkeit zu testen:

I am only happy when it rains.
Where do you go?
Where have you been?
The number you have dialled is incomplete.
The person you have called is temporarily not available.
You will sometimes find English adverbs in different positions, but if you
follow these rules, you will not make a mistake when you use English.
You are the air I am breathing.
Where are you?
I know you.
I know.
Are you having your fun?
The world still turns.
The fire still burns.
I feel you.
How long must we sing this song?
I just can't get enough.
I still haven't found what I'm looking for.
Try walking in my shoes.
You will stumble in my footsteps.
Now I close my eyes.
I am talking to you now.
I like her.
The boy you love is the man that you fear.
My head speaks a language I don't understand.

verschiedene Zeitformen aktiv/passiv mit/ohne modales Hilfsverb mit/ohne 
Verneinung:
I like you.
I do not like you.
I can like you.
I can not like you.
I am liked.
I am not liked.
I can be liked.
I can not be liked.
I liked you.
I did not like you.
I was liked.
I was not liked.
I will like you.
I will not like you.
I will be liked.
I will not be liked.
I will be walking.
I will not be walking.
I will be being liked.
I will not be being liked.
I will have liked you.
I will not have liked you.
I will have been liked.
I will not have been liked.
I am walking.
I am not walking.
I am being liked.
I am not being liked.
I was walking.
I was not walking.
I was being liked.
I was not being liked.
I have liked you.
I have not liked you.
I can have seen you.
I can not have seen you.
I have been liked.
I have not been liked.
I can have been liked.
I can not have been liked.
I had liked you.
I had not liked you.
I could have seen you.
I could not have seen you.
I had been liked.
I had not been liked.
I could have been liked.
I could not have been liked.
I have been liking you.
I have not been liking you.
I can have been liking you.
I can not have been liking you.
I was been liking.
I was not been liking.
I can have been liking.
I can not have been liking.
I had been liking you.
I had not been liking you.
I could have been liking you.
I could not have been liking you.
I had been liking.
I had not been liking.
I could have been liking.
I could not have been liking.

Features:

-Kombinationsfeld bei Verben, bei denen Präsens und Präteritum identisch sind
-Kombinationsfeld, wenn im Englischen "you" als Aufzählungselement steht
-automatische Übersetzung: nach einer Änderung im Text startet die Grammatik-
überprüfung/Übersetzung

unbekannte Vokabeln rot hervorheben
------------------------------------

1. Beispiel: Verb ist unbekannt: to like
zuerst:

Aufzählung enthalten ?
|
nein:
Gibt es ein Prädikat im Satz?
|
nein:
Gibt es ein Satzglied, dass Subjekt sein könnte?
The teacher likes you.
-----------

wenn ja, dann gucken, ob es ein gültiges Objekt gibt.
The teacher likes you.
                  ---

der Rest wird farbig markiert

2. Beispiel: 
 Satz: The teacher likes you.
 Nomen ist unbekannt: the teacher

Gibt es ein Prädikat im Satz?
|
ja: likes

Gibt es ein Subjekt im Satz?
|
nein: teacher ist noch nicht im Wörterbuch, daher ist das Subjekt ungültig

'The teacher' rot markieren.


Achtung!: wenn in der Klasse EnglishView die Funktion OnCreate()
definiert ist, erzeugt das wahrscheinlich einen Laufzeitfehler. Ich habe 
die Funktion extra wieder herausgenommen (sie wird von Visual C++ 
automatisch generiert), damit das Programm nicht abstürzt. Warum das so 
ist, konnte ich noch nicht klären.

nochmals Achtung: wenn man Funktionen (Methoden) auf Steuerelemente in 
der Funktion EnglishView::OnDraw() anwendet, führt das dazu, das diese
Funktion in einer Endlosschleife läuft (100% Prozessorauslastung).


Subjekt: 
Satz: Subjekt Relativsatz Objekt (The school I go to is great)
      Subjekt Objekt Relativsatz (I like the things you do)

Subjekt: EnglishNounRecord (My brother)
         DefiniteArticleNoun (The school)
         DefiniteArticleNoun OfPhrase (The school of my brother)

OfPhrase: 

Beschreibung der Sätze, die mit VTrans übersetzt werden:
Subjekt PrädikatohneAufforderung, das kein Objekt verlangt.
oder Subjekt PrädikatohneAufforderung, das ein Objekt verlangt [1. Objekt].
oder Subjekt PrädikatohneAufforderung, das zwei Objekt verlangt [1. Objekt] 
[2. Objekt].
oder PrädikatmitAufforderung, das kein Objekt verlangt.
oder PrädikatmitAufforderung, das ein Objekt verlangt [1. Objekt].
oder PrädikatmitAufforderung, das zwei Objekt verlangt [1. Objekt] 

Subjekt::=AufzählungselementSubjekt ||  AufzählungselementSubjekt and 
AufzählungselementSubjekt
|| AufzählungselementSubjekt {{, AufzählungselementSubjekt} and 
AufzählungselementSubjekt}

AufzählungselementSubjekt::=the {[Adverb] Adjektiv} Substantiv || a && Substantiv,
das nach der Aussprache nicht mit einem Vokal beginnt || a && Adjektiv,
das nach der Aussprache nicht mit einem Vokal beginnt Substantiv || a && Adjektiv
das nach der Aussprache nicht mit einem Vokal beginnt {Adjektiv} Substantiv ||
a && Adverb,
das nach der Aussprache nicht mit einem Vokal beginnt && Adjektiv {[Adverb] 
Adjektiv} Substantiv || Gerundium || Personalpronom

Personalpronom::= I you he she it we they

Gerundium::=Partizip fortschreitende Gegenwart [Objekt] [Umstandsbestimmung
des Ortes]

Objekt::=AufzählungselementObjekt ||  AufzählungselementObjekt and 
AufzählungselementObjekt || AufzählungselementObjekt 
{{, AufzählungselementObjekt} and AufzählungselementObjekt}

AufzählungselementObjekt::=the {[Adverb] Adjektiv} Substantiv || a && Substantiv,
das nach der Aussprache nicht mit einem Vokal beginnt || a && Adjektiv,
das nach der Aussprache nicht mit einem Vokal beginnt Substantiv || a && Adjektiv
das nach der Aussprache nicht mit einem Vokal beginnt {Adjektiv} Substantiv ||
a && Adverb,
das nach der Aussprache nicht mit einem Vokal beginnt && Adjektiv {[Adverb] 
Adjektiv} Substantiv || Gerundium || Demonstrativpronom

Demonstrativpronom::= me you him her it us them

Umstandsbestimmung des Ortes::=Präposition Aufzählungselement

PrädikatmitAufforderung::=Infinitiv

PrädikatohneAufforderung::= 

========================================================================
       MICROSOFT FOUNDATION CLASS BIBLIOTHEK : VTrans
========================================================================


Diese VTrans-Anwendung hat der Klassen-Assistent für Sie erstellt. Diese Anwendung
zeigt nicht nur die prinzipielle Verwendung der Microsoft Foundation Classes, 
sondern dient auch als Ausgangspunkt für die Erstellung Ihrer eigenen DLLs.

Diese Datei enthält die Zusammenfassung der Bestandteile aller Dateien, die 
Ihre VTrans-Anwendung bilden.

VTrans.dsp
    Diese Datei (Projektdatei) enthält Informationen auf Projektebene und wird zur
    Erstellung eines einzelnen Projekts oder Teilprojekts verwendet. Andere Benutzer können
    die Projektdatei (.dsp) gemeinsam nutzen, sollten aber die Makefiles lokal exportieren.

VTrans.h
    Hierbei handelt es sich um die Haupt-Header-Datei der Anwendung. Diese enthält 
	andere projektspezifische Header (einschließlich Resource.h) und deklariert die
	Anwendungsklasse CVTransApp.

VTrans.cpp
    Hierbei handelt es sich um die Haupt-Quellcodedatei der Anwendung. Diese enthält die
    Anwendungsklasse CVTransApp.

VTrans.rc
	Hierbei handelt es sich um eine Auflistung aller Ressourcen von Microsoft Windows, die 
	vom Programm verwendet werden. Sie enthält die Symbole, Bitmaps und Cursors, die im 
	Unterverzeichnis RES abgelegt sind. Diese Datei lässt sich direkt in Microsoft
	Visual C++ bearbeiten.

VTrans.clw
    Diese Datei enthält Informationen, die vom Klassen-Assistenten verwendet wird, um bestehende
    Klassen zu bearbeiten oder neue hinzuzufügen.  Der Klassen-Assistent verwendet diese Datei auch,
    um Informationen zu speichern, die zum Erstellen und Bearbeiten von Nachrichtentabellen und
    Dialogdatentabellen benötigt werden und um Prototyp-Member-Funktionen zu erstellen.

res\VTrans.ico
    Dies ist eine Symboldatei, die als Symbol für die Anwendung verwendet wird. Dieses 
	Symbol wird durch die Haupt-Ressourcendatei VTrans.rc eingebunden.

res\VTrans.rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++ bearbeitet wurden.
	In diese Datei werden alle Ressourcen abgelegt, die vom Ressourcen-Editor nicht bearbeitet 
	werden können.



/////////////////////////////////////////////////////////////////////////////

Für das Hauptfenster:

MainFrm.h, MainFrm.cpp
    	Diese Dateien enthalten die Frame-Klasse CMainFrame, die von
    	CMDIFrameWnd abgeleitet wurde und alle MDI-Frame-Merkmale steuert.

res\Toolbar.bmp
    Diese Bitmap-Datei wird zum Erstellen unterteilter Bilder für die Symbolleiste verwendet.
    Die erste Symbol- und Statusleiste wird in der Klasse CMainFrame erstellt.
    Bearbeiten Sie diese Bitmap der Symbolleiste mit dem Ressourcen-Editor, und
    aktualisieren Sie IDR_MAINFRAME TOOLBAR in VTrans.rc, um Schaltflächen für die
    Symbolleiste hinzuzufügen.
/////////////////////////////////////////////////////////////////////////////

Für das untergeordnete Rahmenfenster:

ChildFrm.h, ChildFrm.cpp
    Diese Dateien definieren und implementieren die Klasse CChildFrame, die
    die untergeordneten Fenster in einer MDI-Anwendung unterstützt.

/////////////////////////////////////////////////////////////////////////////

Der Klassen-Assistent erstellt einen Dokumenttyp und eine Ansicht(View):

VTransDoc.h, VTransDoc.cpp - das Dokument
    	Diese Dateien enthalten die Klasse CVTransDoc. Bearbeiten Sie diese Dateien,
  	um Ihre speziellen Dokumentdaten hinzuzufügen und das Speichern und Laden von 
	Dateien zu implementieren (mit Hilfe von CVTransDoc::Serialize).

VTransView.h, VTransView.cpp - die Ansicht des Dokuments
    	Diese Dateien enthalten die Klasse CVTransView.
    	CVTransView-Objekte werden verwendet, um CVTransDoc-Objekte anzuzeigen.

res\VTransDoc.ico
    	Dies ist eine Symboldatei, die als Symbol für untergeordnete MDI-Fenster 
	der Klasse CVTransDoc verwendet wird. Dieses Symbol wird durch die
	Haupt-Ressourcendatei VTrans.rc eingebunden.

/////////////////////////////////////////////////////////////////////////////

Hilfe-Unterstützung:

VTrans.hpj
    	Diese Datei ist die Hilfe-Projektdatei, die der Hilfe-Compiler für das 
	Erstellen der Hilfedatei Ihrer Anwendung verwendet.

hlp\*.bmp
    	Hierbei handelt es sich um Bitmap-Dateien, die für die Hilfethemen der 
	Standardbefehle der Microsoft Foundation Classes-Bibliothek erforderlich
	sind.

hlp\*.rtf
    	Diese Datei enthält die Hilfethemen für Standard-MFC-Befehle und 
    	Oberflächenobjekte.

/////////////////////////////////////////////////////////////////////////////
Andere Standarddateien:

StdAfx.h, StdAfx.cpp
    	Mit diesen Dateien werden vorkompilierte Header-Dateien (PCH) mit der Bezeichnung 
	VTrans.pch und eine vorkompilierte Typdatei mit der Bezeichnung StdAfx.obj
	erstellt.

Resource.h
    	Dies ist die Standard-Header-Datei, die neue Ressourcen-IDs definiert.
    	Microsoft Visual C++ liest und aktualisiert diese Datei.

/////////////////////////////////////////////////////////////////////////////
Weitere Hinweise:

Der Klassen-Assistent fügt "ZU ERLEDIGEN:" im Quellcode ein, um die Stellen anzuzeigen, 
an denen Sie Erweiterungen oder Anpassungen vornehmen können.

Wenn Ihre Anwendung die MFC in einer gemeinsam genutzten DLL verwendet und Ihre Anwendung
eine andere als die aktuell auf dem Betriebssystem eingestellte Sprache verwendet, muss 
die entsprechend lokalisierte Ressource MFC42XXX.DLL von der Microsoft Visual C++ CD-ROM 
in das Verzeichnis system oder system32 kopiert und in MFCLOC.DLL umbenannt werden ("XXX" 
steht für die Abkürzung der Sprache. So enthält beispielsweise MFC42DEU.DLL die ins Deutsche 
übersetzten Ressourcen). Anderenfalls erscheinen einige Oberflächenelemente Ihrer Anwendung 
in der Sprache des Betriebssystems.

/////////////////////////////////////////////////////////////////////////////

