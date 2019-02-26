#ifndef UID_H
#define UID_H

#include <QObject>

class Uid : public QObject{
public:
    Uid();
    quint32 next ();
    ~Uid();
private:
    quint32 m_id;
};

#endif // UID_H
