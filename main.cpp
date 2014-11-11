#include <QCoreApplication>
#include <QVector>
#include <QString>
#include <QDebug>

#define UPPER_LIMIT "111111000"
#define LOWER_LIMIT "000001111"

void print(const QVector<bool> &result)
{
    QString tempStr;

    for(int i = 0; i < result.size(); ++i) {
        if(result.at(i)) {
            tempStr.append("1");
        } else {
            tempStr.append("0");
        }
    }

    qDebug() << tempStr;
}

void print(const QVector<QVector<bool> > &results)
{
    QString tempStr;

    for(int i = 0; i < results.size(); ++i) {
        for(int j = 0; j < results.at(i).size(); ++j) {
            if(results.at(i).at(j)) {
                tempStr.append("1");
            } else {
                tempStr.append("0");
            }
        }

        qDebug() << tempStr;
        tempStr.clear();
    }
}

bool isValid(const QVector<bool>  &result)
{
    bool temp;
    int zeros = 0;
    int units = 0;

    for(int i = 0; i < result.size(); ++i) {
        temp = result.value(i);

        if(temp) {
            ++units;
        } else {
            ++zeros;
        }
    }

    if(result.size() % 2) {
        if((zeros + 1 == units) || (zeros == units + 1)) {
            return true;
        } else {
            return false;
        }
    } else {
        if(zeros == units) {
            return true;
        } else {
            return false;
        }
    }
}

bool isEqual(const QVector<bool> &source1, const QString &source2) {
    if(source1.size() != source2.size()) {
        qDebug() << "don't match";
        return false;
    }

    QString tempStr = "0";

    qDebug() << "source1:";
    print(source1);
    qDebug() << "source2:";
    qDebug() << source2;

    bool stringSymbol = false;

    for(int i = 0; i < source1.size(); ++i) {
        if(source2.at(i) == tempStr.at(0)) {
            stringSymbol = false;
        } else {
            stringSymbol = true;
        }

        if(source1.at(i) != stringSymbol) {
            return false;
        }
    }

    qDebug() << "TRUE!!!";

    return true;
}

void counter(QVector<QVector<bool> > &results, const int &digits)
{
    QVector<bool> tempResult;

    tempResult.fill(false, digits);

//    bool isWasLowerLimit = false;
    int k = 0;

    while(true) {
        qDebug() << "digits =" << digits;
        for(int i = 0; i < tempResult.size(); ++i) {
            if(tempResult.at(i)) {
                ++k;
            }
        }

        if(k == tempResult.size()) {
            break;
        }

        k = 0;

        for(int m = 1; m < tempResult.size(); ++m) {
            for(int i = m; i < tempResult.size(); ++i) {
                if(tempResult.at(i)) {
                    ++k;
                }
            }

            if(k == tempResult.size() - m) {
                for(int i = m; i < tempResult.size(); ++i) {
                    tempResult.replace(i, false);
                }

                tempResult.replace(m - 1, true);
            }

            k = 0;
        }

        if(/*isWasLowerLimit && */isValid(tempResult)) {
            results.push_back(tempResult);
            print(tempResult);
        }
//        results.push_back(tempResult);
//        print(tempResult);
        tempResult.replace(tempResult.size() - 1, true);
//        results.push_back(tempResult);
//        print(tempResult);

//        if(isEqual(tempResult, LOWER_LIMIT)) {
//            isWasLowerLimit = true;
//            qDebug() << "isWasLowerLimit" << isWasLowerLimit;
//        }

//        if(isEqual(tempResult, UPPER_LIMIT)) {
//            qDebug() << "isWasUpperLimit";
//            return;
//        }

//        if(isWasLowerLimit && isValid(tempResult)) {
//            results.push_back(tempResult);
//        }

//        if(isEqual(tempResult, LOWER_LIMIT)) {
//            isWasLowerLimit = true;
//            qDebug() << "isWasLowerLimit" << isWasLowerLimit;
//        }

//        if(isEqual(tempResult, UPPER_LIMIT)) {
//            qDebug() << "isWasUpperLimit";
//            return;
//        }

        if(/*isWasLowerLimit && */isValid(tempResult)) {
            results.push_back(tempResult);
            print(tempResult);
        }
    }
}

