#include "Generator.h"
#include <QtMath>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#if defined (Q_OS_UNIX)
#include <QProcess>
#endif

//#ifdef DEBUG
#include <QDebug>
//#endif

Generator::Generator(const int &seqSize,
                     const int &terminalSideLobes,
                     bool isFiltered,
                     const int &closeCentralSideLobes,
                     QObject *parent) :
    QObject(parent)
  , m_isFiltered(isFiltered)
  , m_progressBar("[ ]")
  , m_progress(0)
{
    setSequenceSize(seqSize);
    setCloseCentralSideLobes(closeCentralSideLobes);
    setTerminalSideLobes(terminalSideLobes);
    fillCombinations();
}

Generator::~Generator()
{
    delete[] m_combs;
    delete[] m_combsSimplified;
}

void Generator::setCloseCentralSideLobes(const int &closeCentralSideLobes)
{
    if(qAbs(closeCentralSideLobes)) {
        m_closeCentralSideLobes = qAbs(closeCentralSideLobes);
    } else {
        qErrnoWarning("WARNING! Close-central side lobes must be equal or greater than 1");
        qErrnoWarning("Close-central side lobes set to 1");
        m_closeCentralSideLobes = 1;
    }
}

void Generator::setTerminalSideLobes(const int &absMaxScatter)
{
    if(qAbs(absMaxScatter)) {
        m_terminalSideLobes = absMaxScatter;
    } else {
        qErrnoWarning("WARNING! Terminal side lobes must be equal or greater than 1");
        qErrnoWarning("Terminal side lobes set to 1");
        m_terminalSideLobes = 1;
    }
}

void Generator::setFiltered(const bool &isFiltered)
{
    m_isFiltered = isFiltered;
}

bool Generator::isFiltered() const
{
    return m_isFiltered;
}

int Generator::getCloseCentralSideLobes() const
{
    return m_closeCentralSideLobes;
}

int Generator::getTerminalSideLobes() const
{
    return m_terminalSideLobes;
}

void Generator::setSequenceSize(const int &seqSize)
{
    if(qAbs(seqSize) > 1) {
        m_seqSize = qAbs(seqSize);
    } else {
        qErrnoWarning("WARNING! Size of sequences must be equal or greater than 2");
        qErrnoWarning("Size set to 2");
        m_seqSize = 2;
    }
}

int Generator::getSequenceSize() const
{
    return m_seqSize;
}

QVector<bool> Generator::m_wasShown;

void Generator::generate()
{
    int phase = m_seqSize - 1;
//    Generator::m_wasShown.fill(false, 4);

    m_sequence.fill(0, m_seqSize);
    for(int combIndex = 0; combIndex < m_combSize; combIndex += 2) {
        m_sequence[m_seqSize - phase - 1] = m_combs[combIndex];
        m_sequence[phase] = m_combs[combIndex + 1];
        m_tempSequences.append(m_sequence);
    }

    QVector<QFuture<void> > fs;
    for(int i = 0; i < m_combSize/2; ++i) {
        fs.append(QtConcurrent::run(this, &Generator::gen, phase - 1, m_tempSequences[i], true/*, i*/));
    }

    for(int i = 0; i < m_combSize/2; ++i) {
        fs[i].waitForFinished();
    }
}

void Generator::progressBar()
{
#if defined (Q_OS_UNIX)
    QProcess::execute("clear");
#elif defined (Q_OS_WIN)
    system("cls");
#endif
    ++m_progress;
    if(!(m_progress % 50)) {
        m_progressBar.remove(m_progressBar.size() - 2, 2);
        m_progressBar.append("=>]");
    }
    if(m_progressBar.size() == 50){
        m_progressBar = "[ ]";
        m_progress = 1;
    }
    qDebug() << m_progressBar.toStdString().c_str();
    qDebug() << "Total found:" << m_sequences.size();
}


