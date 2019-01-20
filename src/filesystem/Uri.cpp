#include <iostream>
#include <assert.h>

#include "Uri.h"

#ifdef _WIN32
#define IS_WINDOWS true
#else
#define IS_WINDOWS false
#endif

bool validUriScheme (const string &scheme) {
    if (scheme.empty()) return false;

    auto itr = scheme.begin();
    char c = *itr;

    if (!(std::isalnum(c) || c == '_')) return false;

    for (; itr != scheme.end(); itr++) {
        c = *itr;
        if (std::isalnum(c) || c == '_' || c == '+' || c == '-' || c == '.') continue;
        return false;
    }

    return true;
}

bool Uri::validateUri (Uri &uri) {
    if (!uri.scheme.empty() && !validUriScheme(uri.scheme)) {
        std::cerr << "[UriError]: Scheme contains illegal characters\n";
        return false;
    }

    if (uri.path.empty()) return true;

    if (uri.authority.empty()) {
        if (uri.path[0] != '/') {
            std::cerr << "[UriError]: If a URI contains an authority component, then the path "
                         "component must either be empty or begin with a slash (\"/\") character\n";
            return false;
        }
    } else {
        if (uri.path.length() >= 2 && uri.path[0] == '/' && uri.path[1] == '/') {
            std::cerr << "[UriError]: If a URI does not contain an authority component, then the "
                         "path cannot begin with two slash characters (\"//\")\n";
            return false;
        }
    }

    return true;
}

string Uri::referenceResolution (const string &scheme, const string &path) {
    if (scheme == "https" || scheme == "http" || scheme == "file") {
        if (path.empty()) {
            return "/";
        }

        if (path[0] != '/') {
            return "/" + path;
        }
    }

    return path;
}

const string Uri::getRaw () {
    return raw;
}

const string Uri::getScheme () {
    return scheme;
}

const string Uri::getAuthority () {
    return authority;
}

const string Uri::getPath () {
    return path;
}

const string Uri::getQuery () {
    return query;
}

const string Uri::getFragment () {
    return fragment;
}

string Uri::fsPath () {
    return Uri::makeFsPath(*this);
}

void forwardToBackslashes (string &value) {
    auto end = value.end();
    for (auto itr = value.begin() + 1; itr != end; itr++) {
        if (*itr == '/') *itr = '\\';
    }
}

string Uri::makeFsPath (const Uri &uri) {
    string value {};

    if (!uri.authority.empty() && uri.path.length() > 1 && uri.scheme == "file") {
        value = "//" + uri.authority + uri.path;
    } else if (
            uri.path.length() >= 2
            && uri.path[0] == CharCode::Slash
            && std::isalpha(uri.path[1])
            && uri.path[2] == CharCode::Colon
            ) {
        value = static_cast<char>(std::tolower(uri.path[1])) + uri.path.substr(2);
    } else {
        value = uri.path;
    }

    if (IS_WINDOWS) {
        forwardToBackslashes(value);
    }

    return value;
}

