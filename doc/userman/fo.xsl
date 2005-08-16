<?xml version="1.0" encoding="utf-8"?>

<!-- XSL stylesheet containing additions to the standard DocBook
     XSL-FO stylesheet.
-->

<xsl:stylesheet
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
		xmlns:fo="http://www.w3.org/1999/XSL/Format"
		version="1.0">
	
	<!-- Import the standard DocBook stylesheet that this one is based on.
	     We use a web URL, but the local XML catalog should resolve this to
			 the local copy of the stylesheet, if it exists. -->
	<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl"/>

	<!-- Bring in local changes common to both HTML and FO output -->
	<xsl:include href="common.xsl"/>

	<!-- Add page breaks before each sect1, and define a processing
	     instruction that will let us add additional hard breaks when
			 needed.  From "DocBook XSL: The Complete Guide" 3/e -->
	<xsl:attribute-set name="section.level1.properties">
		<xsl:attribute name="break-before">page</xsl:attribute>
	</xsl:attribute-set>
	<xsl:template match="processing-instruction('hard-pagebreak')">
		<fo:block break-before='page'/>
	</xsl:template>

	<!-- Suppress ulinks from the DocBook from making hyperlinks in the
	  PDF, because most of them are relative links to the refman, and
	  these don't work correctly within a PDF.  If you want cross-links,
	  use the HTML manuals.  This code is from Bob Stayton, on the
	  docbook-apps mailing list. -->
	<xsl:template match="ulink" name="ulink">
		<xsl:choose>
			<xsl:when test="count(child::node())=0">
				<xsl:call-template name="hyphenate-url">
					<xsl:with-param name="url" select="@url"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates/>
			</xsl:otherwise>
		</xsl:choose> 
	</xsl:template>
</xsl:stylesheet>
