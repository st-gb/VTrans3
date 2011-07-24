<!-- adapted from http://de.wikibooks.org/wiki/Websiteentwicklung:_XSLT:_Erste_Schritte#Listing:nach_HTML  -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
      <!--<xsl:output method="text" encoding="ISO-8859-1"/>-->
	  <!--from http://www.xml.com/pub/a/2000/08/30/xsltandhtml/index.html:
		important for outputting HTML tags?!-->
	  <xsl:output method="html" encoding="ISO-8859-1" />

      <xsl:strip-space elements="*"/>
	  
	  <!-- match">>XPath<<"; topmost element in XML tree hierarchy-->
      <xsl:template match="/sentences"> 
		  <HTML><HEAD></HEAD><BODY>	
		  <!--<xsl:text>sentences element</xsl:text>-->
		  <xsl:apply-templates/> <!-- means: insert output of applied "xml:template" rules here -->
		  
		  <!--<="mainVerbInf0Obj"">-->
		  </BODY>
		  </HTML>
      </xsl:template>
    
      <xsl:template match="translation">
	  <!--from http://www.w3.org/TR/1999/REC-xslt-19991116#section-Conditional-Processing-with-xsl:if
	  
		/ from http://www.dpawson.co.uk/xsl/sect2/N1777.html
		
	  <xsl:if test="not(position()=last())">, </xsl:if>--> 
		<xsl:if test="@grammar_part_name = 'definite_article' ">
			<!--<![CDATA[ ... ]]>--><font color="FF0000">
		<xsl:value-of select="."/>  <!-- <pre></pre>&nbsp; "." means: string between ">" and "<" -->
			<!--<xsl:text>-->
			<!--</xsl:text	>-->
			<!--<xsl:text>definite_article</xsl:text>-->
			</font>
			<xsl:message> set red text</xsl:message>
		</xsl:if>
		
		
		<xsl:choose>
        <xsl:when test="@grammar_part_name = 'definite_article' ">
		  <font color="FF0000">
			<xsl:value-of select="."/>
		  </font>
        </xsl:when>
        <xsl:when test="@grammar_part_name = 'I' ">
		  <font color="00FF00">
			<xsl:value-of select="."/>
		  </font>
        </xsl:when>		
        <xsl:when test="@grammar_part_name = 'singular_noun' ">
		  <font color="00FF00">
			<xsl:value-of select="."/>
		  </font>
        </xsl:when>
		<xsl:when test="@grammar_part_name = 'adverb' ">
		  <font color="00FF00">
			<xsl:value-of select="."/>
		  </font>
        </xsl:when>
        <xsl:otherwise>
			<xsl:value-of select="."/>
			<xsl:message> xsl:otherwise: </xsl:message>
        </xsl:otherwise>
        </xsl:choose>

		<!--<xsl:value-of select=""/>--> <!--&nbsp;-->
		<!--<xsl:value-of select="."/>-->  <!-- <pre></pre>&nbsp; "." means: string between ">" and "<" -->
		<!--<xsl:message>&nbsp;</xsl:message>-->
		<!--from http://p2p.wrox.com/xslt/37926-blank-space-xslt.html:
		 see also http://www.w3.org/TR/1999/REC-xslt-19991116#section-Creating-Text  -->
		<xsl:text> </xsl:text>
		<!--<xsl:apply-templates/>-->
      </xsl:template>
	  
    </xsl:stylesheet>

<!--from wikibooks:
//    <?xml version="1.0"?>
//    <programm name="MeinErstesProgramm">
//      <schreib>Hallo, Welt!</schreib>
//    </programm>
//
//   Ein einfaches Stylesheet, welches obige XML-Datei so bearbeitet, dass daraus ein richtiges Pascal-Programm wird, bietet folgendes Listing:
//   [Bearbeiten] Listing:nach Pascal
//
//    <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
//      <xsl:output method="text" encoding="ISO-8859-1"/>
//      <xsl:strip-space elements="*"/>
//    
//      <xsl:template match="/programm">
//      program <xsl:value-of select="@name"/>;
//      begin
//      <xsl:apply-templates/>
//      end.
//      </xsl:template>
//    
//      <xsl:template match="schreib">
//        WriteLn ('<xsl:value-of select="."/>');
//      </xsl:template>
//    </xsl:stylesheet>
//
//   Die Ausgabe des XSLT-Prozessors ist wie folgt:
//   [Bearbeiten] Listing:Ausgabe in Pascal
//
//     program MeinErstesProgramm;
//     begin
//    
//       WriteLn ('Hallo, Welt!');
//    
//     end.-->