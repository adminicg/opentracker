<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:output method="text"/>
<xsl:template match="TrackerServer">
digraph "<xsl:value-of select=".//Network/@name"/>" 
{ 
	<xsl:apply-templates/> 
} 
</xsl:template>

<xsl:template match="configuration" priority="2"/>

<xsl:template name="options">
	<xsl:choose>
		<xsl:when test="name(.)='Station'">
			<xsl:call-template name="Station"/>
		</xsl:when>
		<xsl:when test="name(.)='Marker'">
			<xsl:call-template name="Marker"/>			
		</xsl:when>
		<xsl:when test="name(.)='MaxConfidenceFilter'">
			<xsl:call-template name="MaxConfidenceFilter"/>			
		</xsl:when>
		<xsl:when test="name(.)='CursesOutput'">
			<xsl:call-template name="CursesOutput"/>			
		</xsl:when>
		<xsl:when test="name(.)='NetworkSource'">
			<xsl:call-template name="NetworkSource"/>			
		</xsl:when>

		<xsl:otherwise>[label="<xsl:value-of select="name(.)"/>",shape=box]</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="Station">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@name"/> - <xsl:value-of select="@number"/>"]</xsl:template>

<xsl:template name="Marker">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@tag-file"/>"]</xsl:template>

<xsl:template name="MaxConfidenceFilter">[label="<xsl:value-of select="name(.)"/>",shape=polygon,sides=5]</xsl:template>

<xsl:template name="CursesOutput">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@comment"/>"]</xsl:template>

<xsl:template name="NetworkSource">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>


<xsl:template match="*">
	<xsl:param name="parent"/>
	<xsl:number count="*" format="a" level="any"/><xsl:call-template name="options"/>; 
	<xsl:if test="not(name(..)='TrackerServer')">
		 <xsl:number count="*" format="a" level="any"/> -> <xsl:copy-of select="$parent"/>; 
	</xsl:if>
	<xsl:apply-templates>
		<xsl:with-param name="parent">
			<xsl:number count="*" format="a" level="any"/>
		</xsl:with-param>
	</xsl:apply-templates>
</xsl:template>
</xsl:stylesheet>