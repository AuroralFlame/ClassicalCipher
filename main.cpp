#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include "Matrix.h"

using namespace std;

class ClassicalCipherHandler {
private:
    string text;
    string curAlgorithm;
    std::map<string, string> algorithms;

    string simpleTranspositionEncrypt(int key);

    string simpleTranspositionDecrypt(int key);

    string caesarCipherEncrypt(int key);

    string caesarCipherDecrypt(int key);

    string vigenereEncrypt(string key);

    string vigenereDecrypt(string key);

    string hillEncrypt(Matrix<int> *Z);

    string hillDecrypt(Matrix<int> *Z);

public:
    const string CAESAR_SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890.,";
    const string VIGENERE_SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string HILL_SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    explicit ClassicalCipherHandler(string text);

    bool setCurAlgorithm(const string &algorithm);

    const string &getText() const;

    string decrypt(int key);

    string decrypt(string key);

    string encrypt(int key);

    string encrypt(string key);


    string encrypt(Matrix<int> *key);

    string decrypt(Matrix<int> *key);

    static void inputControl(char &exit);

    void cipherExecutor(char &choice);

    void handleAlgorithm(string &algorithm);


};

ClassicalCipherHandler::ClassicalCipherHandler(string textTo) {
    this->text = textTo;
    algorithms.insert(pair<string, string>("tra", "置换算法"));
    algorithms.insert(pair<string, string>("caesar", "凯撒算法"));
    algorithms.insert(pair<string, string>("vigenere", "维吉尼亚算法"));
    algorithms.insert(pair<string, string>("hill", "希尔密码算法"));
}

/* 获取输入文本 */
const string &ClassicalCipherHandler::getText() const {
    return text;
}

/* 设置当前选择的算法 */
bool ClassicalCipherHandler::setCurAlgorithm(const string &algorithm) {
    bool hasIn = false;

    if (algorithms.find(algorithm) != algorithms.end()) {
        ClassicalCipherHandler::curAlgorithm = algorithm;
        return true;
    } else {
        cout << "错误的算法名称\n";
        return false;
    }
}

/* 简单的置换密码--加密 */
string ClassicalCipherHandler::simpleTranspositionEncrypt(int key) {
    string cipherTmpText;
    auto textLen = text.length();
    auto tmpIndex = 0;
    /* 对于 from 0 to Key-1 的每一列 */
    for (auto column = 0; column < key; column++) {
        tmpIndex = column;
        while (tmpIndex < textLen) {
            cipherTmpText += text[tmpIndex];
            tmpIndex += key;
        }
    }
    //使用符号 "|"跟在密文结尾，防止末位存在空格不易识别
    this->text = cipherTmpText;
    return cipherTmpText + "|";
}

/* 简单的置换密码--解密 */
string ClassicalCipherHandler::simpleTranspositionDecrypt(int key) {
    string plaintTmpText;
    auto textLen = float(this->text.length());
    auto tmpIndex = 0;
    auto rowCount = key, columnCount = int(ceil(textLen / float(key)));
    auto shadeBoxCount = columnCount * rowCount - int(textLen);
    for (int i = 0; i < rowCount * columnCount; ++i) {
        plaintTmpText += " ";
    }
    int columnIn = 0, rowIn = 0;
    for (char symbol : this->text) {
        plaintTmpText[rowIn + columnIn * rowCount] = symbol;
        if ((columnIn == columnCount - 1) || (columnIn == columnCount - 2 && rowIn >= rowCount - shadeBoxCount)) {
            rowIn++;
            columnIn = 0;
        } else {
            columnIn++;
        }
    }
    this->text = plaintTmpText;
    return plaintTmpText;
}


/* 凯撒密码--加密 */
string ClassicalCipherHandler::caesarCipherEncrypt(int key) {
    string cipherTmpText;
    for (char each :text) {
        if (CAESAR_SYMBOLS.find(each) != string::npos) {
            auto symbolIndex = CAESAR_SYMBOLS.find(each);
            auto pos = symbolIndex + key;
            if (pos >= CAESAR_SYMBOLS.length())
                pos -= CAESAR_SYMBOLS.length();
            else if (pos < 0)
                pos += CAESAR_SYMBOLS.length();
            cipherTmpText += CAESAR_SYMBOLS[pos];
        } else
            cipherTmpText += each;
    }
    this->text = cipherTmpText;
    return cipherTmpText;
}

/* 凯撒密码--解密 */
string ClassicalCipherHandler::caesarCipherDecrypt(int key) {
    string plainTmpText;
    for (char each :text) {
        if (CAESAR_SYMBOLS.find(each) != string::npos) {
            auto symbolIndex = CAESAR_SYMBOLS.find(each);
            auto a = symbolIndex - key;
            if (a >= CAESAR_SYMBOLS.length())
                a -= CAESAR_SYMBOLS.length();
            else if (a < 0)
                a += CAESAR_SYMBOLS.length();
            plainTmpText += CAESAR_SYMBOLS[a];
        } else {
            plainTmpText += each;
        }
    }
    this->text = plainTmpText;
    return plainTmpText;
}


