#include <iostream>
#include <algorithm>
#include <filesystem/FileManager.h>
#include <lsp-tools/utils.h>
#include "CompletionProvider.h"

#include "lsp-tools/definitions.h"
#include "lsp-tools/messaging.h"

#include "./PrefixTools.cpp"
#include "../util.h"

//#include <pcre.h>

void CompletionProvider::registerCapabilities (Init::ServerCapabilities &capabilities) {
    if (!capabilities.completionProvider) capabilities.completionProvider = Init::CompletionOptions {};

    /*
     * \ - command
     * @ - citation
     * ! - magic comment
     * $ - math shift
     * # - reference (planned)
     * & - environment (planned)
     */
    capabilities.completionProvider->triggerCharacters = { "\\", "@", "!", "$", "#", "&" };
}

void addEnvironmentCompletions (CompletionList &completions, File &file, PrefixData &prefix) {
    // TODO: Apply edit to closing delim too

    completions.addSnippet("document", "document", prefix.range);
    completions.addSnippet("align", "align", prefix.range);
    completions.addSnippet("salign", "align*", prefix.range);
    completions.addSnippet("theorem", "theorem", prefix.range);
    completions.addSnippet("proof", "proof", prefix.range);
    completions.addSnippet("figure", "figure", prefix.range);
}

void addShortEnvironmentCompletions (CompletionList &completions, File &file, PrefixData &prefix) {
    completions.addSnippet("#document", "\\\\begin{document}\n\n$0\n\n\\\\end{document}", prefix.range);
    completions.addSnippet("#theorem", "\\\\begin{theorem}\n\t$0\n\\\\end{theorem}", prefix.range);
    completions.addSnippet("#proof", "\\\\begin{proof}\n\t$0\n\\\\end{proof}", prefix.range);
    completions.addSnippet("#salign", "\\\\begin{align*}\n\t$0\n\\\\end{align*}", prefix.range);
    completions.addSnippet("#align", "\\\\begin{align}\n\t$0\n\\\\end{align}", prefix.range);
    completions.addSnippet("#list", "\\\\begin{itemize}\n\t\\\\item $0\n\\\\end{itemize}", prefix.range);
    completions.addSnippet("#itemize", "\\\\begin{itemize}\n\t\\\\item $0\n\\\\end{itemize}", prefix.range);
    completions.addSnippet("#enumerate", "\\\\begin{enumerate}\n\t\\\\item $0\n\\\\end{enumerate}", prefix.range);
    completions.addSnippet("#description", "\\\\begin{description}\n\t\\\\item[$1] $0\n\\\\end{description}", prefix.range);
}

void addCommandCompletions (CompletionList &completions, File &file, PrefixData &prefix) {
    completions.addSnippet("\\begin", "\\\\begin{$1}\n\t$0\n\\\\end{$1}", prefix.range);
    completions.addSnippet("\\section", "\\\\section{$1}\n$0", prefix.range);
    completions.addSnippet("\\subsection", "\\\\subsection{$1}\n$0", prefix.range);
    completions.addSnippet("\\subsubsection", "\\\\subsubsection{$1}\n$0", prefix.range);
    completions.addSnippet("\\paragraph", "\\\\paragraph{$1}\n$0", prefix.range);
    completions.addSnippet("\\subparagraph", "\\\\subparagraph{$1}\n$0", prefix.range);
    completions.addSnippet("\\chapter", "\\\\chapter{$1}\n$0", prefix.range);
    completions.addSnippet("\\part", "\\\\part{$1}\n$0", prefix.range);
    completions.addCommand("\\phi", prefix.range);
}

void addMathShiftCompletions (CompletionList &completions, File &file, PrefixData &prefix) {
    completions.addSnippet("$", "\\\\($1\\\\)$0", prefix.range, "b");
    completions.addSnippet("$$", "\\\\[\n\t$0\n\\\\]", prefix.range, prefix.value.length() > 1 ? "a" : "c"); // TODO: Don't use hack
}

string getRootForFile (File &file) {
    return string { "./main.tex" };
}

void addMagicCommentCompletions (CompletionList &completions, File &file, PrefixData &prefix) {
    std::cerr << "command so far: " << prefix.value << "\n";

    completions.addSnippet("!root", "% !TEX root = " + getRootForFile(file), prefix.range);
    completions.addSnippet("!engine", "% !TEX engine = ${0:pdflatex}", prefix.range);

    std::cerr << "R:" << prefix.range.start.column << ":" << prefix.range.end.column << "\n";
}

