#ifndef CSTYLE_H
#define CSTYLE_H


#include <iostream>
#include <vector>
#include <string>

class cStyle
{
    public:
    //state of buffer during working
    enum state{
        original, rmd_Indent,
        add_Indent, no_state};

    // private members
    private:
    // buffer
    std::string orgBuf; // original file buffer
    std::string ridBuf; // remove indentation
    std::string aidBuf; // add indentation

    // characters for input during style
    char newLine = '\n';
    char tab = '\t';

    // members for calculate to add indentation
    int parenNum = -1;
    int braceNum = 0;

    // list for char which ignore
    std::vector<char> igChar = {'#', '"', '/', '\''};
    // list for char which requires for indent
    std::vector<char> inChar = {'{', '}', '(', ':', ';'};

    // tracing members
    char currIgnoreChar = '@'; // set current char which want to be ignore
    int crInd = 0; // current index position
    state curInState = no_state; // current input state of buffer
    state curOutState = no_state; // current output state of buffer

    // private methods
    void removeIndent(); // remove indentation and set ridBuff
    void addIndent(); // add indentation and set aidBuf
    void doProcess(); // do some above stuff
    void push (std::string &, std::string &,int n=1);
    bool isKeychar(const char &c); // check if given char from list
    void ignoreChar(); // ignore till delimiter
    void setCurState(std::string &, state);
    bool isIndentChar(const char&c); // check indet list
    void addLine(std::string &str); // add new line char
    void addTab(std::string &str); // add tab char
    void endParen(); // check ending parenthes conditions
    void semicolon(); // check conditions after semicolon
    void closeBrace(); // check conditions of close brace
    void colon(); // check conditions of colon
    void afterComment(std::string &);
    void openParen();
    // public methods
    public:
    cStyle(const std::string &str); // string
    std::string save(cStyle::state s = cStyle::add_Indent);
};

#endif // CSTYLE_H
