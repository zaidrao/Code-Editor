#include <iostream>
#include <cStyle.h>
#include <cctype>
#include <cstdlib>

// implimantation file
// protected methods

void cStyle::doProcess()
{
    removeIndent();
    addIndent();
}

void cStyle::push (std::string &in, std::string &out, int n)
{
    for(int i = 0; i<n ; i++)
    {
        in.push_back(out[crInd]);
        crInd++;
    }
}

void cStyle::removeIndent()
{
    // this function remove newline char
    // and tab char and ignore some key char
    // like comment, preprocess directive
    curInState = rmd_Indent;
    curOutState = original;
    crInd =0;
    ridBuf.clear();
    for(int i = 0; crInd < orgBuf.size();i++)
    {
        if(isKeychar(orgBuf[crInd]))
        {
            ignoreChar();
            continue;
        }
        //check for \n\t
        if(orgBuf[crInd] == newLine ||
            orgBuf[crInd] == tab)
        {
            crInd++;
            continue;
        }
        // check for space tab
        int p = orgBuf.find("    ",crInd);
        if(p == crInd)
        {
            crInd = crInd + 4;
            continue;
        }
        //check for single else
        p = orgBuf.find("else",crInd);
        if(p == crInd)
        {
            push(ridBuf,orgBuf,4);
            ridBuf.push_back(' ');
            continue;
        }
        push(ridBuf,orgBuf);
    }
}

bool cStyle::isKeychar(const char &c)
{
    for(int i = 0; i<igChar.size(); i++)
    {
        if(igChar[i] == c)
        {
            currIgnoreChar = c;
            return true;
        }
    }
    return false;
}

void cStyle::setCurState(std::string &str, state st)
{
    switch (st)
    {
        case original:
        str = orgBuf;
        break;
        case rmd_Indent:
        str = ridBuf;
        break;
        case add_Indent:
        str = aidBuf;
        break;
        default:
        std::cout<<"Current input state invalid";
        exit(EXIT_FAILURE);
        break;
    }
}

void cStyle::ignoreChar()
{
    std:: string inputbuf; // this buffer use for input from output bufer
    std:: string outputbuf; // output buffer

    // check current input output state and copy data according to state
    setCurState(inputbuf,curInState);
    setCurState(outputbuf,curOutState);

    //copy data
    switch (currIgnoreChar)
    {
        case '#':
        while (outputbuf[crInd] != '\n'
                && crInd<outputbuf.size())
        {
            push(inputbuf,outputbuf);
        }
            push(inputbuf,outputbuf);
        break;
        case '/':
            // check if next index is also /
        if(outputbuf[crInd+1] == '/')
            {
            while (outputbuf[crInd] != '\n'
                && crInd<outputbuf.size())
                {
                    push(inputbuf,outputbuf);
                }
            push(inputbuf,outputbuf);
            if(curInState == add_Indent)
                afterComment(inputbuf);
            }
            // check if next is /*
            else if(outputbuf[crInd+1] == '*')
            {
                inputbuf.push_back(newLine);
            while (crInd<outputbuf.size())
                {
                    push(inputbuf,outputbuf);
                    if(outputbuf[crInd+1] == '*')
                    {
                        if(outputbuf[crInd+2] ==
                         '/')
                         break;
                    }
                }
            push(inputbuf,outputbuf,3);
            inputbuf.push_back(newLine);
            }
            else
            {
                push(inputbuf,outputbuf);
            }
        break;
        case '"':
            // exception \"
            push(inputbuf,outputbuf);
        while (outputbuf[crInd] != '"'
                && crInd<outputbuf.size())
        {
            push(inputbuf,outputbuf);
        }
            push(inputbuf,outputbuf);
        break;
        case '\'':
            push(inputbuf,outputbuf);
        while (outputbuf[crInd] != '\''
                && crInd<outputbuf.size())
        {
            push(inputbuf,outputbuf);
        }
            push(inputbuf,outputbuf);
        break;
        default:
        while (outputbuf[crInd] != '\n'
                && crInd<outputbuf.size())
        {
            push(inputbuf,outputbuf);
        }
        push(inputbuf,outputbuf);
        break;
    }
    switch (curInState)
    {
        case original:
        orgBuf = inputbuf;
        break;
        case rmd_Indent:
        ridBuf = inputbuf;;
        case add_Indent:
        aidBuf = inputbuf;
        break;
        default:
        std::cout<<"Current input state invalid";
        exit(EXIT_FAILURE);
        break;
    }
}