void Generator::gen(int phase, QVector<int> &seq, bool isSimplified/*, int i*/)
{
    //    if(i < 4 && !m_wasShown.at(i)) {
    //        qDebug() << "Thread:" << QThread::currentThreadId();
    //        m_wasShown[i] = true;
    //    }
#ifndef DEBUG
    //    progressBar();
#endif
#ifdef DEBUG
    qDebug() << "in gen(" << phase << ")";
#endif
    int summa = 0;

    if(phase >= phaseLimit() && phase < m_seqSize - 1) {
        if(isOddSeqSize() && (phase == phaseLimit())) {
            for(int combIndex = 0; combIndex < 2; ++combIndex) {
                seq[phase] = qPow(-1, combIndex + 1);
#ifdef DEBUG
                qDebug() << "level:" << m_seqSize - phase;
                qDebug() << "combinations(" << qPow(-1, combIndex + 1) << ")";
                qDebug() << "combIndex =" << combIndex;
                qDebug() << "m_sequence[" << phase << "] =" << m_sequence[phase];
#endif
                for(int index = 0; index < m_seqSize - phase; ++index) {
                    summa += seq.at(index)*seq.at(index + phase);
                }
#ifdef DEBUG
                qDebug() << "summa =" << summa;
#endif
                if(qAbs(summa) <= m_terminalSideLobes) {
                    gen(phase - 1, seq, isSimplified);
                } else {
                    m_sequence[phase] = 0;
#ifdef DEBUG
                    qDebug() << "Wrong branch!";
#endif
                }
                summa = 0;
            }
        } else if(isSimplified && !(seq.at(m_seqSize - phase)^seq.at(phase + 1))) {
            for(int combIndex = 0; combIndex < m_combSizeSimplified; combIndex += 2) {
                seq[m_seqSize - phase - 1] = m_combs[combIndex];
                seq[phase] = m_combs[combIndex + 1];
#ifdef DEBUG
                qDebug() << "level:" << m_seqSize - phase;
                qDebug() << "combinations(" << m_combs[combIndex] << "," << m_combs[combIndex + 1] << ")";
                qDebug() << "combIndex =" << combIndex;
#endif
                for(int index = 0; index < m_seqSize - phase; ++index) {
                    summa += seq.at(index)*seq.at(index + phase);
                }
#ifdef DEBUG
                qDebug() << "summa =" << summa;
#endif
                if(qAbs(summa) <= m_terminalSideLobes) {
                    gen(phase - 1, seq, isSimplified);
                } else {
                    seq[m_seqSize - phase - 1] = 0;
                    seq[phase] = 0;
#ifdef DEBUG
                    qDebug() << "Wrong branch!";
#endif
                }
                summa = 0;
            }
        } else {
            isSimplified = false;
            for(int combIndex = 0; combIndex < m_combSize; combIndex += 2) {
                seq[m_seqSize - phase - 1] = m_combs[combIndex];
                seq[phase] = m_combs[combIndex + 1];
#ifdef DEBUG
                qDebug() << "level:" << m_seqSize - phase;
                qDebug() << "combinations(" << m_combs[combIndex] << "," << m_combs[combIndex + 1] << ")";
                qDebug() << "combIndex =" << combIndex;
#endif
                for(int index = 0; index < m_seqSize - phase; ++index) {
                    summa += seq.at(index)*seq.at(index + phase);
                }
#ifdef DEBUG
                qDebug() << "summa =" << summa;
#endif
                if(qAbs(summa) <= m_terminalSideLobes) {
                    gen(phase - 1, seq, isSimplified);
                } else {
                    seq[m_seqSize - phase - 1] = 0;
                    seq[phase] = 0;
#ifdef DEBUG
                    qDebug() << "Wrong branch!";
#endif
                }
                summa = 0;
            }
        }
    } else if((m_isFiltered && filter(seq)) || !m_isFiltered) {
#ifdef DEBUG
        qDebug() << "emit sequenceGenerated(" << m_sequence << ")";
#endif
        QMutexLocker locker(&m_mutex); // ? need or not
        emit sequenceGenerated(seq);
        m_sequences.append(seq);
    }
}

int Generator::phaseLimit()
{
    return (isOddSeqSize() ? (m_seqSize - 1)/2 : m_seqSize/2);
}

bool Generator::isOddSeqSize()
{
    return (m_seqSize & 1);
}

bool Generator::filter(const QVector<int> &seq)
{
    int summa = 0;

    for(int phase = 1; phase < ((m_seqSize & 1) ? (m_seqSize - 1)/2 : m_seqSize/2); ++phase) {
        for(int index = 0; index < m_seqSize - phase; ++index) {
            summa += seq.at(index)*seq.at(index + phase);
        }
#ifdef DEBUG
        qDebug() << "[!]*****bool Generator::filter():" << (qAbs(summa) <= m_closeCentralSideLobes);
        qDebug() << "[!]*****summa =" << summa;
        qDebug() << "[!]*****bool Generator::filter() | phase:" << phase;
#endif
        if(qAbs(summa) > m_closeCentralSideLobes) {
            return false;
        }
        summa = 0;
    }

    return true;
}

QVector<QVector<int> > Generator::getSequences()
{
    return m_sequences;
}

void Generator::fillCombinations()
{
    int v[] = {-1, -1, -1, 1, 1, -1, 1, 1};
    int vSimple[] = {-1, -1, -1, 1, 1, 1};
    m_combSize = 8;
    m_combSizeSimplified = 6;
    m_combs = new int[m_combSize];
    m_combsSimplified = new int[m_combSizeSimplified];

    for(int i = 0; i < m_combSize; ++i) {
        m_combs[i] = v[i];
    }

    for(int i = 0; i < m_combSizeSimplified; ++i) {
        m_combsSimplified[i] = vSimple[i];
    }
}
