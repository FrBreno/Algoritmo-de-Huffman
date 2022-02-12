#include "ftab.h"

ftab::ftab(const char *filename)
{
    for (int i = 0; i < 256; i++)
    {
        tab[i].first = i;
        tab[i].second = 0;
    }

    FILE *file = fopen(filename, "rb");
    mychar c;
    while (!feof(file))
    {
        fread(&c, sizeof(char), 1, file);
        tab[c].second += 1;
    }
    fclose(file);

    // sort();
}

myint ftab::operator[](mychar k)
{
    return tab[k].second;
}

void ftab::print()
{
    for (int i = 0; i < 256; i++)
    {
        if ((int)tab[i].first == 0)
        {
            cout << (int)tab[i].first << "     NULL" << '\t' << tab[i].second << '\n';
        }
        else if ((int)tab[i].first < 10)
        {
            cout << (int)tab[i].first << "   [control]" << '\t' << tab[i].second << '\n';
        }
        else if ((int)tab[i].first < 32 || (int)tab[i].first == 127)
        {
            cout << (int)tab[i].first << "  [control]" << '\t' << tab[i].second << '\n';
        }
        else if ((int)tab[i].first == 32)
        {
            cout << (int)tab[i].first << "   [space]" << '\t' << tab[i].second << '\n';
        }
        else
        {
            cout << (int)tab[i].first << '\t' << tab[i].first << '\t' << tab[i].second << '\n';
        }
    }
}

void ftab::sort()
{
    for (int j = 255; j > 0; j--)
    {
        for (int i = 0; i < j; i++)
        {
            if (tab[i].second > tab[i + 1].second)
            {
                pair<mychar, myint> aux = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = aux;
            }
        }
    }
}