CompletionList getLatexCompletionsForFileAndPoint (File &file, Point cursorPosition) {
    PrefixData prefix = getPrefixData(file, cursorPosition);

    CompletionList completions {};
    switch (prefix.type) {
        case PrefixType::None:
            return completions;
//        case PrefixType::Citation:
//            addCitationCompletions(completions, file, prefix);
//            break;
//        case PrefixType::Reference:
//            addReferenceCompletions(completions, file, prefix);
//            break;
//        case PrefixType::Package:
//            addPackageCompletions(completions, file, prefix);
//            break;
        case PrefixType::Env:
            addEnvironmentCompletions(completions, file, prefix);
            break;
        case PrefixType::EnvShort:
            addShortEnvironmentCompletions(completions, file, prefix);
            break;
        case PrefixType::Magic:
            addMagicCommentCompletions(completions, file, prefix);
            break;
        case PrefixType::MathShift:
            addMathShiftCompletions(completions, file, prefix);
            break;
        case PrefixType::TextCommand:
            addCommandCompletions(completions, file, prefix);
            break;
//        case PrefixType::MathCommand:
//            addMathCompletions(completions, file, prefix);
//            break;
        default: {}
    }

    return completions;
}

void CompletionProvider::run (Id &id, optional<Value> &paramsOpt) {
    if (!paramsOpt) {
        sendError(&id, ResponseHandler::ErrorCode::InvalidParams, "Missing params for autocomplete request");
        return;
    }

    Value &params = *paramsOpt;

    Value &textDocumentIdentifier = params["textDocument"];
    string uri = textDocumentIdentifier["uri"].GetString();

    File *file = FileManager::get(uri);
    if (file == nullptr) {
        std::cerr << "!!! Null file !!!\n";
        sendNullResult(id);
        return;
    }

    CompletionList completions;

    switch (file->type) {
        case File::Type::Tex:
            completions = getLatexCompletionsForFileAndPoint(*file, getPointFromJSON(params["position"]));
            break;
        case File::Type::Bib:
            // TODO
        default:
            sendNullResult(id);
            return;
    }

    if (completions.empty()) {
        sendNullResult(id);
    } else {
        INIT_WRITER
        ADD_ID(id);
        writer.Key("result"); completions.reflect(writer);
        SEND_MESSAGE
    }
}

void CompletionList::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("isIncomplete"); writer.Bool(isIncomplete);
    writer.Key("items"); writer.StartArray();
    for (auto &item : items) {
        item.reflect(writer);
    }
    writer.EndArray();
    writer.EndObject();
}

void CompletionList::addSnippet (string &&prefix, string &&body, Range &range, string &&sortText) {
    CompletionItem snippet {};
    snippet.label = prefix;

    snippet.textEdit.newText = body;
    snippet.textEdit.range = range;

    if (!sortText.empty()) {
        snippet.sortText = sortText;
    }

    items.emplace_back(snippet);
}

void CompletionList::addUnicodeChar (string prefix, string body, Range &range) {
    CompletionItem snippet {};
    snippet.label = prefix;

    snippet.textEdit.newText = body;
    snippet.textEdit.range = range;

    snippet.detail = body;

    items.emplace_back(snippet);
}

void CompletionList::addCommand (string prefix, Range &range) {
    CompletionItem snippet {};
    snippet.label = prefix;

    snippet.textEdit.newText = prefix;
    snippet.textEdit.range = range;

    items.emplace_back(snippet);
}

void CompletionList::addEnvironment (string &prefix, string &envName, Range &range) {
    CompletionItem snippet {};
    snippet.label = prefix;

    snippet.textEdit.newText = "\\\\begin{" + envName + "}\n\t$0\n\\\\end{" + envName + "}";
    snippet.textEdit.range = range;

    items.emplace_back(snippet);
}

bool CompletionList::empty () {
    return items.empty();
}

void CompletionItem::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("label"); writer.String(label);
    writer.Key("kind"); writer.Int((int) kind);
    if (!detail.empty()) {
        writer.Key("detail"); writer.String(detail);
    }
    if (!documentation.empty()) {
        writer.Key("documentation"); writer.String(documentation); // TODO: Support MarkupContent
    }
    if (deprecated) {
        writer.Key("deprecated"); writer.Bool(deprecated);
    }
    if (!sortText.empty()) {
        writer.Key("sortText"); writer.String(sortText);
    }
    writer.Key("insertTextFormat"); writer.Int((int) format);
    writer.Key("textEdit"); textEdit.reflect(writer);
    writer.EndObject();
}

void TextEdit::reflect (StringWriter &writer) {
    writer.StartObject();
    writer.Key("range"); ADD_RANGE(range);
    writer.Key("newText"); writer.String(newText);
    writer.EndObject();
}
