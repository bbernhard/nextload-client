#include "uid.h"

Uid::Uid()
    : QObject(0),
      m_id(0)
{
}

quint32 Uid::next(){
    return m_id++;
}

Uid::~Uid(){
}
