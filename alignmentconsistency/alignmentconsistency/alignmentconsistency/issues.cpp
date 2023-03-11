// alignmentconsistency.cpp : Defines the initialization routines for the DLL.
//

#include "issues.h"

#include <stdio.h>
#include <string.h>

int_t	myIssueCnt = 0;
char	** myIssues = new char*[10000];
int		* myIssuesGroup = new int[10000];
int		* myIssuesSubGroup = new int[10000];
double	* myIssuesDeviation = new double[10000];


void	AddIssue__internal(
				int		group,
				int		subGroup,
				char	* issueText,
				double	deviation
			)
{
	myIssuesDeviation[myIssueCnt] = deviation;
	myIssuesGroup[myIssueCnt] = group;
	myIssuesSubGroup[myIssueCnt] = subGroup;
	myIssues[myIssueCnt] = issueText;
	myIssueCnt++;
}

void	AddIssue__internal(
				int		group,
				int		subGroup,
				char	* issueText
			)
{
	AddIssue__internal(
			group,
			subGroup,
			issueText,
			1.0
		);
}

char	* CreateInstanceText(
				int_t	ifcInstance
			)
{
	if (ifcInstance) {
		char	* entityName = nullptr;
		engiGetEntityName(sdaiGetInstanceType(ifcInstance), sdaiSTRING, (const char**) &entityName);

		char	* txt = new char[256];
		txt[0] = '#';
#ifndef __EMSCRIPTEN__
		_itoa_s((int) internalGetP21Line(ifcInstance), &txt[1], 255, 10);
#else
		sprintf(&txt[1], (char*) "%d", (int) internalGetP21Line(ifcInstance));
#endif
		size_t i = strlen(txt);
		txt[i++] = ' ';
		txt[i++] = '=';
		txt[i++] = ' ';

		memcpy(&txt[i], entityName, strlen(entityName) + 1);
		i = strlen(txt);
		txt[i++] = '(';
		txt[i++] = ' ';
		txt[i++] = '.';
		txt[i++] = '.';
		txt[i++] = ' ';
		txt[i++] = ')';
		txt[i] = 0;

		return	txt;
	}
	else {
		char	* txt = new char[4];
		txt[0] = '?';
		txt[1] = '?';
		txt[2] = '?';
		txt[3] = 0;
		return	txt;
	}
}

char	* MergeText(
				char	* txtI,
				char	* txtII
			)
{
	char	* txt = new char[strlen(txtI) + strlen(txtII) + 1];
	memcpy(txt, txtI, strlen(txtI));
	memcpy(&txt[strlen(txtI)], txtII, strlen(txtII) + 1);
	return	txt;
}

char	* MergeText(
				char	* txtI,
				char	* txtII,
				char	* txtIII
			)
{
	return	MergeText(MergeText(txtI, txtII), txtIII);
}

char	* MergeText(
				char	* txtI,
				char	* txtII,
				char	* txtIII,
				char	* txtIV
			)
{
	return	MergeText(MergeText(txtI, txtII, txtIII), txtIV);
}

char	* MergeText(
				char	* txtI,
				char	* txtII,
				char	* txtIII,
				char	* txtIV,
				char	* txtV
			)
{
	return	MergeText(MergeText(txtI, txtII, txtIII, txtIV), txtV);
}

char	* MergeText(
				char	* txtI,
				char	* txtII,
				char	* txtIII,
				char	* txtIV,
				char	* txtV,
				char	* txtVI
			)
{
	return	MergeText(MergeText(txtI, txtII, txtIII, txtIV, txtV), txtVI);
}

char	* MergeText(
				char	* txtI,
				char	* txtII,
				char	* txtIII,
				char	* txtIV,
				char	* txtV,
				char	* txtVI,
				char	* txtVII
			)
{
	return	MergeText(MergeText(txtI, txtII, txtIII, txtIV), MergeText(txtV, txtVI, txtVII));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				char	* issueTextADD
			)
{
	AddIssue__internal(group, subGroup, MergeText(issueText, (char*) "'", issueTextADD, (char*) "'"));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText
			)
{
	AddIssue__internal(group, subGroup, issueText);
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstance
			)
{
	char	* ifcInstanceText = CreateInstanceText(ifcInstance);
	
	AddIssue__internal(group, subGroup, MergeText(issueText, (char*) " - ", ifcInstanceText));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstanceI,
				int_t	ifcInstanceII
			)
{
	char	* ifcInstanceTextI = CreateInstanceText(ifcInstanceI),
			* ifcInstanceTextII = CreateInstanceText(ifcInstanceII);
	
	AddIssue__internal(group, subGroup, MergeText(issueText, (char*) " - ", ifcInstanceTextI, (char*) ", ", ifcInstanceTextII));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstance,
				char	* itemText
			)
{
	char	* ifcInstanceText = CreateInstanceText(ifcInstance);
	
	AddIssue__internal(group, subGroup, MergeText(issueText, itemText, (char*) " - ", ifcInstanceText));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstance,
				char	* itemText,
				double	givenValue,
				double	expectedValue
			)
{
	char	* ifcInstanceText = CreateInstanceText(ifcInstance);

	char	g_buffer[256], * g_cp = &g_buffer[0];
#ifndef __EMSCRIPTEN__
	sprintf_s(g_cp, 256, (char*) "%.16f", givenValue);
#else
	sprintf(g_cp, (char*) "%.16f", givenValue);
#endif

	char	e_buffer[256], * e_cp = &e_buffer[0];
#ifndef __EMSCRIPTEN__
	sprintf_s(e_cp, 256, (char*) "%.16f", expectedValue);
#else
	sprintf(e_cp, (char*) "%.16f", expectedValue);
#endif
	AddIssue__internal(group, subGroup, MergeText(issueText, MergeText(itemText, " given value ", g_cp, ", expected value ", e_cp), (char*)" - ", ifcInstanceText));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstanceI,
				int_t	ifcInstanceII,
				char	* itemText
			)
{
	char	* ifcInstanceTextI = CreateInstanceText(ifcInstanceI),
			* ifcInstanceTextII = CreateInstanceText(ifcInstanceII);
	
	AddIssue__internal(group, subGroup, MergeText(issueText, itemText, (char*) " - ", ifcInstanceTextI, (char*) ", ", ifcInstanceTextII));
}

void	AddIssue(
				int		group,
				int		subGroup,
				char	* issueText,
				int_t	ifcInstanceI,
				int_t	ifcInstanceII,
				double	value
			)
{
	char	* ifcInstanceTextI = CreateInstanceText(ifcInstanceI),
			* ifcInstanceTextII = CreateInstanceText(ifcInstanceII);
	
	char	buffer[256], *cp = &buffer[0];
#ifndef __EMSCRIPTEN__
	sprintf_s(cp, 256, (char*) "%.16f", value);
#else
	sprintf(cp, (char*) "%.16f", value);
#endif

	AddIssue__internal(group, subGroup, MergeText(issueText, (char*) " *** ", buffer, (char*) " *** ", ifcInstanceTextI, (char*) " - ", ifcInstanceTextII), value);
}
