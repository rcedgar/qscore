#include "qscore.h"
#include <time.h>

static string g_LogFileName;
static FILE *g_fLog;

void OpenLogFile(const string &FileName)
	{
	if (FileName.empty())
		return;
	g_fLog = fopen(FileName.c_str(), "w");
	if (g_fLog == 0)
		Quit("Error opening log file '%s'", FileName.c_str());
	setbuf(g_fLog, 0);
	}

void CloseLogFile()
	{
	if (g_fLog != 0)
		{
		fclose(g_fLog);
		g_fLog = 0;
		}
	}

void Log(const char *szFormat, ...)
	{
	if (g_fLog == 0)
		return;
	va_list ArgList;
	va_start(ArgList, szFormat);
	vfprintf(g_fLog, szFormat, ArgList);
	va_end(ArgList);
	}

void Quit(const char *szFormat, ...)
	{
	va_list ArgList;
	char szStr[4096];

	fprintf(stderr, "\n***ERROR***\n");
	va_start(ArgList, szFormat);
	vsprintf(szStr, szFormat, ArgList);
	fprintf(stderr, szStr);
	fprintf(stderr, "\n");
	if (0 != g_TestFileName && 0 != g_RefFileName)
		{
		fprintf(stderr, "Test file: %s\n", g_TestFileName);
		fprintf(stderr, "Ref  file: %s\n", g_RefFileName);
		}
	exit(1);
	}

void Warning(const char *szFormat, ...)
	{
	va_list ArgList;
	char szStr[4096];

	fprintf(stderr, "\n***WARNING***\n");
	va_start(ArgList, szFormat);
	vsprintf(szStr, szFormat, ArgList);
	fprintf(stderr, szStr);
	fprintf(stderr, "\n");
	if (0 != g_TestFileName && 0 != g_RefFileName)
		{
		fprintf(stderr, "Test file: %s\n", g_TestFileName);
		fprintf(stderr, "Ref  file: %s\n", g_RefFileName);
		}
	}

FILE *OpenStdioFile(const char *FileName)
	{
	FILE *f = fopen(FileName, "r");
	if (0 == f)
		Quit("Cannot open %s, %s [errno=%d]", FileName, strerror(errno), errno);
	return f;
	}

int GetFileSize(FILE *f)
	{
	long CurrPos = ftell(f);
	if (CurrPos < 0)
		Quit("FileSize: ftell<0 (CurrPos), errno=%d", errno);

	int Ok = fseek(f, 0, SEEK_END);
	if (Ok != 0)
		Quit("FileSize fseek(END) != 0 errno=%d", errno);

	long Size = ftell(f);
	if (Size < 0)
		Quit("FileSize: ftell<0 (size), errno=%d", errno);

	Ok = fseek(f, CurrPos, SEEK_SET);
	if (Ok != 0)
		Quit("FileSize fseek(restore curr pos) != 0 errno=%d", errno);

	long NewPos = ftell(f);
	if (CurrPos < 0)
		Quit("FileSize: ftell=%ld != CurrPos=%ld", CurrPos, NewPos);

	return (int) Size;
	}

void *allocmem(int bytes)
	{
	char *p = (char *) malloc((size_t) (bytes));
	if (0 == p)
		Quit("Out of memory (%d)", bytes);
	return p;
	}

void freemem(void *p)
	{
	if (0 == p)
		return;
	free(((char *) p));
	}
