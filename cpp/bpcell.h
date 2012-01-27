#ifndef BPCELL_H
#define BPCELL_H

#include <vector>
#include "bpevent.h"

class BPCell
{
public:
    BPCell();
    virtual ~BPCell();

    void add(BPEvent e);
    void clean(unsigned int oldT);
    unsigned int size() const;

private:
    unsigned int maxSize;
    unsigned int size_;
    unsigned int a;
    unsigned int b;
    BPEvent * events;
};

#endif // BPCELL_H
