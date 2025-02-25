#include "utils.h"
#include "globals.h"

#include <cmath>
#include <cstring>
#include <fileioc.h>
#include <tice.h>

static int pt = 0;
static int ct = 0;
static int nt = -2;

extern unsigned int parseLine;
extern unsigned int parseCol;

char *formatNum(float num) {
    static char buf[20];
    float absNum;
    int exp;
    bool needExp = (globals.normalSciEngMode != NORMAL_MODE || num > 1e10 || num < -1e10 || (num > -1e-3 && num < 1e-3));

    // If necessary, get the right exponent and make sure the number is properly set
    if (needExp) {
        absNum = fabsf(num);
        exp = (int)log10f(absNum);

        if (globals.normalSciEngMode == ENG_MODE) {
            if (exp < 0) exp -= 3;
            exp = exp / 3 * 3;
        }

        num /= powf(10, (float)exp);
    }

    // Do format the number
    if (globals.fixNr == 255) {
        sprintf(buf, "%f", num);
    } else {
        sprintf(buf, "%10.*f", globals.fixNr, num);
    }

    // Replace minus character
    if (*buf == '-') *buf = 0x0B;

    // Strip trailing zeroes
    char *p = buf + strlen(buf) - 1;
    while (*p == '0' && --p > buf);

    // Eventually strip decimal point too
    if (*p == '.') p--;
    *(p + 1) = '\0';

    // Add |E<exp> to the string!
    if (needExp) {
        *(p + 1) = 0x1C;
        sprintf(p + 2, "%d", exp);
        if (exp < 0) *(p + 2) = 0x0B;
    }

    return buf;
}

bool is2ByteTok(int token) {
    uint8_t All2ByteTokens[9] = {tExtTok, tVarMat, tVarLst, tVarPict, tVarGDB, tVarOut, tVarSys, tVarStrng, t2ByteTok};

    return memchr(All2ByteTokens, token, sizeof(All2ByteTokens)) != nullptr;
}

void seekPrev(ti_var_t slot) {
    if (nt != EOF) {
        ti_Seek(-1, SEEK_CUR, slot);
    }

    nt = ct;
    ct = pt;
    pt = 0;
    parseCol--;
}

int tokenNext(ti_var_t slot) {
    if (nt == -2) {
        nt = ti_GetC(slot);
    }

    pt = ct;
    ct = nt;
    nt = ti_GetC(slot);
    parseCol++;

    return ct;
}

int tokenCurrent() {
    return ct;
}

int tokenPeek() {
    return nt;
}

bool endOfLine(int token) {
    return token == EOF || (uint8_t) token == tEnter || (uint8_t) token == tColon;
}