void cStyle::addIndent()
{
    // it only add newline and tab char
    // according to some rules
    curInState = add_Indent;
    curOutState = rmd_Indent;
    crInd =0;
    aidBuf.clear();
    parenNum = -1;
    braceNum = 0;
    for(int i = 0; crInd < ridBuf.size();i++)
    {
        if(isKeychar(ridBuf[crInd]))
        {
            ignoreChar();
            continue;
        }
        if(isIndentChar(ridBuf[crInd]))
        {
            continue;
        }
        push(aidBuf,ridBuf);
    }
}
bool cStyle::isIndentChar(const char&c)
{
    for(int i = 0; i<inChar.size(); i++)
    {
        if(inChar[i] == c)
        {
            switch (c)
            {
                case '{':
                addLine(aidBuf);
                addTab(aidBuf); // only work when braceNum greater than 0
                braceNum++;
                push(aidBuf,ridBuf);
                addLine(aidBuf);
                addTab(aidBuf);
                return true;
                case '}':
                closeBrace();
                return true;
                case ':':
                colon();
                return true;
                case ';':
                semicolon();
                return true;
                case '(':
                openParen();
                return true;
            }
        }
    }
    return false;
}
void cStyle::addLine(std::string &str)
{
    str.push_back(newLine);
}
void cStyle::addTab(std::string &str)
{
    for(int i = 0; i<braceNum; i++)
    {
        str.push_back(tab);
    }
}
void cStyle::endParen()
{
    if(parenNum == 0) // check if parentheses matching equal to zero
    { // check next characters of closing brace
        while (crInd<ridBuf.size())
        {
            if(ridBuf[crInd+1] == '{')
            {
                push(aidBuf,ridBuf);
                //addTab();
                break;
            }
            else if(ridBuf[crInd+1] == ';')
            {
                push(aidBuf,ridBuf);
                break;
            }
            else if(ridBuf[crInd+1] == '/')
            {
                push(aidBuf,ridBuf);
                  break;
            }
            push(aidBuf,ridBuf);
        }
    }
    else
        push(aidBuf,ridBuf);
}
void cStyle::openParen()
{
    if(parenNum == -1)
        parenNum = 0;
    parenNum++;
    push(aidBuf,ridBuf);
    while (crInd<ridBuf.size())
    {
        if(ridBuf[crInd] == '(')
        {
            parenNum++;
            push(aidBuf,ridBuf);
        }
        else if (ridBuf[crInd] == ')')
        {
            parenNum--;
            if(parenNum == 0)
            {
                endParen();
                break;
            }
            push(aidBuf,ridBuf);
        }
        else
        {
            push(aidBuf,ridBuf);
        }
    }
}
void cStyle::semicolon()
{
    while (crInd<ridBuf.size())
    {
        if(ridBuf[crInd+1] == '{')
            {
                push(aidBuf,ridBuf);
                addLine(aidBuf);
                addTab(aidBuf);
                break;
            }
        else if(ridBuf[crInd+1] == '/')
            {
                push(aidBuf,ridBuf);
                break;
            }
        else if(ridBuf[crInd+1] == '}')
            {
                push(aidBuf,ridBuf); // add colon
                braceNum--;
                addLine(aidBuf);
                addTab(aidBuf);
                braceNum++;
                break;
            }
        else if(isalnum(ridBuf[crInd+1]))
            {
                push(aidBuf,ridBuf);
                addLine(aidBuf);
                addTab(aidBuf);
                break;
            }
        else if(isspace(ridBuf[crInd+1]))
        {
            push(aidBuf,ridBuf);
            continue;
        }
        else if(ispunct(ridBuf[crInd+1]))
        {
            push(aidBuf,ridBuf);
            continue;
        }
    }
}
void cStyle::closeBrace()
{
    braceNum--;
    if(ridBuf[crInd+1] == '}')
    {
        push(aidBuf,ridBuf);
        braceNum--;
        addLine(aidBuf);
        addTab(aidBuf);
        braceNum++;
    }
    else if(ridBuf[crInd+1] == ';')
    {
        push(aidBuf,ridBuf);
    }
    else
    {
        push(aidBuf,ridBuf);
        addLine(aidBuf);
        addTab(aidBuf);
    }
}

void cStyle::colon()
{
    if(ridBuf[crInd+1] == ':')
    {
        push(aidBuf,ridBuf,2);
    }
    else
    {
        push(aidBuf,ridBuf);
        addLine(aidBuf);
        addTab(aidBuf);
    }
}
void cStyle::afterComment(std::string &str)
{
    // first find last \n pos
    int pos;
    pos = str.rfind(newLine,str.size()); // using rfind() because searching from end till \n
    str.erase(str.begin()+pos); // erase a \n otherwise there are two \n
    while (crInd<ridBuf.size())
    {
        if(ridBuf[crInd] == '{')
        {
            addTab(str);
            break;
        }
        else if (ridBuf[crInd] == '}')
        {
            addLine(str);
            braceNum--;
            addTab(str);
            braceNum++;
            break;
        }
        else if (isspace(ridBuf[crInd]))
        {
            push(aidBuf,ridBuf);
            continue;
        }
        else
        {
            addLine(str);
            addTab(str);
            break;
        }
    }
}
//public methods
cStyle::cStyle(const std::string &str)
{
    orgBuf = str;
}
std::string cStyle::save(cStyle::state s)
{
    doProcess();

    switch (s)
    {
        case rmd_Indent:
        return ridBuf;
        case add_Indent:
        return aidBuf;
        default:
        std::cerr<<"Select valid state";
        return "NULL";

    }
}