// implemented as per vscode regular expression
Uri Uri::parse (const string &value) {
    string scheme {};
    string authority {};
    string path {};
    string query {};
    string fragment {};

    // nothing to be found in empty value
    if (value.empty()) return Uri { scheme, authority, path, query, fragment };

    string temp {};

    auto itr = value.begin();
    auto end = value.end();

    // scheme must have at least one character not of :/?#
    switch (*itr) {
        case '/':
            // possibly an authority
            goto authority;
        case ':':
            // authority starts with //, so only possible segment is path
            goto path;
        case '?':
            // like above, only possible is start of query
            goto query;
        case '#':
            // hash is only valid in the fragment
            goto fragment;
        default:
            temp += *itr; // otherwise start to build the segment
    }

    itr++;

    // We have started building a segment, so a scheme is possible.
    // However, we if we see a /?# before : then we need to reinterpret what we have been building
    // If we reach the end seeing none of these, then the whole thing is a path segment
    for (; itr != end; itr++) {
        switch (*itr) {
            case ':':
                // scheme is present
                scheme = temp;
                temp.clear();
                itr++;

                if (itr == end) {
                    // if we only find the scheme, that's still a valid URI
                    return Uri { scheme, authority, path, query, fragment };
                }

                switch (*itr) {
                    case '/':
                        goto authority;
                    case '?':
                        goto query;
                    case '#':
                        goto fragment;
                    default:
                        goto path;
                }
            case '/':
                // cannot jump from scheme to authority without colon; must actually be a path
            case '?':
                // handle dirty start of query
            case '#':
                // handle dirty start of fragment
            default:
                temp += *itr;
        }
    }

    // if we reach the end, temp is holding the constructed value which is a path
    return Uri { scheme, authority, temp, query, fragment };

    authority:
    // we get here if itr is on the first /
    assert(*itr == '/');

    itr++;

    if (itr == end) {
        path = "/";
        return Uri { scheme, authority, path, query, fragment };
    }

    if (*itr != '/') {
        path = "/";
        switch (*itr) {
            case '?':
                // it was a path followed by a query
                goto query;
            case '#':
                // it was a path followed by a fragment
                goto fragment;
            default:
                // it's a path
                goto path;
        }
    }

    itr++;

    // we are guaranteed an authority; we just need to find the end

    for (; itr != end; itr++) {
        switch (*itr) {
            case '/':
                authority = temp;
                temp = "/"; // the character is actually part of the path (unlike query and fragment below)
                itr++;
                goto path;
            case '?':
                authority = temp;
                temp.clear();
                itr++;
                goto query;
            case '#':
                authority = temp;
                temp.clear();
                itr++;
                goto fragment;
            default:
                temp += *itr;
        }
    }

    // if we reach the end while building an authority, it's still a valid authority (even if it was empty too)
    // (this is techically a very long authority :)
    return Uri { scheme, temp, path, query, fragment };

    path:
    for (; itr != end; itr++) {
        switch (*itr) {
            case '?':
                path = temp;
                temp.clear();
                itr++;
                goto query;
            case '#':
                path = temp;
                temp.clear();
                itr++;
                goto fragment;
            default:
                temp += *itr;
        }
    }

    // refreshingly simple; path can be almost anything
    return Uri { scheme, authority, temp, query, fragment };

    query:
    for (; itr != end; itr++) {
        switch (*itr) {
            case '#':
                query = temp;
                temp.clear();
                itr++;
                goto fragment;
            default:
                temp += *itr;
        }
    }

    return Uri { scheme, authority, path, temp, fragment };

    fragment:
    for (; itr != end; itr++) {
        temp += *itr;
    }

    return Uri { scheme, authority, path, query, temp };
}

void Uri::print () const {
    std::cout
            << "[URI]\n\t- " << scheme
            << "\n\t- " << authority
            << "\n\t- " << path
            << "\n\t- " << query
            << "\n\t- " << fragment << std::endl;
}

Uri Uri::file (string path) {
    string scheme = "file";
    string empty {};
    string authority {};

    if (IS_WINDOWS) {
        forwardToBackslashes(path);
    }

    if (path.length() >= 2 && path[0] == '/' && path[1] == '/') {
        auto end = path.end();
        for (auto itr = path.begin() + 2; itr != end; itr++) {
            if (*itr == '/') {
                authority = string(path.begin() + 2, itr);
                path = string(itr, path.end());
                if (path.empty()) path = "/";

                return Uri { scheme, authority, path, empty, empty };
            }
        }

        authority = string(path.begin() + 2, path.end());
        path = "/";

        return Uri { scheme, authority, path, empty, empty };
    }

    return Uri { scheme, authority, path, empty, empty };
}

bool Uri::operator == (Uri &that) {
    return scheme == that.scheme
           && authority == that.authority
           && path == that.path
           && query == that.query
           && fragment == that.fragment;
}
