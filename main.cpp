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
    algorithms.insert(pair<string, string>("tra", "�û��㷨"));
    algorithms.insert(pair<string, string>("caesar", "�����㷨"));
    algorithms.insert(pair<string, string>("vigenere", "ά�������㷨"));
    algorithms.insert(pair<string, string>("hill", "ϣ�������㷨"));
}

/* ��ȡ�����ı� */
const string &ClassicalCipherHandler::getText() const {
    return text;
}

/* ���õ�ǰѡ����㷨 */
bool ClassicalCipherHandler::setCurAlgorithm(const string &algorithm) {
    bool hasIn = false;

    if (algorithms.find(algorithm) != algorithms.end()) {
        ClassicalCipherHandler::curAlgorithm = algorithm;
        return true;
    } else {
        cout << "������㷨����\n";
        return false;
    }
}

/* �򵥵��û�����--���� */
string ClassicalCipherHandler::simpleTranspositionEncrypt(int key) {
    string cipherTmpText;
    auto textLen = text.length();
    auto tmpIndex = 0;
    /* ���� from 0 to Key-1 ��ÿһ�� */
    for (auto column = 0; column < key; column++) {
        tmpIndex = column;
        while (tmpIndex < textLen) {
            cipherTmpText += text[tmpIndex];
            tmpIndex += key;
        }
    }
    //ʹ�÷��� "|"�������Ľ�β����ֹĩλ���ڿո���ʶ��
    this->text = cipherTmpText;
    return cipherTmpText + "|";
}

/* �򵥵��û�����--���� */
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


/* ��������--���� */
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

/* ��������--���� */
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