/* 维吉尼亚密码加密 */
string ClassicalCipherHandler::vigenereEncrypt(string key) {
    string cipherTmpText;

    auto ring = VIGENERE_SYMBOLS.length();
    auto i = 0;
    /* 将密钥-字符串转化为大写 */
    transform(key.begin(), key.end(), key.begin(), ::toupper);
    for (char each : this->text) {
        auto num = VIGENERE_SYMBOLS.find((char) toupper(each));
        if (num != string::npos) {
            num += VIGENERE_SYMBOLS.find(key[i]);

            /* 处理回环 */
            num %= ring;
            if (isupper(each))
                cipherTmpText += VIGENERE_SYMBOLS[num];
            else if (islower(each))
                cipherTmpText += (char) tolower(VIGENERE_SYMBOLS[num]);
            i++;
            if (i == key.length()) i = 0;
        } else {
            cipherTmpText += each;
        }
    }
    this->text = cipherTmpText;
    return cipherTmpText;
}

/* 维吉尼亚密码解密 */
string ClassicalCipherHandler::vigenereDecrypt(string key) {
    string plaintTmpText;
    const auto ring = VIGENERE_SYMBOLS.length();
    auto keySymbolPos = 0;
    /* 将密钥-字符串转化为大写 */
    transform(key.begin(), key.end(), key.begin(), ::toupper);
    auto posInDic = 0;
    /* 根据密钥对密文的每个字母进行解密 */
    for (char each : this->text) {
        /* 找到当前密文字母在字典中的位置 */
        posInDic = VIGENERE_SYMBOLS.find((char) (toupper(each)));
        if (posInDic != string::npos) {

            //cout <<  "密文字母 对应字典中存在的字符为：" << VIGENERE_SYMBOLS[posInDic] <<" in "<<posInDic << endl;
            /* 找到当前密钥字符在字典中的位置 */
            const auto posInDicKey = VIGENERE_SYMBOLS.find(key[keySymbolPos]);
            //cout << std::boolalpha <<(key[keySymbolPos] == VIGENERE_SYMBOLS[25]) << endl;
            if (posInDicKey != string::npos) {
                //cout <<  "当前密钥字母为：" << key[keySymbolPos]<< endl;
                posInDic -= posInDicKey;
            }
            //cout << "最终明文字符位置(未模运算)" << posInDic << "当前密文字符序号" << keySymbolPos<<endl;

            /* 模加运算的模运算部分：处理回环 :对于C++ 趋0取整，需要对负数与整数模运算调整*/
            posInDic = (posInDic < 0) ? (posInDic + ring) : posInDic;
            posInDic %= ring;
            //cout << "最终明文字符位置" << posInDic<<endl;
            if (isupper(each))
                plaintTmpText += VIGENERE_SYMBOLS[posInDic];
            else if (islower(each))
                plaintTmpText += (char) tolower(VIGENERE_SYMBOLS[posInDic]);

            keySymbolPos++;
            if (keySymbolPos == key.length()) {
                keySymbolPos = 0;
            }
        } else {
            plaintTmpText += each;
        }
    }
    this->text = plaintTmpText;
    return plaintTmpText;
}


/* 希尔密码加密 */
string ClassicalCipherHandler::hillEncrypt(Matrix<int> *Z) {
    string cipherTmpText;
    const auto textLen = text.length(), ring = HILL_SYMBOLS.length();

    int a[100] = {0};
    int partCount = textLen / Z->getN(), factor = 0;
    while (partCount--) {
        for (int i = 0; i < Z->getN(); i++) {
            for (int k = 0; k < Z->getN(); k++) {
                a[i + factor] += int(text[i*factor + k] - 'a') * (Z->org()[i][k]);
            }
            a[i + factor] %= ring;
            cipherTmpText += char(a[i + factor]) + 'a';
        }
        factor++;
        factor = factor * Z->getN();
    }
    return cipherTmpText;
}

int gradeMod(int param) {
    for (int i = 1;;) {
        if ((26 * i + 1) % param == 0) {
            return (26 * i + 1) / param;
            break;
        }
        i += 1;
    }
}

Matrix<int> *multiply(Matrix<int> *m, int param, int ring) {
    for (int i = 0; i < m->getN(); i++) {
        for (int j = 0; j < m->getN(); j++) {
            m->org()[i][j] = ((m->org()[i][j] * param) % ring + ring) % ring;
        }
    }
    return new Matrix<int>(m->org(), m->getN());
}

