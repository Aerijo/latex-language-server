#ifndef LANGUAGESERVER_TOOLS_LIBRARY_H
#define LANGUAGESERVER_TOOLS_LIBRARY_H

#include <string>
#include <map>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "definitions.h"
#include "position.h"

using namespace std;
using namespace rapidjson;

typedef string DocumentUri;
typedef string MarkupKind;
typedef int SymbolKind;
typedef int CompletionItemKind;
typedef string CodeActionKind;

Document getMessage ();

#endif
