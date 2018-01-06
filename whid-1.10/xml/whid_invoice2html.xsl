<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- Stylesheet provided by Jarle (jgaa) Aase -->
 	<xsl:output method="html" encoding="utf-8"/>
 	
 	<!-- Language dependent strings -->
 	<xsl:param name="lng_report_summary">Report summary</xsl:param>
	<xsl:param name="lng_total_hours">Total hours</xsl:param>
	<xsl:param name="lng_total">Total</xsl:param>
	<xsl:param name="lng_invoice">Invoice #</xsl:param>
	<xsl:param name="lng_project">Project</xsl:param>
	<xsl:param name="lng_from">From</xsl:param>
	<xsl:param name="lng_to">To</xsl:param>
	<xsl:param name="lng_hours">Hours</xsl:param>
	<xsl:param name="lng_rate">Rate</xsl:param>
	<xsl:param name="lng_sum">Sum</xsl:param>
	<xsl:param name="lng_discount">Discount</xsl:param>
	<xsl:param name="lng_task">Taks</xsl:param>
	
 	<xsl:template match="whid_invoices">
 		<head>
 		<body>
 		<style type="text/css">
 		body { font-family: Sans-Serif; font-size: 10pt;}
		.hdr { color: black; background-color: silver; text-align: left; font-family: Sans-Serif;}
		table { background-color: silver; font-size: 10pt; font-family: Sans-Serif;}
		td { white-space: color: black; text-align: left; background-color: white; }
		.digit { text-align: right; }
		.summary_digit { text-align: right; font-weight: bold;}
		th { background-color: aliceblue; }
		.sep { background-color: white; height=50px; background-color: white;}
		.header {  font-weight: bold; }
		.task { margin-top: 10px; font-weight: bold; }
		.tasks { margin-left: 40px; }
		.project {margin-left: 40px; }
		.project .name { margin-top: 40px; margin-bottom: 20px; font-size: 14pt; font-weight: bold;}
		.summary { margin-top: 20px; font-weight: bold;}
		.invoice { page-break-before: always; }
		.report_postfix { page-break-before: always; }
		.workdescr {padding-top: 10px;}
		.custname {font-size: 14pt; font-weight: bold;}
		.invoice_tag {font-size: 12pt; font-weight: bold; align:}
		</style>
 	
 		<xsl:apply-templates select="invoice"/>
 		
 		<div class="report_postfix">
 			<div class="summary">
 				<h2><xsl:value-of select="$lng_report_summary" /></h2>
 				<div class="totalhours"><xsl:value-of select="$lng_total_hours" />: <xsl:value-of select="summary/@hours"/></div>
 				<div class="totalcharge"><xsl:value-of select="$lng_total" />: <xsl:value-of select="summary/@charge"/></div>
 			</div>
 		</div>
 	
 		</body>
 		</head>
 	</xsl:template>	
 	
 	<xsl:template match="invoice">
 		<div class="invoice">
 			<div class="invoice_tag"><xsl:value-of select="$lng_invoice"/> <xsl:value-of select="@id"/></div>
 			<div class="custname"><xsl:value-of select="customer/@name"/></div>
 			<div class="invdate"><xsl:value-of select="info/@date"/></div>
 	
 			<xsl:apply-templates select="projects/project"/>
 		
 			<div class="summary">
 				<div class="totalhours"><xsl:value-of select="$lng_total_hours" />: <xsl:value-of select="totals/@hours"/></div>
	 			<div class="totalcharge"><xsl:value-of select="$lng_total" />: <xsl:value-of select="totals/@charge"/></div>
 			</div>
 			<hr></hr>
 		</div>
 	</xsl:template>	
 	
 	<xsl:template match="project">
 		<div class="project">
 			<div class="name"><xsl:value-of select="$lng_project" />:  <xsl:value-of select="info/@name"/></div>
 			
 			<div class="tasks">
 				<xsl:apply-templates select="task"/>
 			
 			</div>
 			
 			<div class="summary">
 				<div class="totalhours"><xsl:value-of select="$lng_total_hours" />: <xsl:value-of select="summary/@hours"/></div>
 				<div class="totalcharge"><xsl:value-of select="$lng_total" />: <xsl:value-of select="summary/@charge"/></div>
 			</div>
 			
 		</div>
 		
 	</xsl:template>	
 	
 	<xsl:template match="task">
 		<div class="task"><xsl:value-of select="$lng_task" />: <xsl:value-of select="info/@name"/></div>
 		<table width="90%" border="0">
 			<tr>
 				<th width="20%"><xsl:value-of select="$lng_from" /></th>
 				<th width="20%"><xsl:value-of select="$lng_to" /></th>
 				<th width="15%"><xsl:value-of select="$lng_hours" /></th>
 				<th width="15%"><xsl:value-of select="$lng_rate" /></th>
 				<th width="15%"><xsl:value-of select="$lng_sum" /></th>
 				<th width="15%"><xsl:value-of select="$lng_discount" /></th>
 			</tr>
 			<xsl:apply-templates select="work"/>
 			
 			<tr>
 			<td></td>
 			<td></td>
 			<td class="summary_digit"><xsl:value-of select="summary/@hours"/></td>
 			<td></td>
 			<td class="summary_digit"><xsl:value-of select="summary/@charge"/></td>
 			<td></td>
 		</tr>
 		</table>
 	</xsl:template>
 	
 	<xsl:template match="work">	
 		<tr>
 			<td colspan="6" class="workdescr"><xsl:value-of select="descr"/></td>
 		</tr> 		
 		
 		<xsl:apply-templates select="charge"/>
 		
 		
 	</xsl:template>
 	
 	<xsl:template match="charge">	
 		<tr>
 			<td><xsl:value-of select="info/@from"/></td>
 			<td><xsl:value-of select="info/@to"/></td>
 			<td class="digit"><xsl:value-of select="info/@hours"/></td>
 			<td class="digit"><xsl:value-of select="rate/@charge_per_hour"/></td>
 			<td class="digit"><xsl:value-of select="info/@charge"/></td>
 			<td class="digit"><xsl:value-of select="info/@discount"/></td>
 		</tr>
 	</xsl:template>
 	
</xsl:stylesheet>