/* ά������������� */
string ClassicalCipherHandler::vigenereEncrypt(string key) {
    string cipherTmpText;

    auto ring = VIGENERE_SYMBOLS.length();
    auto i = 0;
    /* ����Կ-�ַ���ת��Ϊ��д */
    transform(key.begin(), key.end(), key.begin(), ::toupper);
    for (char each : this->text) {
        auto num = VIGENERE_SYMBOLS.find((char) toupper(each));
        if (num != string::npos) {
            num += VIGENERE_SYMBOLS.find(key[i]);

            /* ����ػ� */
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

/* ά������������� */
string ClassicalCipherHandler::vigenereDecrypt(string key) {
    string plaintTmpText;
    const auto ring = VIGENERE_SYMBOLS.length();
    auto keySymbolPos = 0;
    /* ����Կ-�ַ���ת��Ϊ��д */
    transform(key.begin(), key.end(), key.begin(), ::toupper);
    auto posInDic = 0;
    /* ������Կ�����ĵ�ÿ����ĸ���н��� */
    for (char each : this->text) {
        /* �ҵ���ǰ������ĸ���ֵ��е�λ�� */
        posInDic = VIGENERE_SYMBOLS.find((char) (toupper(each)));
        if (posInDic != string::npos) {

            //cout <<  "������ĸ ��Ӧ�ֵ��д��ڵ��ַ�Ϊ��" << VIGENERE_SYMBOLS[posInDic] <<" in "<<posInDic << endl;
            /* �ҵ���ǰ��Կ�ַ����ֵ��е�λ�� */
            const auto posInDicKey = VIGENERE_SYMBOLS.find(key[keySymbolPos]);
            //cout << std::boolalpha <<(key[keySymbolPos] == VIGENERE_SYMBOLS[25]) << endl;
            if (posInDicKey != string::npos) {
                //cout <<  "��ǰ��Կ��ĸΪ��" << key[keySymbolPos]<< endl;
                posInDic -= posInDicKey;
            }
            //cout << "���������ַ�λ��(δģ����)" << posInDic << "��ǰ�����ַ����" << keySymbolPos<<endl;

            /* ģ�������ģ���㲿�֣�����ػ� :����C++ ��0ȡ������Ҫ�Ը���������ģ�������*/
            posInDic = (posInDic < 0) ? (posInDic + ring) : posInDic;
            posInDic %= ring;
            //cout << "���������ַ�λ��" << posInDic<<endl;
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


/* ϣ��������� */
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

/* ϣ��������� */
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

/* ���� */
string ClassicalCipherHandler::decrypt(int key) {
    if (this->curAlgorithm == "tra")
        return simpleTranspositionDecrypt(key);
    else if (curAlgorithm == "caesar")
        return caesarCipherDecrypt(key);
    else return "WRONG";
}

/* ����2 */
string ClassicalCipherHandler::decrypt(string key) {
    if (this->curAlgorithm == "vigenere")
        return vigenereDecrypt(key);
    else return "WRONG";
}

/* ����3 */
string ClassicalCipherHandler::decrypt(Matrix<int> *key) {
    if (this->curAlgorithm == "hill")
        return hillDecrypt(key);
    else return "WRONG";
}


/* ���� */
string ClassicalCipherHandler::encrypt(int key) {
    if (this->curAlgorithm == "tra")
        return simpleTranspositionEncrypt(key);
    else if (curAlgorithm == "caesar")
        return caesarCipherEncrypt(key);
    else return "WRONG";
}

/* ����2 */
string ClassicalCipherHandler::encrypt(string key) {
    if (this->curAlgorithm == "vigenere")
        return vigenereEncrypt(key);
    else return "WRONG";
}

/* ����3 */
string ClassicalCipherHandler::encrypt(Matrix<int> *key) {
    if (this->curAlgorithm == "hill")
        return hillEncrypt(key);
    else return "WRONG";
}


void ClassicalCipherHandler::inputControl(char &exit) {
    while (true) {
        cout << "�Ƿ����������ı�(y�������룬n�˳�����)?" << "[y/n]" << endl;
        cin >> exit;
        if (exit != 'y' && exit != 'n') continue;
        else {
            if (exit == 'y')
                cout << "��ʼ��������" << endl;
            else
                cout << "�˳�����" << endl;
            cin.get();
            break;
        }
    }
}

void ClassicalCipherHandler::cipherExecutor(char &choice) {
    int key, N;
    string keyV;

    Matrix<int> *keyHill;
    cout << "��������Կ:\n";

    if (this->curAlgorithm == "vigenere") cin >> keyV;
    else if (this->curAlgorithm == "hill") {
        cout << "�����뷽�����\n";
        cin >> N;
        keyHill = new Matrix<int>(N);
        cout << "��������ԿΪ" << N << "�׵����;���(����Ԫ��x��[0, 25]):\n";
        inputMatrix(keyHill);
    }
    else cin >> key;

    while (true) {
        cout << "�Ƿ����?(y���ܣ�n����)" << "[y/n]" << endl;
        cin >> choice;
        if (choice != 'y' && choice != 'n') continue;
        else {
            if (choice == 'y') {
                if (this->curAlgorithm == "vigenere") cout << "[���ܽ��]��" << this->encrypt(keyV) << endl;
                else if (this->curAlgorithm == "hill") { cout << "[���ܽ��]��" << this->encrypt(keyHill) << endl; }
                else cout << "[���ܽ��]��" << this->encrypt(key) << endl;
            } else {
                if (this->curAlgorithm == "vigenere") cout << "[���ܽ��]��" << this->decrypt(keyV) << endl;
                else if (this->curAlgorithm == "hill") { cout << "[���ܽ��]��" << decrypt(keyHill) << endl; }
                else cout << "[���ܽ��]��" << this->decrypt(key) << endl;
            }
            break;
        }
    }
}

void ClassicalCipherHandler::handleAlgorithm(string &algorithm) {
    cout << "*************************\n";
    cout << "ѡ��һ�������㷨(����ѡ����ŵ��ַ���)\n";
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
        cout << "������һ������:(���Ļ�����)\n";
        getline(cin, text);
        ClassicalCipherHandler cipherHandler(text);
        cipherHandler.handleAlgorithm(algorithm);
        cipherHandler.cipherExecutor(choice);
        cipherHandler.inputControl(exit);
    }
    return 0;
}