void fromBinToHex(const QVector<QVector<bool> > &results)
{
    QString tempStr;
    QString tempStrHex;
    QString strHex;

    for(int i = 0; i < results.size(); ++i) {
        for(int j = 0; j < results.at(i).size(); ++j) {
            if(results.at(i).at(j)) {
                tempStr.append("1");
            } else {
                tempStr.append("0");
            }
        }

        for(int j = 1; j < tempStr.size(); ++j) {
            tempStrHex += tempStr.at(j);
            if(!(j % 4)) {
                if(tempStrHex == "0000") {
                    strHex += "0";
                }
                if(tempStrHex == "0001") {
                    strHex += "1";
                }
                if(tempStrHex == "0010") {
                    strHex += "2";
                }
                if(tempStrHex == "0011") {
                    strHex += "3";
                }
                if(tempStrHex == "0100") {
                    strHex += "4";
                }
                if(tempStrHex == "0101") {
                    strHex += "5";
                }
                if(tempStrHex == "0110") {
                    strHex += "6";
                }
                if(tempStrHex == "0111") {
                    strHex += "7";
                }
                if(tempStrHex == "1000") {
                    strHex += "8";
                }
                if(tempStrHex == "1001") {
                    strHex += "9";
                }
                if(tempStrHex == "1010") {
                    strHex += "A";
                }
                if(tempStrHex == "1011") {
                    strHex += "B";
                }
                if(tempStrHex == "1100") {
                    strHex += "C";
                }
                if(tempStrHex == "1101") {
                    strHex += "D";
                }
                if(tempStrHex == "1110") {
                    strHex += "E";
                }
                if(tempStrHex == "1111") {
                    strHex += "F";
                }
                tempStrHex.clear();
            }
        }
        strHex.push_front(tempStr.at(0));

        qDebug() << tempStr << strHex;
        tempStr.clear();
        strHex.clear();
    }
}

int fromBoolToInt(const bool &boolValue)
{
    if(boolValue) {
        return 1;
    } else {
        return -1;
    }
}

int correlation(const QVector<bool> &source1, const QVector<bool> &source2)
{
    int tempI = 0;
    int summa = 0;

    for(int i = 0; i < source1.size() + source2.size() - 1; ++i) {
        for(int j = 0; j < source2.size(); ++j) {
            if(i < source1.size()) {
                tempI = i;
            } else {
                tempI = source1.size() - 1;
            }

            summa += fromBoolToInt(source1.at(tempI))*fromBoolToInt(source2.at(source2.size() - 1 - j));
            --tempI;

            if(tempI < 0 || (source2.size() - 1 - j < 0)) {
                break;
            }
        }
    }

    return summa;
}

QVector<bool> subSequence(const QVector<bool> &source, const int &offset, const int &digits)
{
    if(offset + digits > source.size()) {
        qDebug() << "exception";
    }

    QVector<bool> result;

    for(int i = offset; i < offset + digits; ++i) {
        result.append(source.at(i));
    }

    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<QVector<bool> > vec1;
    QVector<QVector<bool> > vec2;

    counter(vec1, 11);
    print(vec1);
    counter(vec2, 5);
    print(vec2);
//    print(vec);

//    qDebug() << "Size" << vec.size();

//    fromBinToHex(vec);

//    qDebug() << "Size" << vec.size();

    for(int i = 0; i < vec1.size(); ++i) {
        for(int j = 0; j < vec2.size(); ++j) {
            qDebug() << correlation(vec1.at(i), vec2.at(j));
        }
    }

    print(vec1.at(0));
    qDebug() << "Sub Sequence";
    print(subSequence(vec1.at(0), 2, 6));

    return a.exec();
}
