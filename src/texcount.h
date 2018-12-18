//
// Implementation of TeXCount in C++
//

#ifndef LATEX_LANGUAGE_SERVER_TEXCOUNT_H
#define LATEX_LANGUAGE_SERVER_TEXCOUNT_H

struct WordCountData {
    int plain;
    int math;
    int header;
    int figure;
    int bibliography;
};

struct CountOptions {

};

#endif //LATEX_LANGUAGE_SERVER_TEXCOUNT_H