/* 希尔密码解密 */
string ClassicalCipherHandler::hillDecrypt(Matrix<int> *Z) {
    string plaintTmpText;
    const auto textLen = text.length(), ring = HILL_SYMBOLS.length();
    Matrix<int> *KI = multiply(Z->getAdjoint(), gradeMod(int(Z->getDeterminant()) % ring), ring);
    int a[100] = {0};

    if (KI->getN() <= textLen) {
        int partCount = textLen / KI->getN(), factor = 0;
        while (partCount--) {
            for (int i = 0; i < KI->getN(); i++) {
                for (int k = 0; k < KI->getN(); k++) {
                    a[i + factor] += int(text[k + factor * i] - 'a') * (KI->org()[i][k]);
                }
                a[i + factor] %= ring;
                plaintTmpText += char(a[i + factor]) + 'a';
            }
            factor++;
            factor = factor * KI->getN();
        }
    }
    return plaintTmpText;
}

/* 解密 */
string ClassicalCipherHandler::decrypt(int key) {
    if (this->curAlgorithm == "tra")
        return simpleTranspositionDecrypt(key);
    else if (curAlgorithm == "caesar")
        return caesarCipherDecrypt(key);
    else return "WRONG";
}

/* 解密2 */
string ClassicalCipherHandler::decrypt(string key) {
    if (this->curAlgorithm == "vigenere")
        return vigenereDecrypt(key);
    else return "WRONG";
}

/* 解密3 */
string ClassicalCipherHandler::decrypt(Matrix<int> *key) {
    if (this->curAlgorithm == "hill")
        return hillDecrypt(key);
    else return "WRONG";
}


/* 加密 */
string ClassicalCipherHandler::encrypt(int key) {
    if (this->curAlgorithm == "tra")
        return simpleTranspositionEncrypt(key);
    else if (curAlgorithm == "caesar")
        return caesarCipherEncrypt(key);
    else return "WRONG";
}

/* 加密2 */
string ClassicalCipherHandler::encrypt(string key) {
    if (this->curAlgorithm == "vigenere")
        return vigenereEncrypt(key);
    else return "WRONG";
}

/* 加密3 */
string ClassicalCipherHandler::encrypt(Matrix<int> *key) {
    if (this->curAlgorithm == "hill")
        return hillEncrypt(key);
    else return "WRONG";
}


void ClassicalCipherHandler::inputControl(char &exit) {
    while (true) {
        cout << "是否重新输入文本(y重新输入，n退出程序)?" << "[y/n]" << endl;
        cin >> exit;
        if (exit != 'y' && exit != 'n') continue;
        else {
            if (exit == 'y')
                cout << "开始重新输入" << endl;
            else
                cout << "退出程序" << endl;
            cin.get();
            break;
        }
    }
}

void ClassicalCipherHandler::cipherExecutor(char &choice) {
    int key, N;
    string keyV;

    Matrix<int> *keyHill;
    cout << "请输入密钥:\n";

    if (this->curAlgorithm == "vigenere") cin >> keyV;
    else if (this->curAlgorithm == "hill") {
        cout << "请输入方阵阶数\n";
        cin >> N;
        keyHill = new Matrix<int>(N);
        cout << "请输入密钥为" << N << "阶的整型矩阵(矩阵元素x∈[0, 25]):\n";
        inputMatrix(keyHill);
    }
    else cin >> key;

    while (true) {
        cout << "是否加密?(y加密，n解密)" << "[y/n]" << endl;
        cin >> choice;
        if (choice != 'y' && choice != 'n') continue;
        else {
            if (choice == 'y') {
                if (this->curAlgorithm == "vigenere") cout << "[加密结果]：" << this->encrypt(keyV) << endl;
                else if (this->curAlgorithm == "hill") { cout << "[加密结果]：" << this->encrypt(keyHill) << endl; }
                else cout << "[加密结果]：" << this->encrypt(key) << endl;
            } else {
                if (this->curAlgorithm == "vigenere") cout << "[解密结果]：" << this->decrypt(keyV) << endl;
                else if (this->curAlgorithm == "hill") { cout << "[解密结果]：" << decrypt(keyHill) << endl; }
                else cout << "[解密结果]：" << this->decrypt(key) << endl;
            }
            break;
        }
    }
}

void ClassicalCipherHandler::handleAlgorithm(string &algorithm) {
    cout << "*************************\n";
    cout << "选择一个加密算法(输入选项方括号的字符串)\n";
    for (pair<string, string> each: algorithms) {
        cout << "[" << each.first << "]:" << each.second << endl;
    }
    cout << "*************************\n";
    while (cin >> algorithm && !this->setCurAlgorithm(algorithm));
}


int main() {

    char choice, exit = 'Q';
    string text, algorithm;
    while (!cin.eof() && exit != 'n') {
        cout << "请输入一段文字:(明文或密文)\n";
        getline(cin, text);
        ClassicalCipherHandler cipherHandler(text);
        cipherHandler.handleAlgorithm(algorithm);
        cipherHandler.cipherExecutor(choice);
        cipherHandler.inputControl(exit);
    }
    return 0;
}
